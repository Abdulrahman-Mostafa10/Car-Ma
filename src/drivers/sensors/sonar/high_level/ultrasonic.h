// Author: Amir Kedis
// This file Meant to drive the sonar sensor
// with easy to understand interface
// it uses Arduino functions for now.

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class Ultrasonic
{
private:
  uint8_t trig_pin;
  uint8_t echo_pin;

  float timeout; // in microseconds
                 // NOTE: if you set it too low you might not get the full range

public:
  Ultrasonic(uint8_t trig_pin, uint8_t echo_pin, float timeout = 10000);

  void begin();
  float measureDistanceCm();
  unsigned long getDuration();
};

#endif
