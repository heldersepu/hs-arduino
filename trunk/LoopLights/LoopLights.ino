const int pinLed[3] = {2, 4, 6};
const int pinMax = sizeof(pinLed)/sizeof(int);
boolean doLights = true;
int lspeed = 250;

void turnAll(boolean value){
  for (int i = 0; i < pinMax; i++) {
    digitalWrite(pinLed[i], value);
  }
}

void doOnOff(int portNum){
  digitalWrite(portNum, HIGH);
  delay(lspeed*2);
  digitalWrite(portNum, LOW);
  delay(lspeed);
}

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  for (int i = 0; i < pinMax; i++) {
    pinMode(pinLed[i], OUTPUT);
  }  
}

void loop(){   
  for (int i = 0; i < pinMax; i++) {
    if (Serial.available()) {
      checkInput(Serial.read());
    }
    if (doLights) {
      doOnOff(pinLed[i]);      
    }
  }  
}

