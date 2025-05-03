// NOTE: IMPORTANT DISCLAIMER
// The usage of the Arduino Shield V1 (Deprecated Version) 
// is too arcane and undocumented So I had to work with 
// there source code to know how to deal with it.
// Author: Amir Kedis - Ada Fruit Library firmware authors

#include "dc_motor.h"

// Motor control pin bits in shift register
// These are the bit positions in the shift register that control
// the direction pins for each motor
#define MOTOR1_A 2  // Motor 1 positive terminal control
#define MOTOR1_B 3  // Motor 1 negative terminal control
#define MOTOR2_A 1  // Motor 2 positive terminal control
#define MOTOR2_B 4  // Motor 2 negative terminal control
#define MOTOR3_A 5  // Motor 3 positive terminal control
#define MOTOR3_B 7  // Motor 3 negative terminal control
#define MOTOR4_A 0  // Motor 4 positive terminal control
#define MOTOR4_B 6  // Motor 4 negative terminal control

// Static member initialization
uint8_t MotorController::latch_state = 0;
bool MotorController::initialized = false;

/**
 * Enable the motor controller
 * Sets up the control pins as outputs and initializes the shift register
 */
void MotorController::enable() {
    if (initialized) return;


    // MOTORLATCH - D12 - PB4
    DDRB |= (1 << PB4);

    // MOTORDATA - D8 - PB0
    DDRB |= (1 << PB0);

    // MOTORCLK - D4 - PD4
    DDRD |= (1 << PD4);

    // MOTORENABLE - D7 - PD7
    DDRD |= (1 << PD7);

    // Reset latch state and send to shift register
    latch_state = 0;
    latch_tx();

    // Enable the chip outputs by setting MOTORENABLE LOW
    PORTD &= ~(1 << PD7);

    initialized = true;
}

/**
 * Transmit the latch state to the shift register
 * This controls the direction pins for all motors
 */
void MotorController::latch_tx() {
    // Pull the latch pin LOW
    PORTB &= ~(1 << PB4);  // MOTORLATCH LOW

    // Start with data line LOW
    PORTB &= ~(1 << PB0);  // MOTORDATA LOW

    // Shift out all 8 bits
    for (uint8_t i = 0; i < 8; i++) {
        PORTD &= ~(1 << PD4);  // MOTORCLK LOW

        if (latch_state & (1 << (7-i))) {
            PORTB |= (1 << PB0);  // MOTORDATA HIGH
        } else {
            PORTB &= ~(1 << PB0); // MOTORDATA LOW
        }

        PORTD |= (1 << PD4);  // MOTORCLK HIGH
    }

    PORTB |= (1 << PB4);  // MOTORLATCH HIGH
}

/**
 * Set a specific pin bit in the shift register
 */
void MotorController::setPin(uint8_t pin, bool state) {
    if (state) {
        latch_state |= (1 << pin);
    } else {
        latch_state &= ~(1 << pin);
    }
}

DCMotor::DCMotor(uint8_t num, uint8_t freq) {
    MotorController::enable();

    motorNum = num;


    switch (motorNum) {
        case 1:
            in1Pin = MOTOR1_A;
            in2Pin = MOTOR1_B;
            pwmPin = 11;  // OC2A - Timer2 Channel A - PB3
            break;

        case 2:
            in1Pin = MOTOR2_A;
            in2Pin = MOTOR2_B;
            pwmPin = 3;   // OC2B - Timer2 Channel B - PD3
            break;

        case 3:
            in1Pin = MOTOR3_A;
            in2Pin = MOTOR3_B;
            pwmPin = 6;   // OC0A - Timer0 Channel A - PD6
            break;

        case 4:
            in1Pin = MOTOR4_A;
            in2Pin = MOTOR4_B;
            pwmPin = 5;   // OC0B - Timer0 Channel B - PD5
            break;

        default:

            return;
    }

    // Initialize motor direction pins to both LOW (motor stopped)
    MotorController::setPin(in1Pin, false);
    MotorController::setPin(in2Pin, false);
    MotorController::latch_tx();


    initPWM(freq);
}

void DCMotor::initPWM(uint8_t freq) {
    switch (motorNum) {
        case 1:
            // Motor 1 uses Timer2 Channel A (OC2A) on pin 11 (PB3)
            // Set pin 11 (PB3) as output
            DDRB |= (1 << PB3);

            // Fast PWM mode, non-inverting output on OC2A
            TCCR2A |= (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);

            // Set prescaler according to frequency
            TCCR2B = freq & 0x07;

            // Start with 0 duty cycle
            OCR2A = 0;
            break;

        case 2:
            // Motor 2 uses Timer2 Channel B (OC2B) on pin 3 (PD3)
            // Set pin 3 (PD3) as output
            DDRD |= (1 << PD3);

            // Fast PWM mode, non-inverting output on OC2B
            TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);

            // Set prescaler according to frequency
            TCCR2B = freq & 0x07;

            // Start with 0 duty cycle
            OCR2B = 0;
            break;

        case 3:
            // Motor 3 uses Timer0 Channel A (OC0A) on pin 6 (PD6)
            // NOTE: Timer0 is also used for millis()/delay() functions

            // Set pin 6 (PD6) as output
            DDRD |= (1 << PD6);

            // Fast PWM mode, non-inverting output on OC0A
            TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);

            // Set prescaler carefully to not completely disrupt Arduino timing functions
            TCCR0B = (TCCR0B & 0xF8) | (freq & 0x07);

            // Start with 0 duty cycle
            OCR0A = 0;
            break;

        case 4:
            // Motor 4 uses Timer0 Channel B (OC0B) on pin 5 (PD5)
            // NOTE: Timer0 is also used for millis()/delay() functions

            // Set pin 5 (PD5) as output
            DDRD |= (1 << PD5);

            // Fast PWM mode, non-inverting output on OC0B
            TCCR0A |= (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);

            // Set prescaler carefully to not completely disrupt Arduino timing functions
            TCCR0B = (TCCR0B & 0xF8) | (freq & 0x07);

            // Start with 0 duty cycle
            OCR0B = 0;
            break;
        default:
          break;
    }
}

/**
 * Set the PWM value (duty cycle) for the motor
 */
void DCMotor::setPWM(uint8_t speed) {
    switch (motorNum) {
        case 1:
            // Motor 1 uses OCR2A register
            OCR2A = speed;
            break;

        case 2:
            // Motor 2 uses OCR2B register
            OCR2B = speed;
            break;

        case 3:
            // Motor 3 uses OCR0A register
            OCR0A = speed;
            break;

        case 4:
            // Motor 4 uses OCR0B register
            OCR0B = speed;
            break;
    }
}

/**
 * Run the motor in a specific direction
 * @param cmd Direction: FORWARD, BACKWARD, or RELEASE
 */
void DCMotor::run(uint8_t cmd) {
    switch (cmd) {
        case FORWARD:
            // Set in1 HIGH, in2 LOW
            MotorController::setPin(in1Pin, true);
            MotorController::setPin(in2Pin, false);
            MotorController::latch_tx();
            break;
            
        case BACKWARD:
            // Set in1 LOW, in2 HIGH
            MotorController::setPin(in1Pin, false);
            MotorController::setPin(in2Pin, true);
            MotorController::latch_tx();
            break;
            
        case RELEASE:
            // Set in1 LOW, in2 LOW
            MotorController::setPin(in1Pin, false);
            MotorController::setPin(in2Pin, false);
            MotorController::latch_tx();
            break;
    }
}

void DCMotor::setSpeed(uint8_t speed) {
    setPWM(speed);
}
