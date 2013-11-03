const int BoardID = '3';
const int pinMax = 5;

struct leds {
  int pin;
  boolean on;
  boolean cyclic;
};

leds Leds[pinMax] = {{3, 0, 1}, {4, 0, 1}, {5, 0, 0}, {6, 0, 0}, {7, 0, 0}};
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

void do_the_lights()
{
    for (int i = 0; i < pinMax; i++) {
        digitalWrite(Leds[i].pin, Leds[i].on);
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println(BoardID);
    // initialize the LED pins as output:
    for (int i = 0; i < pinMax; i++) {
        pinMode(Leds[i].pin, OUTPUT);
    }
}

void loop(){
    // check for error during setup
    if (setup_status != 0) {
        errFlash(setup_status, 250);
    }

    // check input from the serial port.
    while (Serial.available()) {
        int inSerial = Serial.read();
        checkInput(inSerial);
    }

    do_the_lights();
}

