#include "Servo.h"

const int MAX = 2;
Servo motors[MAX] = {{22}, {30}};

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
  for (int i = 0; i < MAX; i++) {
    motors[i].step(val);  
  }
}
