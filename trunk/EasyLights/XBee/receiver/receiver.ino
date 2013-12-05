#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 12); // RX, TX

const int BoardID = 3;
const int pinMax = 6;

struct leds {
  int pin;
  boolean on;
  boolean cyclic;
  char value;
};

leds LedsList[4][pinMax] = {
    {{3, 0, 1, 'A'}, {4, 0, 0, 'F'}, {5, 0, 0, 'C'}, {6, 0, 0, 'L'}, {7, 0, 0, 'H'}, {8, 0, 0, '~'}},
    {{3, 0, 1, 'B'}, {4, 0, 0, 'F'}, {5, 0, 0, 'C'}, {6, 0, 0, 'L'}, {7, 0, 0, 'H'}, {8, 0, 0, '~'}},
    {{3, 0, 1, 'A'}, {4, 0, 1, 'B'}, {5, 0, 0, 'C'}, {6, 0, 0, 'M'}, {7, 0, 0, 'S'}, {8, 0, 0, 'I'}},
    {{5, 0, 0, 'C'}, {6, 0, 0, 'R'}, {3, 0, 0, 'S'}, {4, 0, 0, 'S'}, {3, 0, 1, 'A'}, {4, 0, 1, 'B'}}
};
leds Leds[pinMax];

int setup_status = 0;
unsigned long lastCycle = 0;
boolean cyclicState = LOW;
boolean verbose_output = false;
long updateTime = 0;


void checkInput(int inByte) {
    if (verbose_output) Serial.println(inByte);
    switch (inByte) {
        case 49:
            turnAll(HIGH);
            break;
        case 48:
            turnAll(LOW);
            break;
        case 86:
            doVerbose();
            break;
        case 87:
            doStatus();
            break;
        default:
            for (int i = 0; i < pinMax; i++) {
                if (Leds[i].value == inByte) {
                    Leds[i].on = !Leds[i].on;
                }
            }
    }
}

void doVerbose() {
    verbose_output = !verbose_output;
    Serial.print("verbose: ");
    Serial.println(verbose_output);
}

void doStatus() {
    for (int i = 0; i < pinMax; i++) {
        Serial.print(Leds[i].pin);
        Serial.print(":");
        Serial.print(Leds[i].on);
        Serial.print(".");
        Serial.print(Leds[i].value);
        Serial.print("; ");
    }
    Serial.println("");
}

void turnAll(boolean value) {
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

void do_the_lights() {
    for (int i = 0; i < pinMax; i++) {
        if (Leds[i].cyclic && Leds[i].on) {
            if ((millis() - lastCycle) > 600) {
                lastCycle = millis();
                cyclicState = !cyclicState;
            }
            digitalWrite(Leds[i].pin, cyclicState);
        } else {
            digitalWrite(Leds[i].pin, Leds[i].on);
        }
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("INIT");
    Serial.println(BoardID);
    mySerial.begin(9600);

    // initialize the LED pins as output:
    for (int i = 0; i < pinMax; i++) {
        Leds[i] = LedsList[BoardID][i];
        pinMode(Leds[i].pin, OUTPUT);
    }
}

void loop() {
    // check for error during setup
    if (setup_status != 0) {
        errFlash(setup_status, 250);
    }

    // check input from the serial port.
    while (mySerial.available()) {
        int inSerial = mySerial.read();
        Serial.print(inSerial);
        Serial.print(" ");
        if (inSerial == 'K') {
            for (int i = 0; i < pinMax; i++) {
                Leds[i].on = false;
            }
        } else if (inSerial != 10) {
            checkInput(inSerial);
        } else {
            break;
        }
    }
    Serial.println("");

    do_the_lights();
}

