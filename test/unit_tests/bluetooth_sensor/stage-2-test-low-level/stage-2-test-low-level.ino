#include "bluetooth.h"

Bluetooth bt(9600); // HC-05 default baud rate

void setup()
{
    // Only use Serial for setup diagnostics (disconnect HC-05 TX before upload)
    // Serial.begin(9600);
    Serial.println("Setting up Bluetooth...");

    bt.begin();
    bt.send("Arduino Ready!\n");
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

        bt.send("Received: '");
        bt.send(&command);
        bt.send("' (ASCII: ");

        char asciiBuf[5];
        itoa((int)command, asciiBuf, 10);
        bt.send(asciiBuf);
        bt.send(")\n");

        switch (command)
        {
        case '1':
            bt.send("Reset pressed\n");
            break;
        case '2':
            bt.send("Start pressed\n");
            break;
        case '3':
            bt.send("Parking pressed\n");
            break;
        default:
            bt.send("Unknown command received\n");
            break;
        }
    }
}
