struct leds {
  int pin;
  boolean on;
};

const int pinMax = 3;
leds Leds[pinMax] = {{4, false}, {5, false}, {6, false}};
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
    Serial.begin(9600);
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
    
    // do the lights
    for (int i = 0; i < pinMax; i++) {
        digitalWrite(Leds[i].pin, Leds[i].on);
    }
}

