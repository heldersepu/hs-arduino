const int pinButton = 2;
const int pinLed[3] = {9, 10, 11};
const int pinMax = sizeof(pinLed)/sizeof(int);

int fadeValue = 0;
unsigned long clickTime = 0;
unsigned long ledTime = 0;
unsigned long fTime = 0;
unsigned long mTime = 0;
boolean doubleClick = false;
boolean verbose = false;

int ledActive;
int ledLoops = 500;

void doOnOff(int portNum){
  if (ledTime == 0) {
    ledTime = millis();
  }
  if ((millis() - ledTime) < 100) {
    digitalWrite(portNum, HIGH);
  } else {
    digitalWrite(portNum, LOW);
    if ((millis() - ledTime) > 200) {
      ledTime = 0;
    }
  }
}

void doFadeOnOff(int portNum){
  if (ledTime == 0) {
    ledTime = millis();
    fadeValue = 0;
  }
  if ((millis() - ledTime) > 5) {
    fadeValue +=2;
    ledTime = millis();
  }

  if (fadeValue < 250) {
    analogWrite(portNum, fadeValue);
  } else {
    analogWrite(portNum, (500 - fadeValue));
    if (fadeValue >= 500) {
      ledTime = 0;
    }
  }
  Serial.println(portNum);
}

void checkInput(int inByte){
  switch (inByte) {
    case 49:
      Serial.println("ON");
      turnAll(HIGH);
      delay(500);
      break;
    case 48:
      Serial.println("OFF");
      turnAll(LOW);
      break;
    case 97: // a
    case 98: // b
    case 99: // c
      ledTime = 0;
      for (int i = 0; i < 1000; i++) {
        doOnOff(pinLed[inByte-97]);
      }
      delay(250);
      break;
    case 116: // t
      Serial.print("\nBoard Time: ");
      Serial.println(millis());
      Serial.print("clickTime: ");
      Serial.println(clickTime);
      Serial.print("ledTime: ");
      Serial.println(ledTime);
      break;
    case 118: //v
      verbose = !(verbose);
      break;
    default:
      Serial.println(inByte);
  }
}

void doOutPut(int pin, int count) {
  Serial.print(pin);
  Serial.print(",");
  Serial.print(count);
  Serial.print("; ");
  if (count % 5 == 0){
    Serial.println("  ");
  }
}

void turnAll(boolean value){
  for (int i = 0; i < pinMax; i++) {
    digitalWrite(pinLed[i], value);
  }
  if (!value) {
    ledTime = 0;
  }
}


void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(pinButton, INPUT);
  Serial.begin(9600);
  Serial.println("Testing Random");
  Serial.println(" ");

  // initialize the LED pin as an output:
  for (int i = 0; i < pinMax; i++) {
    pinMode(pinLed[i], OUTPUT);
  }
}

void loop(){
  // check if the pushbutton is pressed.
  if (digitalRead(pinButton) == HIGH) {
    turnAll(LOW);
    ledLoops = 1;
    int diffTime = millis() - clickTime;
    if (diffTime > 10){
      doubleClick = (diffTime < 500);
    }
    clickTime = millis();
  } else {
    if (Serial.available()) {
      checkInput(Serial.read());
    }

    if (ledLoops < 11) {
      if (ledTime == 0){
        ledActive = random(pinMax);
        if (verbose) {
          doOutPut(pinLed[ledActive], ledLoops);
        }
        ledLoops++;
      }
      if (doubleClick){
        doOnOff(pinLed[ledActive]);
      } else {
        doFadeOnOff(pinLed[ledActive]);
      }
    } else {
      // turn LED off:
      turnAll(LOW);
      ledTime = 0;
      doubleClick = false;
    }
    mTime =millis();
    fTime =(mTime % 900000);
    if (fTime > 0 && fTime < 10) {
      ledLoops = 1;
      doubleClick = true;
    }
  }
}
