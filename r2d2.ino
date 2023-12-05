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

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include "src/include/r2d2.h"
#include "src/include/r2d2_motor.h"

volatile int64_t started;
uint8_t position;

void setup()
{
  // Debug
  Serial.begin(115200);
  Serial.println("<Arduino is ready>");

  // Reset the motors
  R2M_set_speed_all(MAX_SPEED);
  R2M_release_all();

  // Configure the interrupt on PIN_POWER to start the LOOP
  attachInterrupt(digitalPinToInterrupt(PIN_POWER), R2C_power_watcher_2, RISING);

  // Initialize global variables
  // delay(500);
  // started = true;
  started = 0;
  position = POS_INITIAL;

  //
  //
  // print_logo();

  // LED definitions
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_GREEN, LOW);
}

void loop()
{
  if (started < 1)
  {
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
    
    delay(100);
    return;
  }
  else
  {
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);

    Serial.print("Distance: ");
    Serial.println(R2C_get_distance(MIN_NUM_DIST_READS));
    
    if (R2C_get_distance(MIN_NUM_DIST_READS) < MIN_VALUE_FOR_BLOCK)
    {
      R2C_controller(0);
      return;
    }

    R2M_move_fw();
    delay(10);
  }
}
