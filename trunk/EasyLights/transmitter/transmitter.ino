#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup()  
{
    Serial.begin(57600);
    Serial.println("INIT");
    mySerial.begin(9600);
}

void loop()
{
    if (mySerial.available())
        Serial.write(mySerial.read());
    if (Serial.available())
        mySerial.write(Serial.read());
}
