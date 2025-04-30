// Author: Abd El-Rahman Mostafa
// Implementation of the Bluetooth driver for HC-05/HC-06 using hardware Serial.

#include "bluetooth.h"

Bluetooth::Bluetooth(uint32_t baud_rate) : baud_rate(baud_rate)
{
}

void Bluetooth::begin()
{
    Serial.begin(baud_rate);
}

void Bluetooth::send(const char *data)
{
    Serial.print(data);
}

void Bluetooth::sendBytes(const uint8_t *data, size_t length)
{
    Serial.write(data, length);
}

int Bluetooth::available()
{
    return Serial.available();
}

int Bluetooth::read()
{
    return Serial.read();
}

String Bluetooth::readString()
{
    return Serial.readStringUntil('\n');
}