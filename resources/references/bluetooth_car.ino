// Arduino Bluetooth Controlled Car
// Before uploading the code you have to install the necessary library
// *Note* - Disconnect the Bluetooth Module before hiting the upload button otherwise you'll get compilation error message.

// AFMotor Library https://learn.adafruit.com/adafruit-motor-shield/library-install
// After downloading the library open Arduino IDE >> go to sketch >> Include Libray >> ADD. ZIP Libray >> Select the downloaded
// ZIP File >> Open it >> Done

// *Update*: If you are using Arduino IDE 2.0 or later, you can install the library from the Library Manager.

// Now You Can Upload the Code without any problem but make sure the bluetooth module isn't connected with Arduino while uploading code

/*
you can see a working demo for this code here
https://drive.google.com/file/d/1XCNEgaYXiMa_1CRb4NLZ6zVvaw8m1zID/view?usp=drivesdk
*/

#include <AFMotor.h>

// initial motors pin
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

char command;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available() > 0)
    {
        command = Serial.read();
        /*the command coming form the bluetooth module/ mobile app
        is at the end just a character
        print the character that is emitted by each button press to the serial monitor
        to see what is being sent by the mobile app
        */
        Stop(); // initialize with motors stoped 
        // (and also to stop the motors when no command is received in the next loop iteration)
        
        switch (command)
        {
        case 'F':
            forward();
            break;
        case 'B':
            back();
            break;
        case 'L':
            left();
            break;
        case 'R':
            right();
            break;
        }
    }
}

void forward()
{
    motor1.setSpeed(255); 
    motor1.run(FORWARD);  
    motor2.setSpeed(255); 
    motor2.run(FORWARD);  
    motor3.setSpeed(255); 
    motor3.run(FORWARD);  
    motor4.setSpeed(255); 
    motor4.run(FORWARD);  
}

void back()
{
    motor1.setSpeed(255); 
    motor1.run(BACKWARD); 
    motor2.setSpeed(255); 
    motor2.run(BACKWARD); 
    motor3.setSpeed(255); 
    motor3.run(BACKWARD); 
    motor4.setSpeed(255); 
    motor4.run(BACKWARD); 
}

void left()
{
    motor1.setSpeed(255); 
    motor1.run(BACKWARD); 
    motor2.setSpeed(255); 
    motor2.run(BACKWARD); 
    motor3.setSpeed(255); 
    motor3.run(FORWARD);  
    motor4.setSpeed(255); 
    motor4.run(FORWARD);  
}

void right()
{
    motor1.setSpeed(255); 
    motor1.run(FORWARD);  
    motor2.setSpeed(255); 
    motor2.run(FORWARD);  
    motor3.setSpeed(255); 
    motor3.run(BACKWARD); 
    motor4.setSpeed(255); 
    motor4.run(BACKWARD); 
}

void Stop()
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