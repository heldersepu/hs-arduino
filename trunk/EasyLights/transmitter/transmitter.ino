#include <WiServer.h>

boolean verbose_output = false;
long updateTime = 0;
char lights[5] = {'A','B','C','D','E'};
int light = 0;

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
            verbose_output = !verbose_output;
            Serial.print("verbose: ");
            Serial.println(verbose_output);
            WiServer.enableVerboseMode(verbose_output);
            break;
        default:
            if (verbose_output) {
                Serial.print("checkInput: ");
                Serial.println(inByte);
            }
    }
}

boolean serveFunction(char* URL) {
    if (verbose_output) {
        Serial.println(URL);
    }
    if (strcmp(URL, "/3") == 0) {
        if (millis() >= updateTime) {
            updateTime = millis() + 5000;
            light++;
            if (light > 4) {
                light = 0;
            }
        }
        WiServer.print(lights[light]);
        return true;
    } else if (strcmp(URL, "/4") == 0) {
        WiServer.print("NOT ACTIVE");
        return true;
    } else if (strcmp(URL, "/5") == 0) {
        WiServer.print("NOT ACTIVE");
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
    Serial.begin(57600);
    Serial.println("INIT");
    WiServer.init(serveFunction);
    WiServer.enableVerboseMode(false);
    Serial.println("LOOP");
}

void loop() {
    WiServer.server_task();

    // check input from the serial port.
    while (Serial.available()) {
        checkInput(Serial.read());
    }

    delay(10);
}

