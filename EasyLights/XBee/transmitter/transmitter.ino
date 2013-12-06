#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 12); // RX, TX

boolean sample_output = true;
long updateTime = 0;
int samplePin = 0;

unsigned long lastCycle = 0;
boolean cyclicState = LOW;

const int pinMax = 10;

struct input {
  int pin;
  boolean on;
  boolean cyclic;
  char value;
};

input Input[pinMax] = {{17, 0, 1, 'B'}, {18, 0, 0, 'H'}, {19, 0, 0, 'L'}, {20, 0, 1, 'A'}, {21, 0, 1, 'B'},
                       {22, 0, 0, 'C'}, {23, 0, 0, 'F'}, {24, 0, 0, 'I'}, {22, 0, 0, 'M'}, {22, 0, 0, 'R'}};

void doSample() {
    char mystr[5] = "\10\10\10\10";
    mystr[0] = 'K';
    if (millis() >= updateTime) {
        updateTime = millis() + 5000;
        samplePin += 1;
        if (samplePin > pinMax) {
            samplePin = 0;
        }
    }
    if (Input[samplePin].cyclic) {
        if ((millis() - lastCycle) > 600) {
            lastCycle = millis();
            cyclicState = !cyclicState;
        }
        if (cyclicState) {
            mystr[1] = Input[samplePin].value;
        }
    } else {
        mystr[1] = Input[samplePin].value;
    }
    mystr[2] = 10;
    mySerial.write(mystr);
    Serial.println(mystr);
    Serial.println("*********");
}

void setup() {
    Serial.begin(9600);
    Serial.println("INIT");
    mySerial.begin(9600);
}

void loop() {
    if (mySerial.available())
        Serial.write(mySerial.read());
    if (Serial.available())
        mySerial.write(Serial.read());

    if (sample_output) {
        doSample();
    }

}
