// main.ino
// Car-Ma: Embedded Systems Project

#include <Arduino.h>

// Pin definitions
const int ledPin = 13; // Built-in LED pin

void setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // Configure the LED pin as an output
    pinMode(ledPin, OUTPUT);

    // Print a startup message
    Serial.println("Car-Ma system initialized.");
}

void loop() {
    // Blink the LED
    digitalWrite(ledPin, HIGH); // Turn the LED on
    delay(1000);                // Wait for 1 second
    digitalWrite(ledPin, LOW);  // Turn the LED off
    delay(1000);                // Wait for 1 second

    // Print a status message
    Serial.println("Car-Ma system running...");
}