/**
 * @file r2d2_controller.cpp
 * @author Charly Batista (carlbsb@gmail.com)
 * @brief R2D2 is am autonomous guided vehicle (AGV) created as part of the Mechatronics undergraduate program in UniSenai Joinville/Brazil
 * @version 0.1
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
#include <HardwareSerial.h>
#include "include/r2d2.h"
#include "include/r2d2_motor.h"

static void R2C_loop_move(R2Direction direction);

volatile bool interrupt_attached = true;

uint16_t sensorValue = MIN_VALUE_FOR_BLOCK;

unsigned long start_timer_block = 0;
unsigned long end_timer_block = 0;

unsigned long start_timer_free = 0;
unsigned long end_timer_free = 0;

// Function to watch for the proximity sensor and reacts accordingly
void R2C_proximity_watcher()
{
  // Serial.println(" <<<<< INTERRUPT >>>>>");
  found_block = true;
  // read_sensor();
}

void R2C_read_sensor()
{
  delayMicroseconds(5);

  if (found_block)
  {
    // We then IMMEDIATELY stop because we found a blocker
    //    and we need to quickly react changing the direction of our movement!
    R2M_release_all();
    // delayMicroseconds(500);

    // We temporarily disable the PROXIMITY INTERRUPT because we'll work inside the WHILE loop
    // clearInterrupt(digitalPinToInterrupt(PROXIMITY_INTER_PIN));
    Serial.println("  >>>>> DETACH INTERRUPT <<<<<");
    detachInterrupt(digitalPinToInterrupt(PROXIMITY_INTER_PIN));
    interrupt_attached = false;

    bool first_iter = true;

    while (started)
    {
      // We check the sensor to make sure we found a block
      sensorValue = analogRead(DISTANCE_PIN);

      // Yes, we found a block!
      if (sensorValue > MIN_VALUE_FOR_BLOCK)
      {
        Serial.print("Sensor Value: ");
        Serial.println(sensorValue);

        // We check if this is the first iteration for this loop and increment the position
        if (first_iter)
        {
          // We just do a sanity check to make sure we don't increment the position in a too short time.
          // It prevents sensor misreads to cause our logic to go wrong!
          end_timer_block = millis();
          first_iter = false;

          // Ok, it's more than MIN_TIME_BETWEEN_READS microseconds
          if (!start_timer_block || (end_timer_block - start_timer_block) > MIN_TIME_BETWEEN_READS)
            position++;

          // We restart our time counter
          start_timer_block = millis();
        }

        // Checking if we have the WAL in front of us
        if (position == POS_WALL)
        {
          // We got to the WAL, we need to do a 90 degrees turn to the LEFT and move forward
          R2M_rotate_left(1);

          goto ENABLE_AND_EXIT;
        }
        else if (position == POS_END)
        {
          // We got to the end of the line, time to make sure we FORCE stop
          R2M_release_all();

          // Now we just change the STARTED flag to FALSE
          started = false;
          // found_block = false;
          // rotating = false;
          // position = POS_INITIAL;
        }
        else
        {
          // We just have another block, so moving to the RIGHT
          R2M_move_right();
        }
      }

      // It seems we got a free path
      else
      {
        found_block = false;
        R2M_move_fw();

        Serial.println("Free path!");
        Serial.print("Position: ");
        Serial.println(position);

        //
        end_timer_free = millis();
        if (!start_timer_free || end_timer_free - start_timer_free < MIN_TIME_BETWEEN_READS)
        {
          attachInterrupt(digitalPinToInterrupt(PROXIMITY_INTER_PIN), R2C_proximity_watcher, RISING);
          goto ENABLE_AND_EXIT;
        }

        // We only have one front sensor, so we'll keep moving for more 500ms to make sure we don't collide to the block
        R2M_release_all();
        delayMicroseconds(500);

        //
        start_timer_free = millis();
        goto ENABLE_AND_EXIT;
      }
    }

  ENABLE_AND_EXIT:
    if (!interrupt_attached)
    {
      Serial.println("  >>>>> ATTACH INTERRUPT <<<<<");
      attachInterrupt(digitalPinToInterrupt(PROXIMITY_INTER_PIN), R2C_proximity_watcher, RISING);
    }
  }
}

// Function that starts and/or shuts down the AGV
void R2C_power_watcher()
{
  Serial.println("Starting!!!!");

  // We reset the variables
  // found_block = false;
  // position = POS_INITIAL;
  started = true;
}
