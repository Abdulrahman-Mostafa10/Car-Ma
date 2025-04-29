// Author: Amir Kedis
// This the implementation now uses low level reg control
// NOTE: removed timeout cause it caused more false 0.

#include "ultrasonic.h"
#include <avr/io.h>
#include <util/delay.h>

// CONSTANT
#define SONAR_SPEED 0.034 // cm/us

// NOTE: this is the clock period for the timer
#define CLOCK_PERIOD_PRESCALER 4.0 // us (1 / 18MHz) * 10^6 * 64 

// REG MACROS
#define GET_PORT_REG(port) (port == 'B' ? &PORTB : \
                           port == 'C' ? &PORTC : \
                           port == 'D' ? &PORTD : 0)
#define GET_PIN_REG(port) (port == 'B' ? &PINB : \
                           port == 'C' ? &PINC : \
                           port == 'D' ? &PIND : 0)
#define GET_DDR_REG(port) (port == 'B' ? &DDRB : \
                           port == 'C' ? &DDRC : \
                           port == 'D' ? &DDRD : 0)



Ultrasonic::Ultrasonic(uint8_t trig_port, uint8_t trig_pin,uint8_t echo_port, uint8_t echo_pin) {
  this->trig_port = trig_port;
  this->trig_pin = trig_pin;
  this->echo_port = echo_port;
  this->echo_pin = echo_pin;
}

void Ultrasonic::begin() {
  // Set trig pin as output
  volatile uint8_t *trig_ddr = GET_DDR_REG(trig_port);
  *trig_ddr |= (1 << trig_pin);

  // Set echo pin as input
  volatile uint8_t *echo_ddr = GET_DDR_REG(echo_port);
  *echo_ddr &= ~(1 << echo_pin);

  // Set trig pin to low
  volatile uint8_t *trig_port_reg = GET_PORT_REG(trig_port);
  *trig_port_reg &= ~(1 << trig_pin);

  // NOTE: Important will use TIMER 1
  TCCR1A = 0;           // NOTE: this means normal timer operations
  TCCR1B = (1 << CS11) | (1 << CS10); // Prescaler 64
}

unsigned long Ultrasonic::getDuration() {
  volatile uint8_t *trig_port_reg = GET_PORT_REG(trig_port);
  volatile uint8_t *echo_pin_reg = GET_PIN_REG(echo_port);

  // Clear TRIG
  *trig_port_reg &= ~(1 << trig_pin);
  _delay_us(2);

  // give the sensor the pulse
  *trig_port_reg |= (1 << trig_pin);
  _delay_us(10);
  *trig_port_reg &= ~(1 << trig_pin);

  // Read the ECHO pin
  unsigned long duration_ticks = 0;
  uint16_t start_time_ticks;
  uint16_t end_time_ticks;

  TCNT1 = 0;           // Clear the timer

  // NOTE: This is waiting for ECHO pin to go high (start of the pulse
  while (!(*echo_pin_reg & (1 << echo_pin))) {
  }

  start_time_ticks = TCNT1; // NOTE: now this is the start time of the pulse

  while ((*echo_pin_reg & (1 << echo_pin))) {
  }

  end_time_ticks = TCNT1; // now this is the duration

  // duration in timer ticks, handling overflow
  if (end_time_ticks >= start_time_ticks) {
    duration_ticks = end_time_ticks - start_time_ticks;
  } else {
    duration_ticks = (65535 - start_time_ticks) + end_time_ticks + 1;
  }

  unsigned long duration_us = (unsigned long)((float)duration_ticks * CLOCK_PERIOD_PRESCALER);

  // NOTE: from arduino code they say there are ticks wated on the 3 loops
  // equal to 16 ===> :) its seems to be better this way
  if (duration_us > 16) {
      return duration_us - 16;
  } else {
      return 0;
  }
}

float Ultrasonic::measureDistanceCm() {
  unsigned long duration = getDuration();
  return (float)duration * SONAR_SPEED / 2.0;
}

