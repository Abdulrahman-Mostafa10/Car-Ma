// Author: Amir Kedis
// This the implementation of the basic (easy) driver.

#include "ultrasonic.h"

#define SONAR_SPEED 0.034 // cm/us

Ultrasonic::Ultrasonic(uint8_t trig_pin, uint8_t echo_pin, float timeout)
{
  this->trig_pin = trig_pin;
  this->echo_pin = echo_pin;
  this->timeout = timeout;
}

void Ultrasonic::begin()
{
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  digitalWrite(trig_pin, LOW); // ensure no weird pulse at the begining
}

unsigned long Ultrasonic::getDuration()
{
  // Clear TRIG
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  // give the sensor the pulse
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  // Read the ECHO pin
  return pulseIn(echo_pin, HIGH);
}

float Ultrasonic::measureDistanceCm()
{
  unsigned long duration = getDuration();
  return duration * SONAR_SPEED / 2;
}
