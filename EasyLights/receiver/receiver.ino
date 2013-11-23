#include <WiServer.h>
char BoardID[3] = {"/3"};
const int pinMax = 6;

struct leds {
  int pin;
  boolean on;
  boolean cyclic;
  char value;
};

leds LedsList[4][pinMax] = {
    {{3, 0, 1, 'A'}, {4, 0, 0, 'F'}, {5, 0, 0, 'C'}, {6, 0, 0, 'L'}, {7, 0, 0, 'H'}, {8, 0, 0, '~'}},
    {{3, 0, 1, 'B'}, {4, 0, 1, 'F'}, {5, 0, 0, 'C'}, {6, 0, 0, 'L'}, {7, 0, 0, 'H'}, {8, 0, 0, '~'}},
    {{3, 0, 1, 'A'}, {4, 0, 1, 'B'}, {5, 0, 0, 'C'}, {6, 0, 0, 'M'}, {7, 0, 0, 'S'}, {8, 0, 0, 'I'}},
    {{5, 0, 0, 'C'}, {6, 0, 0, 'R'}, {3, 0, 0, 'S'}, {4, 0, 0, 'S'}, {3, 0, 1, 'A'}, {4, 0, 1, 'B'}}
};
leds Leds[pinMax];

int setup_status = 0;
unsigned long lastCycle = 0;
boolean cyclicState = LOW;
boolean verbose_output = false;
long updateTime = 0;

// Wireless configuration parameters ----------------------------------------
uint8 ip[] = {192,168,1,100};
GETrequest gRequest(ip, 80, "192.168.1.100", BoardID);

unsigned char local_ip[] = {192,168,1, BoardID[1]};   // IP of WiShield
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
            turnAll(HIGH);
            break;
        case 48:
            turnAll(LOW);
            break;
        case 86:
            verbose_output = !verbose_output;
            Serial.print("verbose: ");
            Serial.println(verbose_output);
            WiServer.enableVerboseMode(verbose_output);
            break;
        default:
            for (int i = 0; i < pinMax; i++) {
                if (Leds[i].value == inByte) {
                    Leds[i].on = !Leds[i].on;
                }
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
        WiServer.print("ACK");
        return true;
    }
    return false;
}

void responseFunc(char* data, int len) {
    if (verbose_output) {
        Serial.print("responseFunc: " );
        Serial.println(data);
    }
    boolean isOK = false;
    while (len-- > 0) {
        if (*(data++) == 'K') {
            isOK = true;
            break;
        }
    }
    if (isOK) {
        for (int i = 0; i < pinMax; i++) {
            Leds[i].on = false;
        }
        while (len-- > 0) {
            checkInput(*(data++));
        }
    }
}


void setup() {
    Serial.begin(9600);
    Serial.println("INIT");
    Serial.println(BoardID);
    
    // initialize the LED pins as output:
    for (int i = 0; i < pinMax; i++) {
        Leds[i] = LedsList[BoardID[1]-51][i];
        pinMode(Leds[i].pin, OUTPUT);
    }

    // initialize the wifi function
    WiServer.init(NULL);
    WiServer.enableVerboseMode(false);
    gRequest.setReturnFunc(responseFunc);

    Serial.println("LOOP");
}


void loop() {
    // check for error during setup
    if (setup_status != 0) {
        errFlash(setup_status, 250);
    }

    // check input from the serial port.
    while (Serial.available()) {
        checkInput(Serial.read());
    }

    // request data from transmitter
    if (millis() >= updateTime) {
        updateTime = millis() + 1000;
        gRequest.submit();
        if (verbose_output) Serial.println("gRequest");
    }
    WiServer.server_task();
    do_the_lights();
    delay(10);
}

