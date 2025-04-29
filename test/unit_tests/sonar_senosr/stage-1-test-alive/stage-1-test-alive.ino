// Author: Amir Kedis
// This file is used to check if the sensor is working
// I will not add any additional complexities ISA.


// PORTS
#define TRIG_PIN 9
#define ECHO_PIN 10

#define SONAR_SPEED 0.034 // cm/us

long duration;
int distance; // distance in cm

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Ultrasonic Sensor Test");
  Serial.println("-----------------------");
}

void loop() {
  // Clear TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // give the sensor the pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the ECHO pin
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance
  distance = duration * SONAR_SPEED / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
