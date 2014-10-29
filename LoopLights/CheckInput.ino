void checkInput(int inByte){
  switch (inByte) {
  case 49:
    Serial.println("ON");
    doLights = false;
    turnAll(HIGH);
    delay(500);
    break;
  case 48:
    Serial.println("OFF");
    doLights = false;
    turnAll(LOW);
    break;
  case 97: // a
  case 98: // b
  case 99: // c
    doLights = false;
    doOnOff(pinLed[inByte-97]);
    delay(250);
    break;
  case 102: // f
    lspeed = 50;
    break;
  case 115: // s
    lspeed = 250;
    doLights = !doLights;
    break;
  case 116: // t
    Serial.print("\nBoard Time: ");
    Serial.println(millis());
    break;
  default:
    Serial.println(inByte);
  }
}
