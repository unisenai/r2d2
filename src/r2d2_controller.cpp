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

enum ITER_STATE
{
  ITER_DOWN,
  ITER_UP
};

// Local variables
volatile int8_t iter = POS_INITIAL;
volatile int8_t iter_stt = ITER_DOWN;
volatile bool started = false;

// Function to watch for the proximity sensor and reacts accordingly
void R2C_proximity_watcher()
{
  if (started)
  {
    // We need to STOP IMMEDIATELY because we either found a blocker or a clear path
    //    and in any case we need to quickly react changing the direction of our movement!
    R2M_release_all();

    // We check if we identified a blocker or a clear path in front of us
    if (iter_stt == ITER_DOWN)
    {
      // Ok, the previous state was DOWN meaning we found something blocking our way.
      // We move the iteration to the next obstacle
      iter++;

      /**
      We also identify the state of our interrupt signal if UP or DOWN
         and in this case we received an UP signal, meaning the signal level
         went from DOWN to UP, so we have a logical high level input in the PIN now.
      Important to note that the signal will keep HIGH while there is an obstacle in the
         defined range. The range is mechanically configured using the trimpots in the
         controller panel.
      */
      iter_stt = ITER_UP;

      // Checking if we have the WAL in front of us
      if (iter == POS_WALL)
      {
        // We got to the WAL, we need to do a 90 degrees turn to the LEFT and move forward
        R2C_loop_move(dir_right);
      }
      else if (iter == POS_END)
      {
        // We got to the end of the line, time to stop, meaning we DO NOTHING with the engines because we are already stopped!
        // Just change the STARTED flag to FALSE
        started = false;
      }
      else
      {
        // We just have another block, so moving to the RIGHT
        R2M_rotate_left(TIME_ANGLE_90);
        R2C_loop_move(dir_forward);
      }
    }
    else
    {
      /**
      We identified the signal went down, which means we don't have any obstacle in front of us.
      This means we can move forward!
      */
      iter_stt = ITER_DOWN;

      // Time to move FORWARD!
      R2C_loop_move(dir_forward);
    }
  }
}

// Function that starts and/or shuts down the AGV
void R2C_power_watcher()
{

  Serial.println("Starting!!!!");

  if (started)
    R2C_loop_move(dir_forward);
  else
    R2M_move_fw();

  started = !started;
}


static void R2C_loop_move(R2Direction direction)
{
  while(true)
  {
    switch(direction)
    {
      case dir_forward: R2M_move_fw(); break;
      case dir_backward: R2M_move_bw(); break;
      case dir_left: R2M_move_left(); break;
      case dir_right: R2M_move_right(); break;
    }

    delay(1000);
  }
}
