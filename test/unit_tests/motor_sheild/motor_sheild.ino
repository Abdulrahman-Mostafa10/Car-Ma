// DC Motor Driver Test
// Author: Amir Kedis

#include <avr/io.h>
#include <util/delay.h>
#include "dc_motor.h"

int main(void) {
    // NOTE: we have 2 make 2 motors work on a timer and the other on the other timer.
    // Motors 1 & 2 share Timer2, Motors 3 & 4 share Timer0
    DCMotor motor1(1, MOTOR12_1KHZ);
    DCMotor motor2(2, MOTOR12_1KHZ);
    DCMotor motor3(3, MOTOR34_1KHZ);
    DCMotor motor4(4, MOTOR34_1KHZ);

    motor1.setSpeed(200);
    motor2.setSpeed(150);
    motor3.setSpeed(175);
    motor4.setSpeed(225);

    while(1) {
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        motor3.run(FORWARD);
        motor4.run(FORWARD);

        _delay_ms(2000);  // Run forward for 2 seconds

        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        motor3.run(BACKWARD);
        motor4.run(BACKWARD);
        _delay_ms(2000);  // Run backward for 2 seconds


        motor1.run(RELEASE);
        motor2.run(RELEASE);
        motor3.run(RELEASE);
        motor4.run(RELEASE);

        _delay_ms(1000);  // Pause for 1 second


        // Gradually increase motor1 speed
        for (uint8_t speed = 0; speed < 255; speed += 5) {
            motor1.setSpeed(speed);
            _delay_ms(50);
        }

        // Gradually decrease motor1 speed
        for (uint8_t speed = 255; speed > 0; speed -= 5) {
            motor1.setSpeed(speed);
            _delay_ms(50);
        }

        _delay_ms(1000);
    }

    return 0;
}
