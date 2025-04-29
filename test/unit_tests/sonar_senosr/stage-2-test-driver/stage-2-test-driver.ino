// Author: Amir Kedis
// Sketch using driver

#include "ultrasonic.h"

// Params: trigPin, echoPin, (optional) timeout
Ultrasonic sonar(9, 10, 100000);

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
