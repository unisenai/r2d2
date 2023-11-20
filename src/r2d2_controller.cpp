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
static void read_sensor();

//
enum POSITION
{
  POS_INITIAL = 0,
  POS_OBSTACLE_1,
  POS_OBSTACLE_2,
  POS_OBSTACLE_3,
  POS_WALL,
  POS_END
};

// Local variables
volatile int8_t position = POS_INITIAL;
volatile bool found_block = false;

uint16_t sensorValue = MIN_VALUE_FOR_BLOCK;

unsigned long timer_start_block = 0;
unsigned long timer_end_block = 0;

unsigned long timer_start_free = 0;
unsigned long timer_end_free = 0;

volatile unsigned long timer_start_rotate = 0;
volatile unsigned long timer_end_rotate = 0;
volatile unsigned long timer_pass_rotate = 0;

// Function to watch for the proximity sensor and reacts accordingly
void R2C_proximity_watcher()
{

  // Serial.print("  >> rotating: ");
  // Serial.println(rotating);

  // if (rotating)
  // {
  //   timer_pass_rotate = micros() - timer_start_rotate;
  //   R2M_rotate_left(TIME_ANGLE_90);
  // }
  // else
    read_sensor();
}

static void read_sensor()
{
  if (rotating)
    return;

  delayMicroseconds(500);
  // We need to STOP IMMEDIATELY because we either found a blocker or a clear path
  //    and in any case we need to quickly react changing the direction of our movement!

  if (!found_block)
  {
    R2M_release_all();
    delayMicroseconds(500);

    // We immediately change the variable to prevent other calls to enter the loop
    //  and this works as a poor-man lock mechanism
    found_block = true;
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
          timer_end_block = micros();
          first_iter = false;

          // Ok, it's more than MIN_TIME_BETWEEN_READS microseconds
          if (!timer_start_block || (timer_end_block - timer_start_block) > MIN_TIME_BETWEEN_READS)
            position++;

          // We restart our time counter
          timer_start_block = micros();
        }

        // Checking if we have the WAL in front of us
        if (position == POS_WALL)
        {
          // We got to the WAL, we need to do a 90 degrees turn to the LEFT and move forward
          noInterrupts();
          R2M_rotate_left(TIME_ANGLE_90);
          interrupts();
        }
        else if (position == POS_END)
        {
          // We got to the end of the line, time to make sure we stop
          R2M_release_all();
          delayMicroseconds(500);

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
        timer_end_free = micros();
        if (!timer_start_free || timer_end_free - timer_start_free < MIN_TIME_BETWEEN_READS)
          return;

        // We only have one front sensor, so we'll keep moving for more 500ms to make sure we don't collide to the block
        R2M_release_all();
        delayMicroseconds(500);

        //
        timer_start_free = micros();

        return;
      }
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
