#include <SoftwareSerial.h>

struct leds {
  int pin;
  boolean on;
};
const int pinButton = 8;
const int pinMax = 3;
leds Leds[pinMax] = {{4, false}, {5, false}, {6, false}};
SoftwareSerial mySerial(2, 3); // RX, TX
int setup_status = 0;


void checkInput(int inByte){
    switch (inByte) {
        case 49:
            turnAll(HIGH);
            break;
        case 48:
            turnAll(LOW);
            break;
        default:
            if (inByte > 64 && inByte < 123) {
                Leds[inByte-65].on = !Leds[inByte-65].on;
            }
    }
}

void turnAll(boolean value){
    for (int i = 0; i < pinMax; i++) {
        Leds[i].on = value;
    }
}

void errFlash(int mTime, int mDelay) {
    if (millis() < mTime) {
        turnAll(!Leds[0].on);
        delay(mDelay);
    }
}

void setup() {
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    Serial.println("Init Receiver");
    Serial.println(" ");

    // initialize an input pin:
    pinMode(pinButton, INPUT);
    // initialize the LED pins as an output:
    for (int i = 0; i < pinMax; i++) {
        pinMode(Leds[i].pin, OUTPUT);
    }        
}

void loop(){
    // check for error during setup
    if (setup_status != 0) {
        errFlash(setup_status, 250);
    }           
    // check if the pushbutton is pressed.
    if (digitalRead(pinButton) == HIGH) {
        turnAll(LOW);
        Serial.println("button");
        Serial.println(mySerial.available());
    }
    // check input from the serial port.
    if (mySerial.available()) {
        int inSerial = mySerial.read();
        checkInput(inSerial);
        Serial.println(inSerial);
    }
    // do the lights
    for (int i = 0; i < pinMax; i++) {
        digitalWrite(Leds[i].pin, Leds[i].on);
    }
}
