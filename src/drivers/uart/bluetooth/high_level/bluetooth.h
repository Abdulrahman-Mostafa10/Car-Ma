/*
Author: Abd El-Rahman Mostafa
The file is meant to drive the bluetooth module (HC-05/HC-06)
with an easy-to-understand the interface
It uses Arduino functions for now
*/

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class Bluetooth
{
private:
    SoftwareSerial *btSerial; // SoftwareSerial for Bluetooth communication
    uint8_t rx_pin;
    uint8_t tx_pin;
    uint8_t baud_rate;

public:
    Bluetooth(uint8_t rx_pin, uint8_t tx_pin, uint32_t baud_rate = 9600);

    ~Bluetooth();

    void begin();

    void send(const char *data);

    void sendBytes(const uint8_t *data, size_t length);

    int available();

    int read();

    String readString();
};

#endif