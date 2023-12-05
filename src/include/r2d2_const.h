/**
 * @file r2d2_const.h
 * @author Charly Batista (carlbsb@gmail.com)
 * @brief R2D2 is am autonomous guided vehicle (AGV) created as part of the Mechatronics undergraduate program in UniSenai Joinville/Brazil
 * @version 0.1
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __R2D2_CONST_H__
#define __R2D2_CONST_H__

// PIN definitions
#define PIN_POWER 2
#define PIN_TRIG A0
#define PIN_ECHO A1
#define PIN_LED_GREEN A2
#define PIN_LED_RED A3


//
#define MIN_VALUE_FOR_BLOCK 30
#define MIN_TIME_BETWEEN_READS 1000

#define DELAY_TO_AVOID_COLLISION 500
#define DELAY_FOR_CHECK 0
#define DELAY_FOR_CHANGE_DIRECTION 300

// We define the maximum speed the motors will run
#define MAX_SPEED 200
#define ANGLE_SPEED 180

// Time to move to certain angle
#define TIME_ANGLE_90 850

//
#define MIN_NUM_DIST_READS 5
#define NUM_ERRATIC_CHECKS 3

#endif  // __R2D2_CONST_H__