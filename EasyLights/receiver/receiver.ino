#include <WiServer.h>
const int BoardID = 3;
const int pinMax = 5;

struct leds {
  int pin;
  boolean on;
  boolean cyclic;
};

leds Leds[pinMax] = {{3, 0, 1}, {4, 0, 1}, {5, 0, 0}, {6, 0, 0}, {7, 0, 0}};
int setup_status = 0;
unsigned long lastCycle = 0;
boolean cyclicState = LOW;

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,1, BoardID};   // IP address of WiShield
unsigned char gateway_ip[] = {192,168,1,1};	   // router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};     // subnet mask for the local network
const prog_char ssid[] PROGMEM = {"ARDUINO"};      // max 32 bytes

unsigned char security_type = 0;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"udinese10"};	// max 64 characters
prog_uchar wep_keys[] PROGMEM = { };
unsigned char wireless_mode = 2;
unsigned char ssid_len;
unsigned char security_passphrase_len;
// End of wireless configuration parameters ----------------------------------------

void checkInput(int inByte){
    switch (inByte) {
        case 49:
            turnAll(HIGH);
            break;
        case 48:
            turnAll(LOW);
            break;
        default:
            if (inByte > 64 && inByte < 123) {
                Leds[inByte-65].on = !Leds[inByte-65].on;
            }
    }
}

void turnAll(boolean value){
    for (int i = 0; i < pinMax; i++) {
        Leds[i].on = value;
    }
}

void errFlash(int mTime, int mDelay) {
    if (millis() < mTime) {
        turnAll(!Leds[0].on);
        delay(mDelay);
    }
}

void do_the_lights() {
    for (int i = 0; i < pinMax; i++) {
        if (Leds[i].cyclic && Leds[i].on) {
            if ((millis() - lastCycle) > 600) {
                lastCycle = millis();
                cyclicState = !cyclicState;
            }
            digitalWrite(Leds[i].pin, cyclicState);
        } else {
            digitalWrite(Leds[i].pin, Leds[i].on);
        }
    }
}

boolean serveFunction(char* URL) {  
    if (strcmp(URL, "/") == 0) {
        WiServer.print("Hello World!");
        turnAll(HIGH);        
        return true;
    }
    return false;
}

void printData(char* data, int len) {
    while (len-- > 0) {
        Serial.print(*(data++));
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("INIT");
    Serial.println(BoardID);
    // initialize the LED pins as output:
    for (int i = 0; i < pinMax; i++) {
        pinMode(Leds[i].pin, OUTPUT);
    }
    // initialize the serve function
    WiServer.init(serveFunction);
    WiServer.enableVerboseMode(true);
    Serial.println("LOOP");
}

void loop(){
    // check for error during setup
    if (setup_status != 0) {
        errFlash(setup_status, 250);
    }

    // check input from the serial port.
    while (Serial.available()) {
        int inSerial = Serial.read();
        checkInput(inSerial);
    }

    WiServer.server_task(); 
    do_the_lights();
    delay(10);    
}

