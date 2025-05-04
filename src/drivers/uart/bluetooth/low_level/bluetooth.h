// Author: Abd El-Rahman Mostafa (Modified by Amir)
// Bluetooth driver for HC-05/HC-06 using direct AVR register access

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>
#include <Arduino.h>

class Bluetooth
{
private:
    uint32_t baud_rate;
    uint16_t ubrr;

public:
    Bluetooth(uint32_t baud_rate = 9600, uint32_t cpu_clock = 16000000UL)
    {
        this->baud_rate = baud_rate;
        // NOTE: Calculate UBRR value for the desired baud rate
        // Also notice UART normal prescaler is 16
        ubrr = (cpu_clock / 16 / baud_rate) - 1;
    }

    void begin()
    {
        // Set baud rate
        UBRR0H = (uint8_t)(ubrr >> 8);
        UBRR0L = (uint8_t)(ubrr);

        // Enable receiver and transmitter
        UCSR0B = (1 << RXEN0) | (1 << TXEN0);

        // Set frame format: 8 data bits, 2 stop bits
        UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    }

    // Send a null-terminated character string
    void send(const char *data)
    {
        while (*data)
        {
            sendByte(*data++);
        }
    }

    // Send raw byte array
    void sendBytes(const uint8_t *data, size_t length)
    {
        for (size_t i = 0; i < length; i++)
        {
            sendByte(data[i]);
        }
    }

    // Check if data is available to read
    int available()
    {
        return (UCSR0A & (1 << RXC0));
    }

    // Read a single byte (blocking)
    int read()
    {
        while (!available())
            ;
        return UDR0;
    }

    // Read until newline and return as String
    String readString()
    {
        String result = "";
        char c;
        while (true)
        {
            if (available())
            {
                c = UDR0;
                if (c == '\n')
                    break;
                result += c;
            }
        }
        return result;
    }

private:
    // Helper to send a single byte
    void sendByte(uint8_t byte)
    {
        while (!(UCSR0A & (1 << UDRE0)))
            ; // Wait for empty transmit buffer
        UDR0 = byte;
    }
};

#endif // BLUETOOTH_H
