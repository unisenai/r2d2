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

/*
void R2C_read_sensor()
{
  delayMicroseconds(5);

  if (found_block)
  {
    // We then IMMEDIATELY stop because we found a blocker
    //    and we need to quickly react changing the direction of our movement!
    R2M_release_all();
    delay(5);

    // We temporarily disable the PROXIMITY INTERRUPT because we'll work inside the WHILE loop
    // clearInterrupt(digitalPinToInterrupt(PROXIMITY_INTER_PIN));
    Serial.println("  >>>>> DETACH INTERRUPT <<<<<");
    detachInterrupt(digitalPinToInterrupt(PROXIMITY_INTER_PIN));
    interrupt_attached = false;

    bool first_iter = true;
    bool led_on = false;
    unsigned long led_timer_in = 0;
    unsigned long led_timer_out = 0;

    while (started)
    {
      // We check the sensor to make sure we found a block
      sensorValue = analogRead(PIN_DISTANCE);

      // Yes, we found a block!
      if (sensorValue > MIN_VALUE_FOR_BLOCK)
      {
        // Toggle the RED LED
        {
          led_timer_out = millis();
          if (led_timer_in == 0 || (led_timer_out - led_timer_in) >= 100)
          {
            if(led_on)
              digitalWrite(LED_RED_PIN, LOW);
            else
              digitalWrite(LED_RED_PIN, HIGH);
            led_timer_in = millis();
            led_on = !led_on;
          }
        }

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
          {
            position++;
            delay(500);
          }

          // We restart our time counter
          start_timer_block = millis();
        }

        // Checking if we have the WAL in front of us
        if (position == POS_WALL)
        {
          // We got to the WAL, we need to do a 90 degrees turn to the LEFT and move forward
          delay(200);
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
        if (!first_iter)
        {
          R2M_move_right();
          delay(DELAY_TO_AVOID_COLLISION);
        }
        else
        {
          R2M_move_fw();
        }

        found_block = false;

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
        // R2M_release_all();
        // delay(500);

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

*/

// Function that starts and/or shuts down the AGV
void R2C_power_watcher_2()
{
  Serial.println("Starting!!!!");

  // We reset the variables
  // position = POS_INITIAL;
  started++;
}

uint64_t duration;

uint16_t R2C_get_distance(uint8_t num_avg)
{

  uint16_t dist_sum;
  uint16_t dist_tmp;

  if (num_avg < MIN_NUM_DIST_READS)
    num_avg = MIN_NUM_DIST_READS;

  dist_sum = 0;
  for (int i = 0; i < num_avg; i++)
  {
    // Clears the PIN_TRIG
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    // Sets the PIN_TRIG on HIGH state for 10 micro seconds
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    // Reads the PIN_ECHO, returns the sound wave travel time in microseconds
    duration = pulseIn(PIN_ECHO, HIGH);
    // Calculating the distance
    dist_tmp = (duration * 0.034 / 2);
    if (dist_tmp == 0 || dist_tmp > 100)
    {
      num_avg--;
      continue;
    }

    dist_sum += dist_tmp;
  }

  return (dist_sum / num_avg);
}

/**
 * We check if we got an erratic read
 */
bool is_erratic(bool check_min_distance)
{
  // Do another NUM_ERRATIC_CHECKS checks
  uint16_t dist_sum = 0;
  for (int j = 0; j <= NUM_ERRATIC_CHECKS; j++)
  {
    // We use the default minimum value for the checks avg
    dist_sum += R2C_get_distance(MIN_NUM_DIST_READS);
  }

  // If the average of the reads are larger than the minimum distance allowed we just continue, it was an erratic read
  if ((check_min_distance && (dist_sum / NUM_ERRATIC_CHECKS) > MIN_VALUE_FOR_BLOCK) ||
      (!check_min_distance && (dist_sum / NUM_ERRATIC_CHECKS) < MIN_VALUE_FOR_BLOCK))
    return true;

  return false;
}

void R2C_controller(uint16_t ndist)
{
  uint16_t distance = ndist;

  //
  if (is_erratic(true))
  {
    Serial.println("  >>>>>>> ERRATIC <<<<<<<");
    return;
  }

  position++;
  R2M_release_all();
  delay(DELAY_FOR_CHANGE_DIRECTION);

  unsigned long led_timer_in = 0;
  unsigned long led_timer_out = 0;
  bool led_on = false;

  while (true)
  {

    // Toggle the RED LED
    {
      led_timer_out = millis();
      if (led_timer_in == 0 || (led_timer_out - led_timer_in) >= 100)
      {
        if (led_on)
          digitalWrite(PIN_LED_RED, LOW);
        else
          digitalWrite(PIN_LED_RED, HIGH);
        led_timer_in = millis();
        led_on = !led_on;
      }
    }

    switch (position)
    {
    case POS_OBSTACLE_1:
    case POS_OBSTACLE_2:
    case POS_OBSTACLE_3:
      R2M_move_right();

      break; // -- END POS_OBSTACLE_X

    case POS_WALL:
      R2M_rotate_left(TIME_ANGLE_90);
      delay(DELAY_FOR_CHANGE_DIRECTION + 200);

      return;
      break; // -- END POS_WALL

    case POS_END:
      // We got to the end of the line, time to make sure we FORCE stop
      R2M_release_all();
      delay(5000);

      // Now we just change the STARTED flag to FALSE
      started = -9223372036854775808;

      //
      return;
      break; // -- END POS_END

    default:
      return;
      break;
    }

    distance += R2C_get_distance(MIN_NUM_DIST_READS);
    if (distance > MIN_VALUE_FOR_BLOCK)
      if (!is_erratic(false))
      {
        R2M_release_all();
        delay(DELAY_FOR_CHANGE_DIRECTION);
        return;
      }
  }
}