#include "Lewis.h"
Lewis Morse;

void setup() {
  Morse.begin(13, 13, 5);
  pinMode(46, OUTPUT);
  blinkOnce(46);
}

void loop() {
  printLetter('H');
  printLetter('A');
  printLetter('P');
  printLetter('P');
  printLetter('Y');
  delay(500);
  printLetter('B');
  printLetter('I');
  printLetter('R');
  printLetter('T');
  printLetter('H');
  printLetter('D');
  printLetter('A');
  printLetter('Y');    
  blinkOnce(46);
}

void printLetter(uint8_t c) {
  Morse.write(c);
  delay(50);
}

void blinkOnce(int x) {
  digitalWrite(x, HIGH);   
  delay(1000);             
  digitalWrite(x, LOW);    
  delay(500);
}


