/*
Author: Abd El-Rahman Mostafa
Implementation of the Bluetooth driver for HC-05/HC-06.
*/

#include "bluetooth.h"

Bluetooth::Bluetooth(uint8_t rx_pin, uint8_t tx_pin, uint32_t baud_rate)
    : rx_pin(rx_pin), tx_pin(tx_pin), baud_rate(baud_rate)
{
    btSerial = new SoftwareSerial(rx_pin, tx_pin);
}

Bluetooth::~Bluetooth()
{
    delete btSerial;
}

void Bluetooth::begin()
{
    btSerial->begin(baud_rate);
}

void Bluetooth::send(const char *data)
{
    btSerial->print(data);
}

void Bluetooth::sendBytes(const uint8_t *data, size_t length)
{
    btSerial->write(data, length);
}

int Bluetooth::available()
{
    return btSerial->available();
}

int Bluetooth::read()
{
    return btSerial->read();
}

String Bluetooth::readString()
{
    return btSerial->readStringUntil('\n');
}