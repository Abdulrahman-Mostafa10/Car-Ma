// Author: Amir Kedis
// Using low level driver

#include "ultrasonic.h"

// Params: trig port, trig pin, echo port, echo pin
Ultrasonic sonar('B', 1, 'B', 2);

void setup() {
  Serial.begin(9600);

  sonar.begin();

  Serial.println("Ultrasonic Sensor Module Test");
  Serial.println("----------------------------");
}

void loop() {
  float distanceCm = sonar.measureDistanceCm();
  
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  Serial.flush();
  
  delay(500);
}
