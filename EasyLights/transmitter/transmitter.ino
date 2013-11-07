#include <WiServer.h>

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

boolean serveFunction(char* URL) {
    if (strcmp(URL, "/") == 0) {
        WiServer.print("ACK");
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
    delay(10);
}

