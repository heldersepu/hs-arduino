#include "Servo.h"

Servo stepper(4);

void setup() {
  Serial.begin(9600);  
}

void loop() {
  if (Serial.available()>0) {
    start(Serial.parseInt());    
  }   
}

void start(int val) {
  Serial.println(val);
  stepper.step(val);   
}
