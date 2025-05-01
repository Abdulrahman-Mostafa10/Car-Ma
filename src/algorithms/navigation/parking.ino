#include "ultrasonic.h"
#include "bluetooth.h"
#include <Arduino.h>

#define trig_pin_1 A0
#define echo_pin_1 A1

#define trig_pin_2 A2
#define echo_pin_2 A3

#define trig_pin_3 A4
#define echo_pin_3 A5

#define min_side_distance 25.0
#define min_front_distance 15.0
#define max_reading_distance 200.0

Ultrasonic right_sensor_1(trig_pin_1, echo_pin_1);
Ultrasonic right_sensor_2(trig_pin_2, echo_pin_2);
Ultrasonic front_sensor(trig_pin_3, echo_pin_3);
Bluetooth bluetooth_module(9600);


bool has_turned = false;
bool has_parked = false;

unsigned long turning_speed = 145;
unsigned long turning_time = 800;
unsigned long moving_time = 100;
unsigned long moving_speed = 135;
unsigned long after_turn_time = 100;

bool stop_flag = true;
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
      
      char command = Serial.read();
      switch (command)
      {
        case '1':
            Serial.println("Reset pressed");
            reset = true;
            stop_flag = false;
            break;
        case '2':
            Serial.println("stop pressed");
            reset = false;
            stop_flag = true;
            break;
        case '3':
            Serial.println("Parking pressed");
            reset = false;
            stop_flag = false;
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
    Serial.println("in reset mode");
    delay(1000);

    stop_flag = true;
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
  if(distance_1 > max_reading_distance || distance_2 > max_reading_distance) {
    Serial.println("Skipping reading outlier");
    Serial.println(max_reading_distance); 
    return;     // do nothing
  }

  // Stoping motors state
  if(stop_flag || (has_turned && has_parked)) {
    stop();
  }
      
  // Turning right state
  else if (!has_turned && distance_1 > min_side_distance && distance_2 > min_side_distance) {
    Serial.println("Both sensors are clear");
    stop();
    delay(500);
    turn_right_for_millis(turning_speed, turning_time);

    has_turned = true; // return to default state
  }

  else if(has_turned){
    move_forward_for_millis(moving_speed , after_turn_time);
    if(distance_3 < min_front_distance){
      has_parked = true;                  // move to stop state
      Serial.println("Done Parking!!");
    }

  }
  
  // Default state: move forward slowly
  else {
    move_forward_for_millis(moving_speed , moving_time);
  }

  delay(500);
}