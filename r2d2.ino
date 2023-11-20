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

// DEBUG
char receivedChar;
boolean newData = false;
volatile bool started;

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
}

// Boolean to represent toggle state for the POWER button
volatile bool togglestate = false;

ISR(PCINT1_vect)
{
  if(started)
    return;

  togglestate = !togglestate;

  Serial.println("Button Pressed!!");

  // Only acts when release the button
  if (!togglestate)
  {
    R2C_power_watcher();
  }
}

void loop()
{
  if(started)
  {
    R2M_move_fw();
  }

  delay(100);
}

void loops()
{
  recvOneChar();
  if (newData == true)
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

    case '5':
      R2M_rotate_left(TIME_ANGLE_90);
      break;
    default:
      Serial.print("This just in ... ");
      Serial.println(receivedChar);
    }

    if (receivedChar != '5')
    {
      delay(300);
      R2M_release_all();
    }
    newData = false;
  }
}

void recvOneChar()
{
  if (Serial.available() > 0)
  {
    receivedChar = Serial.read();
    newData = true;
  }
}