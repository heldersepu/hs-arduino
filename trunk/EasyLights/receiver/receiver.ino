#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1); // RX, TX

void setup()  {
  Serial.begin(57600);
  Serial.println("init");
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void loop()   
{
  if (mySerial.available()) {
      Serial.print((char)mySerial.read());
  }
  if (Serial.available()) {
      mySerial.print((char)Serial.read());
  }
  delay(100);
}
