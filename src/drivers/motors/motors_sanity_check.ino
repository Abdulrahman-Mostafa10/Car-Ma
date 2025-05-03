#include <AFMotor.h>

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR12_1KHZ);
AF_DCMotor motor4(4, MOTOR12_1KHZ);

int speed;
char orientation;

void setup()
{
    Serial.begin(9600);
    stop();
}

void loop()
{
    if (Serial.available())
    {

        orientation = Serial.read();
        speed = Serial.parseInt();
        unsigned long millis = 2000;

        switch (orientation)
        {
        case 'f':
            move_forward_for_millis(speed, millis);
            break;

        case 'b':
            move_backward_for_millis(speed, millis);
            break;

        case 'l':
            turn_left_for_millis(speed, millis);
            break;

        case 'r':
            turn_right_for_millis(speed, millis);
            break;

        default:
            stop();
            break;
        }
    }
}

void move_forward_for_millis(int speed, unsigned long milliseconds)
{
    // the directions are deduced by trial and error
    motor1.setSpeed(speed);
    motor1.run(BACKWARD);

    motor2.setSpeed(speed);
    motor2.run(BACKWARD);

    motor3.setSpeed(speed);
    motor3.run(FORWARD);

    motor4.setSpeed(speed);
    motor4.run(BACKWARD);

    delay(milliseconds);

    stop();
}

void move_backward_for_millis(int speed, unsigned long milliseconds)
{
    // the directions are deduced by trial and error
    motor1.setSpeed(speed);
    motor1.run(FORWARD);

    motor2.setSpeed(speed);
    motor2.run(FORWARD);

    motor3.setSpeed(speed);
    motor3.run(BACKWARD);

    motor4.setSpeed(speed);
    motor4.run(FORWARD);

    delay(milliseconds);

    stop();
}

void turn_left_for_millis(int speed, unsigned long milliseconds)
{
    // the directions are deduced by trial and error
    motor1.setSpeed(speed);
    motor1.run(FORWARD);

    motor2.setSpeed(speed);
    motor2.run(FORWARD);

    motor3.setSpeed(speed);
    motor3.run(FORWARD);

    motor4.setSpeed(speed);
    motor4.run(BACKWARD);

    delay(milliseconds);

    stop();
}

void turn_right_for_millis(int speed, unsigned long milliseconds)
{
    // the directions are deduced by trial and error
    motor1.setSpeed(speed);
    motor1.run(BACKWARD);

    motor2.setSpeed(speed);
    motor2.run(BACKWARD);

    motor3.setSpeed(speed);
    motor3.run(BACKWARD);

    motor4.setSpeed(speed);
    motor4.run(FORWARD);

    delay(milliseconds);

    stop();
}

void stop()
{
    motor1.setSpeed(0);
    motor1.run(RELEASE);
    motor2.setSpeed(0);
    motor2.run(RELEASE);
    motor3.setSpeed(0);
    motor3.run(RELEASE);
    motor4.setSpeed(0);
    motor4.run(RELEASE);
}