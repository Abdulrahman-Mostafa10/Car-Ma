#include "ultrasonic.h"
#include "bluetooth.h"
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>


#define min_side_distance 30.0
#define min_front_distance 15.0
#define max_reading_distance 200.0

Ultrasonic right_sensor_1('C', PC0, 'C', PC1);
Ultrasonic right_sensor_2('C', PC2, 'C', PC3);
Ultrasonic front_sensor('C', PC4, 'C', PC5);

Bluetooth bluetooth_module(9600);


bool has_turned = false;
bool has_parked = false;

unsigned long turning_speed = 145;
unsigned long turning_time = 850;
unsigned long moving_time = 75;
unsigned long moving_speed = 135;
unsigned long after_turn_time = 100;

bool stop_flag = true;
bool reset = false;

int main() {

  // setup
  // Serial.begin(9600);

  right_sensor_1.begin();
  right_sensor_2.begin();
  front_sensor.begin();

  bluetooth_module.send("Arduino Ready!\n");

  stop();


  // loop
  while (1) {
    if (bluetooth_module.available())
    {

        char command = Serial.read();
        switch (command)
        {
          case '1':
              // Serial.println("Reset pressed");
              reset = true;
              stop_flag = false;
              break;
          case '2':
              // Serial.println("stop pressed");
              reset = false;
              stop_flag = true;
              break;
          case '3':
              // Serial.println("Parking pressed");
              reset = false;
              stop_flag = false;
              break;
          default:
              // Serial.println("Unknown command received");
              break;
        }
    }

    if(reset == true){
      has_turned = false;
      has_parked = false;
      stop();
      // Serial.println("in reset mode");
      _delay_ms(1000);

      stop_flag = true;
      reset = false;
    }


    float distance_1 = right_sensor_1.measureDistanceCm();
    // Serial.print("Distance 1: ");
    Serial.println(distance_1);

    float distance_2 = right_sensor_2.measureDistanceCm();
    // Serial.print("Distance 2: ");
    Serial.println(distance_2);

    float distance_3 = front_sensor.measureDistanceCm();
    // Serial.print("Distance 3: ");
    // Serial.println(distance_3);


    // avoiding noise
    if(distance_1 > max_reading_distance || distance_2 > max_reading_distance) {
      // Serial.println("Skipping reading outlier");
      // Serial.println(max_reading_distance); 
      return;     // do nothing
    }

    // Stoping motors state
    if(stop_flag || (has_turned && has_parked)) {
      stop();
    }
        
    // Turning right state
    else if (!has_turned && distance_1 > min_side_distance && distance_2 > min_side_distance) {
      // Serial.println("Both sensors are clear");
      stop();
      _delay_ms(500);
      turn_right_for_millis(turning_speed, turning_time);

      has_turned = true; // return to default state
    }

    else if(has_turned){
      move_forward_for_millis(moving_speed , after_turn_time);
      if(distance_3 < min_front_distance){
        has_parked = true;                  // move to stop state
        // Serial.println("Done Parking!!");
      }

    }

    // Default state: move forward slowly
    else {
      move_forward_for_millis(moving_speed , moving_time);
    }

    _delay_ms(500);

  }

  return 0;
}
