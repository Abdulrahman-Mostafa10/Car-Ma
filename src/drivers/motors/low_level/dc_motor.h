// NOTE: IMPORTANT DISCLAIMER
// The usage of the Arduino Shield V1 (Deprecated Version) 
// is too arcane and undocumented So I had to work with 
// there source code to know how to deal with it.
// Author: Amir Kedis - Ada Fruit Library firmware authors

#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include <avr/io.h>

/**
 * Pin definitions for Arduino Uno:
 * MOTORLATCH  - D12 - PB4 - Output to latch shift register
 * MOTORDATA   - D8  - PB0 - Serial data output to shift register
 * MOTORCLK    - D4  - PD4 - Clock signal for shift register
 * MOTORENABLE - D7  - PD7 - Enable pin for the motor driver chip
 * 
 * PWM pins for each motor:
 * Motor 1 - D11 - PB3 - OC2A - Timer2 Channel A
 * Motor 2 - D3  - PD3 - OC2B - Timer2 Channel B
 * Motor 3 - D6  - PD6 - OC0A - Timer0 Channel A
 * Motor 4 - D5  - PD5 - OC0B - Timer0 Channel B
 * 
 * NOTE: Timer0 is typically used for millis()/delay() functions
 * Using motors 3 & 4 may affect timing functions
 */

// Pin definitions
#define MOTORLATCH  12    // D12 - pin PB4
#define MOTORDATA   8     // D8  - pin PB0
#define MOTORCLK    4     // D4  - pin PD4
#define MOTORENABLE 7     // D7  - pin PD7

// Direction constants
enum Direction {
    FORWARD = 1,
    BACKWARD = 2,
    RELEASE = 4
};

// PWM frequencies for motors 1 & 2 (Timer2)
enum PWMFrequency {
    MOTOR12_64KHZ = 0x01,  // No prescaling
    MOTOR12_8KHZ = 0x02,   // 1/8 prescaler
    MOTOR12_2KHZ = 0x03,   // 1/32 prescaler
    MOTOR12_1KHZ = 0x04,   // 1/64 prescaler
    MOTOR12_500HZ = 0x05,  // 1/128 prescaler
    MOTOR12_250HZ = 0x06,  // 1/256 prescaler
    MOTOR12_150HZ = 0x07,  // 1/1024 prescaler
    
    // For motors 3 & 4 (Timer0)
    MOTOR34_64KHZ = 0x01,  // No prescaling
    MOTOR34_8KHZ = 0x02,   // 1/8 prescaler 
    MOTOR34_1KHZ = 0x03,   // 1/64 prescaler
    MOTOR34_500HZ = 0x04,  // 1/256 prescaler
    MOTOR34_150HZ = 0x05   // 1/1024 prescaler
};

/**
 * It handles the shift register that controls all motor directions.
 */
class MotorController {
private:
    static uint8_t latch_state;
    static bool initialized;

public:
    static void enable();
    static void latch_tx();
    static void setPin(uint8_t pin, bool state);

    friend class DCMotor;
};

class DCMotor {
private:
    uint8_t motorNum;
    uint8_t pwmPin;
    uint8_t in1Pin;
    uint8_t in2Pin;
    
    /**
     * Initialize the PWM for a specific motor
     * 
     * Motor 1: Timer2A, Pin 11 (PB3)
     * Motor 2: Timer2B, Pin 3 (PD3)
     * Motor 3: Timer0A, Pin 6 (PD6)
     * Motor 4: Timer0B, Pin 5 (PD5)
     */
    void initPWM(uint8_t freq);
    
    void setPWM(uint8_t speed);
    
public:
    DCMotor(uint8_t num, uint8_t freq);
    
    void run(uint8_t cmd);
    
    void setSpeed(uint8_t speed);
};

#endif // DC_MOTOR_H
