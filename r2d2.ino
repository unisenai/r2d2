/**
 * @file r2d2.c
 * @author Charly Batista (carlbsb@gmail.com)
 * @brief R2D2 is am autonomous guided vehicle (AGV) created as part of the Mechatronics undergraduate program in UniSenai Joinville/Brazil
 * @version 0.1
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <PinChangeInt.h>

#include "src/include/r2d2.h"
#include "src/include/r2d2_motor.h"
#include "src/include/r2d2_map1.h"

// DEBUG
char receivedChar;
boolean newData = false;

// Local variables
#define PROXIMITY_PIN 2
#define POWER_PIN A0
volatile int8_t iter = 0;
volatile bool started = false;

void setup()
{
  R2M_set_speed_all(MAX_SPEED);
  R2M_release_all();

  // Debug
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");

  // Configure the interrupt on PROXIMITY_PIN
  // This interrupt will be triggered by the proximity sensor HC-SR04
  // We are waiting for any change, either when raising or falling signal on the pin  
  attachInterrupt(digitalPinToInterrupt(PROXIMITY_PIN), proximity_watcher, CHANGE);

  // Startup interrupt function
  attachPinChangeInterrupt(POWER_PIN, power, CHANGE); // interrupt connected to pin 11
}

void loop()
{
  recvOneChar();
  if(newData == true)
  {
    switch (receivedChar)
    {
      case '1':
        R2M_move_fw();
        break;
      case '2':
        R2M_move_bw();
        break;
      case '3':
        R2M_move_left();
        break;
      case '4':
        R2M_move_right();
        break;

      default:
        Serial.print("This just in ... ");
        Serial.println(receivedChar);
    }

    delay(300);
    R2M_release_all();
    newData = false;
  }
  
}

void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}