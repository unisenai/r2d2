/**
 * @file r2d2_motor.h
 * @author Charly Batista (carlbsb@gmail.com)
 * @brief R2D2 is am autonomous guided vehicle (AGV) created as part of the Mechatronics undergraduate program in UniSenai Joinville/Brazil
 * @version 0.1
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __R2D2_MOTOR_H__
#define __R2D2_MOTOR_H__

//
#include <AFMotor.h>

// We firstly define the position of the DC motors
// We are using 4 DC motors in this project controlled by one
//  L293D H-Bridge Motor Driver shield
typedef enum
{
  // Motor in the RIGHT-FRONT side
  motor_rf = 1,

  // Motor in the RIGHT-BACK side
  motor_rb,

  // Motor in the LEFT-BACK side
  motor_lb,

  // Motor in the LEFT-FRONT side
  motor_lf
} R2Motor;

typedef enum
{
  // Motor in the RIGHT-FRONT side
  dir_forward,

  // Motor in the RIGHT-BACK side
  dir_backward,

  // Motor in the LEFT-BACK side
  dir_left,

  // Motor in the LEFT-FRONT side
  dir_right
} R2Direction;

// We define the maximum speed the motors will run
#define MAX_SPEED 200
#define ANGLE_SPEED 200

// Time to move to certain angle
#define TIME_ANGLE_90 850

// Moves the car forward
void R2M_move_fw();

// Moves the car backwards
void R2M_move_bw();

// Moves the car to the left
void R2M_move_left();

// Moves the car to the right
void R2M_move_right();

// Stops all the motors
void R2M_stop_all();

// 
void R2M_release_all();

// Sets the same speed for all motors
void R2M_set_speed_all(uint8_t speed);

// Sets speed for a specific motor
void R2M_set_speed(AF_DCMotor *motor, uint8_t speed);

// Rotates the car to the left up to the specified angle
void R2M_rotate_left(uint32_t time);

#endif // __R2D2_MOTOR_H__