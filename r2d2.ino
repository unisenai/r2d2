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

volatile bool started;
volatile int8_t position;
volatile bool found_block;

void setups()
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

  //
  //
  // print_logo();

  // LED definitions
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);

  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(LED_GREEN_PIN, LOW);
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
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, HIGH);
    R2C_power_watcher();
  }
}

#define trigPin A0
#define echoPin A1

unsigned long int duration;
unsigned int distance;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
  Serial.begin(115200);
}

void loop()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor

  Serial.print(">> Duration: ");
  Serial.println(duration);

  Serial.print("Distance: ");
  Serial.println(distance);
  delay(200);
}

void loops()
{
  if (started)
  {
    if (found_block)
    {
      R2C_read_sensor();
    }
    else
    {
      R2M_move_fw();
    }
  }

  delay(100);
}