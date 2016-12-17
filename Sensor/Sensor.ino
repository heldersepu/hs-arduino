const int ledRed = 4;
const int ledBlu = 6;
const int sensor = 8;

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
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledBlu, LOW);    
  } else {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledBlu, HIGH     );    
  }
}
