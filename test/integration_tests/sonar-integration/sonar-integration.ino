// Author: Amir Kedis
// Using low level driver

#include "ultrasonic.h"

// Params: trig port, trig pin, echo port, echo pin
Ultrasonic sonar('C', PC0, 'C', PC1);
Ultrasonic sonar2('C', PC2, 'C', PC3);
Ultrasonic sonar3('C', PC4, 'C', PC5);

void setup() {
  Serial.begin(9600);

  sonar.begin();

  Serial.println("Ultrasonic Sensor Module Test");
  Serial.println("----------------------------");
}

void loop() {
  float distanceCm = sonar.measureDistanceCm();
  float distanceCm2 = sonar2.measureDistanceCm();
  float distanceCm3 = sonar3.measureDistanceCm();
  

  Serial.print("Distance 1: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  Serial.print("Distance 2: ");
  Serial.print(distanceCm2);
  Serial.println(" cm");
  Serial.print("Distance 3: ");
  Serial.print(distanceCm3);
  Serial.println(" cm");

  Serial.flush();
  
  delay(500);
}
