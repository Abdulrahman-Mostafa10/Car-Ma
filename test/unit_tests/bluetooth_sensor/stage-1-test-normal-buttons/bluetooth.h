// Author: Abd El-Rahman Mostafa
// This file is meant to drive a Bluetooth module (e.g., HC-05/HC-06)
// with an easy-to-understand interface.
// It uses Arduino hardware Serial for pins 0 (RX) and 1 (TX).

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

class Bluetooth
{
private:
    uint32_t baud_rate; // Baud rate for Bluetooth communication (default: 9600 for HC-05/HC-06)

public:
    Bluetooth(uint32_t baud_rate = 9600);

    void begin();

    void send(const char *data);

    void sendBytes(const uint8_t *data, size_t length);

    int available();

    int read();

    String readString();
};

#endif