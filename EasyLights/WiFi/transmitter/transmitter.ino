#include <WiServer.h>
const int pinMax = 10;

struct input {
  int pin;
  boolean on;
  char value;
};

input Input[pinMax] = {{17, 0, 'B'}, {18, 0, 'H'}, {19, 0, 'L'}, {20, 0, 'A'}, {21, 0, 'S'},
                       {22, 0, 'C'}, {23, 0, 'F'}, {24, 0, 'I'}, {22, 0, 'M'}, {22, 0, 'R'}};
int setup_status = 0;
boolean verbose_output = false;
int samplePin = 0;
boolean sample_output = false;
long updateTime = 0;

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,1,100};        // IP of WiShield
unsigned char gateway_ip[] = {192,168,1,1};	   // router or gateway
unsigned char subnet_mask[] = {255,255,255,0};     // subnet mask
const prog_char ssid[] PROGMEM = {"ARDUINO"};      // max 32 bytes

unsigned char security_type = 0;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"udinese10"};	// max 64 chars
prog_uchar wep_keys[] PROGMEM = { };
unsigned char wireless_mode = 2;
unsigned char ssid_len;
unsigned char security_passphrase_len;
// End of wireless configuration parameters -----------------------------------


void checkInput(int inByte) {
    if (verbose_output) Serial.println(inByte);
    switch (inByte) {
        case 49:
            //turnAll(HIGH);
            break;
        case 48:
            //turnAll(LOW);
            break;
        case 86:
            doVerbose();
            break;
        case 87:
            doStatus();
            break;
        case 88:
            sample_output = !sample_output;
            break;
        default:
            if (verbose_output) {
                Serial.print("checkInput: ");
                Serial.println(inByte);
            }
    }
}

void doVerbose() {
    verbose_output = !verbose_output;
    Serial.print("verbose: ");
    Serial.println(verbose_output);
    WiServer.enableVerboseMode(verbose_output);
}

void doStatus() {
    for (int i = 0; i < pinMax; i++) {
        Serial.print(Input[i].pin);
        Serial.print(":");
        Serial.print(Input[i].on);
        Serial.print(".");
        Serial.print(Input[i].value);
        Serial.print("; ");
    }
    Serial.println("");
}

void errFlash(int mTime, int mDelay) {
    if (millis() < mTime) {
        //turnAll(!Leds[0].on);
        delay(mDelay);
    }
}

boolean serveFunction(char* URL) {
    if (verbose_output) {
        Serial.println(URL);
        Serial.println(millis());
    }

    if (sample_output) {
        if (millis() >= updateTime) {
            updateTime = millis() + 3000;
            samplePin += 1;
            if (samplePin >= pinMax) {
                samplePin = 0;
            }
        }
        WiServer.print(Input[samplePin].value);
        return true;
    }

    if (URL[0] == '/') {
        for (int i = 0; i < pinMax; i++) {
            if (verbose_output) {
                Serial.print(i);
                Serial.print(Input[i].on);
                Serial.print(Input[i].value);
            }
            if (Input[i].on) {
                WiServer.print(Input[i].value);
            }
        }
        if (verbose_output) Serial.println("");
        return true;
    }
    return false;
}

void responseFunc(char* data, int len) {
    while (len-- > 0) {
        Serial.print(*(data++));
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("INIT");
    pinMode(3, OUTPUT);

    //for (int i = 0; i < pinMax; i++) {
    //    pinMode(Input[i].pin, INPUT_PULLUP);
    //}
    Serial.println("INPUT_PULLUP");
    WiServer.enableVerboseMode(true);
    Serial.println("serveFunction");
    WiServer.init(serveFunction);

    Serial.println("LOOP");
}

void loop() {
    WiServer.server_task();

    // check input from the serial port.
    while (Serial.available()) {
        checkInput(Serial.read());
    }
    //for (int i = 0; i < pinMax; i++) {
    //    Input[i].on = !digitalRead(Input[i].pin);
    //}
    delay(10);
}

