#include "bluetooth.h"
Bluetooth bt(9600);
void setup()
{
    Serial.begin(9600);
    bt.begin();
    bt.send("Arduino Ready!\n");
    Serial.println("Bluetooth initialized. Waiting for commands...");
}
void loop()
{
    if (bt.available())
    {
        char command = bt.read();
        Serial.print("Received: '");
        Serial.print(command);
        Serial.print("' (ASCII: ");
        Serial.print((int)command);
        Serial.println(")");
        switch (command)
        {
        case 'U':
            Serial.println("Up pressed");
            break;
        case 'D':
            Serial.println("Down pressed");
            break;
        case 'L':
            Serial.println("Left pressed");
            break;
        case 'R':
            Serial.println("Right pressed");
            break;
        default:
            Serial.println("Unknown command received");
            break;
        }
    }
}