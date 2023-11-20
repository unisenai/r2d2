/**
 * @file r2d2.h
 * @author Charly Batista (carlbsb@gmail.com)
 * @brief R2D2 is am autonomous guided vehicle (AGV) created as part of the Mechatronics undergraduate program in UniSenai Joinville/Brazil
 * @version 0.1
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __R2D2_H__
#define __R2D2_H__

// PIN definitions
#define PROXIMITY_INTER_PIN 2
#define POWER_INTER_PIN A0
#define DISTANCE_PIN A5
#define MIN_VALUE_FOR_BLOCK 800
#define MIN_TIME_BETWEEN_READS (1 * 1000 * 1000) // From micros to millis

extern volatile bool started;

// Function to watch for the proximity sensor and reacts accordingly
void R2C_proximity_watcher();

// Function that starts and/or shuts down the AGV
void R2C_power_watcher();

#endif  // __R2D2_H__