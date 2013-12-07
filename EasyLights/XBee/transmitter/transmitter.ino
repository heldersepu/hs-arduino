#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 12); // RX, TX

int setup_status = 0;
boolean verbose_output = false;
boolean sample_output = false;
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

input Input[pinMax] = {{17, 0, 1, 'B'}, {18, 0, 0, 'H'}, {19, 0, 0, 'L'}, {20, 0, 1, 'A'}, {21, 0, 1, 'S'},
                       {22, 0, 0, 'C'}, {23, 0, 0, 'F'}, {24, 0, 0, 'I'}, {22, 0, 0, 'M'}, {22, 0, 0, 'R'}};


void checkInput(int inByte) {
    if (verbose_output) Serial.println(inByte);
    switch (inByte) {
        case 49:
            //turnAll(HIGH);
            break;
        case 48:
            //turnAll(LOW);
            break;
        case 86:
            doVerbose();
            break;
        case 87:
            doStatus();
            break;
        case 88:
            sample_output = !sample_output;
            break;
        default:
            if (verbose_output) {
                Serial.print("checkInput: ");
                Serial.println(inByte);
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
        Serial.print(Input[i].pin);
        Serial.print(":");
        Serial.print(Input[i].on);
        Serial.print(".");
        Serial.print(Input[i].value);
        Serial.print("; ");
    }
    Serial.println("");
}

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

void doLights() {
    char mystr[pinMax+2];
    boolean iStop = false;
    for (int i = 0; i < pinMax+2; i++) {
        mystr[i] = 10;
    }
    int j = 0;
    mystr[0] = 'K';
    for (int i = 0; i < pinMax; i++) {
        if (Input[i].on) {
            if (Input[i].cyclic) {
                if ((millis() - lastCycle) > 600) {
                    lastCycle = millis();
                    cyclicState = !cyclicState;
                }
                if (cyclicState) {
                    mystr[j++] = Input[i].value;
                }
            } else {
                mystr[j++] = Input[i].value;
            }
            if (mystr[j-1] == 'S') {
                iStop = true;
            }
        }
    }
    
    if (iStop) {
        mystr[j++] = 'T';
        mystr[j++] = 'P';
    }
    mySerial.write(mystr);
    
    if (verbose_output) {
        for (int i = 0; i < pinMax+2; i++) {
            Serial.print(mystr[i]);
            Serial.print(',');
        }
        Serial.println("*********");
    }
}

void errFlash(int mTime, int mDelay) {
    if (millis() < mTime) {
        //turnAll(!Leds[0].on);
        delay(mDelay);
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("INIT");
    mySerial.begin(9600);
    
    for (int i = 0; i < pinMax; i++) {
       pinMode(Input[i].pin, INPUT_PULLUP);
    }
}

void loop() {
    // check input from the serial port.
    while (Serial.available()) {
        checkInput(Serial.read());
    }
    
    // check the switches.
    for (int i = 0; i < pinMax; i++) {
       Input[i].on = !digitalRead(Input[i].pin);
    }

    if (sample_output) {
        doSample();
    } else {
        doLights();
    }
}
