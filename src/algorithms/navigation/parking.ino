#include "ultrasonic.h"
#include <Arduino.h>
#include "bluetooth.h"
#include "motor.h"

#define trig_pin_1 A0
#define echo_pin_1 A1

#define trig_pin_2 A2
#define echo_pin_2 A3

#define trig_pin_3 A4
#define echo_pin_3 A5

#define min_side_distance 20
#define min_front_distance 15
#define max_reading_distance 200

Ultrasonic right_sensor_1(trig_pin_1, echo_pin_1);
Ultrasonic right_sensor_2(trig_pin_2, echo_pin_2);
Ultrasonic front_sensor(trig_pin_3, echo_pin_3);
Bluetooth bluetooth_module(9600);


bool has_turned = false;
bool has_parked = false;

unsigned long turning_speed = 125;
unsigned long turning_time = 800;
unsigned long moving_time = 200;
unsigned long moving_speed = 125;

bool stop = true;
bool reset = false;

void setup() {
  Serial.begin(9600);

  right_sensor_1.begin();
  right_sensor_2.begin();
  front_sensor.begin();

  bluetooth_module.send("Arduino Ready!\n");

  stop();
}

void loop() {

  if (bluetooth_module.available())
  {
      char command = bluetooth_module.read();
      switch (command)
      {
        case 'R':
            Serial.println("Reset pressed");
            reset = true;
            stop = false;
            break;
        case 'S':
            Serial.println("Stop pressed");
            reset = false;
            stop = true;
            break;
        case 'P':
            Serial.println("Parking pressed");
            reset = false;
            stop = false;
            break;
        default:
            Serial.println("Unknown command received");
            break;
      }
  }

  if(reset == true){
    has_turned = false;
    has_parked = false;
    stop();
    delay(1000);

    stop = false;
    reset = false;
  }



  float distance_1 = right_sensor_1.measureDistanceCm();
  Serial.print("Distance 1: ");
  Serial.println(distance_1);

  float distance_2 = right_sensor_2.measureDistanceCm();
  Serial.print("Distance 2: ");
  Serial.println(distance_2);

  float distance_3 = front_sensor.measureDistanceCm();
  Serial.print("Distance 3: ");
  Serial.println(distance_3);


  // avoiding noise
  if(distance_1 > max_reading_distance || distance_2 > max_reading_distance);  
    return;     // do nothing

  // Stoping motors state
  if(stop || (has_turned && has_parked)) {
    stop();
  }
      
  // Turning right state
  else if (distance_1 > min_side_distance && distance_2 > min_side_distance && !has_turned) {
    Serial.println("Both sensors are clear");
    digitalWrite(led, HIGH);

    stop();
    delay(500);
    turn_right_for_millis(speed, turning_time);

    has_turned = true; // return to default state
  }

  // Default state: move forward slowly
  else {
    move_forward_for_millis(speed , moving_time);
    if(distance_3 < min_front_distance)
      has_parked = true;  // move to stop state 
  }

  delay(500);
}





