#include "Lcd.h"
#include "Servo.h"

const int MAX = 3;
Servo motors[MAX] = {{22}, {30}, {38}};
Lcd lcd;

void setup() {
  Serial.begin(9600);
  lcd.hello();
}

void loop() {
  if (Serial.available()>0) {
    start(Serial.readString());
  }
}

void start(String val) {
  Serial.println(val);
  for (int i = 0; i < val.length(); i+=2) {
    if (val[i] > 10) {
      action(val[i],val[i+1]);
    }
  }
}

void action(char k, char v) {
  if (k >= '0' && k <= '9') {
    motors[k - '0'].move(v);
  } else if (k == '.'){
    Serial.println("sleep");
  } else {
    Serial.print("Invalid Key=");
    Serial.println(k);
  }
}
