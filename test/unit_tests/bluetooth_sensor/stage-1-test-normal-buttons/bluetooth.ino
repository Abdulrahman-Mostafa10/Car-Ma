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
#ifdef IGNORE_ZERO
        if (command == '0')
            return;
#endif
        Serial.print("Received: '");
        Serial.print(command);
        Serial.print("' (ASCII: ");
        Serial.print((int)command);
        Serial.println(")");
        switch (command)
        {
        case 'R':
            Serial.println("Reset pressed");
            break;
        case 'S':
            Serial.println("Start pressed");
            break;
        case 'P':
            Serial.println("Parking pressed");
            break;
        default:
            Serial.println("Unknown command received");
            break;
        }
    }
}