#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 12); // RX, TX

const int BoardID = 6;
char MasterKey[8] = "123EASY";


struct leds {
  int pin;
  boolean on;
  char value;
};
const int pinMax = 10;
leds LedsList[4][pinMax] = {
    {{3, 0, 'A'}, {0,0,'~'},   {4, 0, 'C'}, {5, 0, 'A'}, {6, 0, 'C'}, {7, 0, 'F'}, {8, 0, 'L'}, {9, 0, 'H'}, {10, 0, 'L'}, { 0,0,'~'}  },
    {{3, 0, 'B'}, {0,0,'~'},   {4, 0, 'C'}, {5, 0, 'B'}, {6, 0, 'C'}, {7, 0, 'F'}, {8, 0, 'L'}, {9, 0, 'H'}, {10, 0, 'L'}, { 0,0,'~'}  },
    {{3, 0, 'S'}, {0,0,'~'},   {4, 0, 'A'}, {5, 0, 'C'}, {6, 0, 'M'}, {7, 0, 'B'}, {8, 0, 'I'}, {0,0,'~'},   {0,0,'~'},    { 0,0,'~'}  },
    {{3, 0, 'A'}, {3, 0, 'T'}, {4, 0, 'C'}, {5, 0, 'C'}, {6, 0, 'C'}, {7, 0, 'R'}, {8, 0, 'R'}, {9, 0, 'C'}, {10, 0, 'B'}, {10, 0, 'P'}}
};
leds Leds[pinMax];

int setup_status = 0;
boolean verbose_output = false;


void checkInput(int inByte) {
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
        if (Leds[i].pin > 0) {
            if (Leds[i].value <= 'B' ) {
                if (Leds[i].pin == Leds[i+1].pin) {
                    digitalWrite(Leds[i].pin, !(Leds[i].on || Leds[i+1].on));
                    i++;
                } else {
                    digitalWrite(Leds[i].pin, !Leds[i].on);
                }
            } else {
                digitalWrite(Leds[i].pin, !Leds[i].on);
            }
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
        Leds[i] = LedsList[BoardID-3][i];
        if (Leds[i].pin > 0) {
            pinMode(Leds[i].pin, OUTPUT);
        }
    }
}

void loop() {
    // check for error during setup
    if (setup_status != 0) {
        errFlash(setup_status, 250);
    }

    // check input from the serial port.
    while (Serial.available()) {
        checkInput(Serial.read());
    }

    // check input from the soft serial port.
    boolean isOK = false;
    while (mySerial.available()) {
        int inSerial = mySerial.read();
        if (isOK) {
            if (verbose_output) {
                if (inSerial > 10) {
                    Serial.print((char)inSerial);
                    Serial.print(" ");
                }
            }
            if (inSerial == 'K') {
                if (verbose_output) Serial.println("");
                for (int i = 0; i < pinMax; i++) {
                    Leds[i].on = false;
                }
            } else if (inSerial != 10) {
                checkInput(inSerial);
            } else {
                break;
            }
        } else {
            int j = 0;
            while (mySerial.available() && (inSerial == MasterKey[j++])) {
                if (j > 6) {
                    isOK = true;
                    break;
                }
                inSerial = mySerial.read();
            }
        }
    }

    do_the_lights();
}

