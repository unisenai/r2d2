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

#include "src/include/r2d2.h"
#include "src/include/r2d2_motor.h"

volatile bool started;
volatile int8_t position;
volatile bool found_block;

void setup()
{
  // Debug
  Serial.begin(115200);
  Serial.println("<Arduino is ready>");

  // Reset the motors
  R2M_set_speed_all(MAX_SPEED);
  R2M_release_all();

  // Configure the interrupt on PROXIMITY_INTER_PIN
  // This interrupt will be triggered by the proximity sensor HC-SR04
  // We are waiting for any change, either when raising or falling signal on the pin
  attachInterrupt(digitalPinToInterrupt(PROXIMITY_INTER_PIN), R2C_proximity_watcher, RISING);

  // Startup interrupt function
  pinMode(POWER_INTER_PIN, INPUT_PULLUP);

  // Enable PCIE0 Bit0 = 1 (Port B)
  PCICR |= (1 << PCIE1);

  // Select PCINT8 Bit0 = 1 (Pin A0)
  PCMSK1 = (1 << PCINT8); // A0

  // Initialize global variables
  started = false;
  position = POS_INITIAL;
  found_block = false;
}

// Boolean to represent toggle state for the POWER button
volatile bool togglestate = false;

ISR(PCINT1_vect)
{
  if (started)
    return;

  togglestate = !togglestate;

  Serial.println("Button Pressed!!");

  // Only acts when release the button
  if (!togglestate)
  {
    // Disable this interrupt for now because we are getting too much noise from the DC motors
    //    and they are causing the interrupt to auto-trigger ?!?!?!
    PCMSK1 &= ~(1 << PCINT8);
    R2C_power_watcher();
  }
}

void loop()
{
  if (started)
  {
    if (found_block)
    {
      switch (position)
      {
      case POS_OBSTACLE_1:
      case POS_OBSTACLE_2:
      case POS_OBSTACLE_3:
        break;

      POS_WALL:
        break;

      POS_END:
        break;
      }
    }
    else
    {
      R2M_move_fw();
    }
  }

  delay(100);
}