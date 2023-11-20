/**
 * @file r2d2_motor.cpp
 * @author Charly Batista (carlbsb@gmail.com)
 * @brief R2D2 is am autonomous guided vehicle (AGV) created as part of the Mechatronics undergraduate program in UniSenai Joinville/Brazil
 * @version 0.1
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "include/r2d2_motor.h"

// We create the Motor objects
AF_DCMotor m_lf(motor_lf);
AF_DCMotor m_lb(motor_lb);
AF_DCMotor m_rf(motor_rf);
AF_DCMotor m_rb(motor_rb);

// Moves the car forward
void R2M_move_fw()
{
  m_lf.run(BACKWARD);
  m_lb.run(BACKWARD);
  m_rf.run(BACKWARD);
  m_rb.run(BACKWARD);
}

// Moves the car backwards
void R2M_move_bw()
{
  m_lf.run(FORWARD);
  m_lb.run(FORWARD);
  m_rf.run(FORWARD);
  m_rb.run(FORWARD);
}

// Moves the car to the left
void R2M_move_left()
{
  m_lf.run(FORWARD);
  m_lb.run(BACKWARD);
  m_rf.run(FORWARD);
  m_rb.run(BACKWARD);
}

// Moves the car to the right
void R2M_move_right()
{
  m_lf.run(BACKWARD);
  m_lb.run(FORWARD);
  m_rf.run(BACKWARD);
  m_rb.run(FORWARD);
}

// Stops all the motors
void R2M_stop_all()
{
}

// Sets the same speed for all motors
void R2M_set_speed_all(uint8_t speed)
{
  R2M_set_speed(&m_lf, speed);
  R2M_set_speed(&m_lb, speed);
  R2M_set_speed(&m_rf, speed);
  R2M_set_speed(&m_rb, speed);
}

// Sets speed for a specific motor
void R2M_set_speed(AF_DCMotor *motor, uint8_t speed)
{
  (*motor).setSpeed(speed);
}

void R2M_release_all()
{
  m_lf.run(RELEASE);
  m_lb.run(RELEASE);
  m_rf.run(RELEASE);
  m_rb.run(RELEASE);
}