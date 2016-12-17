void checkInput(int inByte){
  switch (inByte) {
  case 97: // a
    Serial.println("ON 1");    
    digitalWrite(ledRed, HIGH);
    break;
  case 98: // b
    Serial.println("ON 2");
    digitalWrite(ledBlu, HIGH);
    break;
  case 99: // c
    Serial.println("OFF");
    digitalWrite(ledRed, LOW);
    digitalWrite(ledBlu, LOW);
    break;
  case 116: // t
    Serial.print("\nBoard Time: ");
    Serial.println(millis());
    break;
  default:
    Serial.println(inByte);
  }
}
