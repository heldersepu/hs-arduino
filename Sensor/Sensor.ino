const int ledRed = 4;
const int ledBlu = 6;
const int sensor = 8;

void changeLed(int ledOn, int ledOff) {
  digitalWrite(ledOn, HIGH);
  digitalWrite(ledOff, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlu, OUTPUT);
  pinMode(sensor, INPUT);
}

void loop() {
  if (Serial.available()) {
    checkInput(Serial.read());
  }

  if (digitalRead(sensor) == HIGH) {
    changeLed(ledRed, ledBlu);
  } else {
    changeLed(ledBlu, ledRed);
  }
}

