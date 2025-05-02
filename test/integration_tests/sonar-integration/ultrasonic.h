// Author: Amir Kedis
// This file Meant to drive the sonar sensor
// now with low level register control


#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>


class Ultrasonic {
private:
  uint8_t trig_port;
  uint8_t trig_pin;
  uint8_t echo_port;
  uint8_t echo_pin;


public:
  Ultrasonic(uint8_t trig_port, uint8_t trig_pin,uint8_t echo_port, uint8_t echo_pin);

  void begin();
  float measureDistanceCm();
  unsigned long getDuration(); 
};

#endif
