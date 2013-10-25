#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

void setup()  {
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
  
  pinMode(7, OUTPUT);
  digitalWrite(7, 1);
}

void loop()   
{
    mySerial.print("1");
    delay(100);
    mySerial.print("2");
    delay(100);
    mySerial.print("3");
    
    delay(500);
    if (mySerial.available()) {
        digitalWrite(7, 0);
    }
}
