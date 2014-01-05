#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 12); // RX, TX

const int BoardID = 0;
char MasterKey[8] = "123EASY";


struct input {
  int pin;
  boolean on;
  boolean cyclic;
  char value;
};
const int pinMax = 10;
input Input[pinMax] = {
    {17, 0, 1, 'B'}, {18, 0, 0, 'H'}, {19, 0, 0, 'L'}, {20, 0, 1, 'A'}, {21, 0, 0, 'S'},
    {15, 0, 0, 'C'}, {23, 0, 0, 'F'}, {24, 0, 0, 'I'}, {25, 0, 0, 'M'}, {26, 0, 0, 'R'}
};

int blink_freq = 500;
int setup_status = 0;
boolean verbose_output = false;
boolean sample_output = false;
long updateTime = 0;
int samplePin = 0;

unsigned long lastCycle = 0;
boolean cyclicState = LOW;

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
        if ((millis() - lastCycle) > blink_freq) {
            lastCycle = millis();
            cyclicState = !cyclicState;
        }
        if (cyclicState) {
            mystr[2] = Input[samplePin].value;
        }
    } else {
        mystr[2] = Input[samplePin].value;
    }
    mystr[3] = 10;
    mySerial.write(MasterKey);
    mySerial.write(mystr);
    Serial.println(mystr);
    Serial.println("*********");
}

void printStatus(char mystr[pinMax+2]) {
    int j = 0;
    for (int i = 0; i < pinMax+2; i++) {
        if (mystr[i] > 10) {
            j++;
            Serial.print(mystr[i]);
            Serial.print(',');
        }
    }
    for (int i = 0; i < (2*pinMax - 2*j); i++) {
        Serial.print(' ');
    }
    Serial.println("*********");
}

void doLights() {
    char mystr[pinMax+2];
    boolean iStop = false;
    boolean iLeft = false;
    boolean iRigh = false;
    boolean iHigh = false;
    boolean iMark = false;
    boolean iLow = false;
    boolean iFog = false;

    for (int i = 0; i < pinMax+2; i++) {
        mystr[i] = 10;
    }
    int j = 1;
    mystr[0] = 'K';
    for (int i = 0; i < pinMax; i++) {
        if (Input[i].on) {
            if (Input[i].cyclic) {
                if ((millis() - lastCycle) > blink_freq) {
                    lastCycle = millis();
                    cyclicState = !cyclicState;
                }
                if (cyclicState) {
                    mystr[j++] = Input[i].value;
                }
            } else if ((Input[i].value != 'F') && (Input[i].value != 'H') && (Input[i].value != 'L')) {
                mystr[j++] = Input[i].value;
            }

            switch (Input[i].value) {
                case 'A': iLeft = true; break;
                case 'B': iRigh = true; break;
                case 'S': iStop = true; break;
                case 'H': iHigh = true; break;
                case 'M': iMark = true; break;
                case 'L': iLow  = true; break;
                case 'F': iFog  = true; break;
            }
        }
    }

    // turn signal take priority over stop
    if (iStop) {
        if (!iLeft) {
            mystr[j++] = 'T';
        }
        if (!iRigh) {
            mystr[j++] = 'P';
        }
    }

    // fog light are on only if low is on
    if (iFog && iLow) {
        mystr[j++] = 'F';
    }

    // special case for low and high
    if (iLow) {
        if (iHigh) {
            mystr[j++] = 'H';
        } else {
            mystr[j++] = 'L';
        }
    }

    mySerial.write(MasterKey);
    mySerial.write(mystr);

    // turn on-off the onboard lights
    digitalWrite(3, !(iLeft & cyclicState));
    digitalWrite(4, !iMark);
    digitalWrite(5, !iMark);
    digitalWrite(6, !(iRigh & cyclicState));

    if (verbose_output) {
        printStatus(mystr);
    } else {
        delay(10);
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
    Serial.println(BoardID);
    mySerial.begin(9600);

    // initialize onboard lights
    for (int i = 3; i < 7; i++) {
        pinMode(i, OUTPUT);
    }

    // initialize input pins
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
