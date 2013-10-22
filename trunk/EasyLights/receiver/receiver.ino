const int pinButton = 2;
const int pinMax = 3;
const int pinLed[pinMax] = {4, 5, 6};
boolean pinState[pinMax] = {LOW, LOW, LOW};


void checkInput(int inByte){
    switch (inByte) {
        case 49:
            Serial.println("ON");
            turnAll(HIGH);
            break;
        case 48:
            Serial.println("OFF");
            turnAll(LOW);
            break;
        case 97: // a
        case 98: // b
        case 99: // c
            pinState[inByte-97] = !pinState[inByte-97];
            break;
        case 116: // t
            Serial.print("\nBoard Time: ");
            Serial.println(millis());
            break;
        default:
            Serial.println(inByte);
    }
}

void turnAll(boolean value){
    for (int i = 0; i < pinMax; i++) {
        pinState[i] = value;
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("Init Receiver");
    Serial.println(" ");

    // initialize an input pin:
    pinMode(pinButton, INPUT);
    // initialize the LED pins as an output:
    for (int i = 0; i < pinMax; i++) {
        pinMode(pinLed[i], OUTPUT);
    }
}

void loop(){
    // check if the pushbutton is pressed.
    if (digitalRead(pinButton) == HIGH) {
        turnAll(LOW);
    }
    // check input from the serial port.
    if (Serial.available()) {
        checkInput(Serial.read());
    }
    // turn the lights
    for (int i = 0; i < pinMax; i++) {
        digitalWrite(pinLed[i], pinState[i]);
    }
}
