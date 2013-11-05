#include <WiServer.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,1,100};       // IP address of WiShield
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

void printData(char* data, int len) {
    while (len-- > 0) {
        Serial.print(*(data++));
    }
}

void setup() {
    Serial.begin(57600);
    Serial.println("INIT");
    mySerial.begin(9600);
    WiServer.init(NULL);
    WiServer.enableVerboseMode(true);
    Serial.println("LOOP");
}

uint8 board_ip[] = {192, 168, 1, 3};
GETrequest gRequest(board_ip, 80, "192.168.1.3", "/");
long updateTime = 0;
void loop() {
    if (mySerial.available())
        Serial.write(mySerial.read());
    if (Serial.available())
        mySerial.write(Serial.read());

    if (millis() >= updateTime) {
        gRequest.submit();
        updateTime += 1000 * 20;
    }

    WiServer.server_task();
    delay(10);
}
