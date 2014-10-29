const int pinLed[3] = {2, 4, 6};
const int pinMax = sizeof(pinLed)/sizeof(int);


void setup() {
  // initialize the LED pin as an output:
  for (int i = 0; i < pinMax; i++) {
    pinMode(pinLed[i], OUTPUT);
  }  
}

void loop(){   
  for (int i = 0; i < pinMax; i++) {
    digitalWrite(pinLed[i], HIGH);
    delay(500);
    digitalWrite(pinLed[i], LOW);
    delay(250);
  }  
}

