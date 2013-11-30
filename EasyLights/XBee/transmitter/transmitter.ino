#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 12); // RX, TX

boolean sample_output = true;
long updateTime = 0;
int samplePin = 64;

void setup()
{
    Serial.begin(9600);
    Serial.println("INIT");
    mySerial.begin(9600);
}

void doSample()
{
    if (sample_output) {
        char mystr[5] = "\10\10\10\10";
        mystr[0] = 'K';
        if (millis() >= updateTime) {
            updateTime = millis() + 3000;
            samplePin += 1;
            if (samplePin >= 70) {
                samplePin = 64;
            }
        }
        mystr[1] = samplePin;
        mystr[2] = 10;
        mySerial.write(mystr);
        Serial.println(mystr);
        Serial.println("*********");
    }
}

void loop()
{
    if (mySerial.available())
        Serial.write(mySerial.read());
    if (Serial.available())
        mySerial.write(Serial.read());
        
    doSample();    
        
}
