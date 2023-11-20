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
#define PROXIMITY_INTER_PIN 2
#define POWER_INTER_PIN A0
#define DISTANCE_PIN A5
#define MIN_VALUE_FOR_BLOCK 800
#define MIN_TIME_BETWEEN_READS 1000

// We define the maximum speed the motors will run
#define MAX_SPEED 200
#define ANGLE_SPEED 180

// Time to move to certain angle
#define TIME_ANGLE_90 850

#endif  // __R2D2_CONST_H__