#include <SPI.h>
#include <Ethernet.h>

struct leds {
  int pin;
  boolean on;
};
const int ledMax = 3;
leds Led[ledMax] = {{5,false}, {6,false}, {7, false}};
char serverName[] = "www.sepuweb.com";
char getWebPage[] = "GET http://www.sepuweb.com/phptest/email/read.php HTTP/1.0";

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetClient client;
boolean pageStart = false;
boolean lastConnected = false;
unsigned long lastConnectionTime = 0;

void httpRequest() {
  Serial.println("connecting...");
  if (client.connect(serverName, 80)) {
    delay(1000);
    Serial.println("connected");
    client.println(getWebPage);
    client.println();
    lastConnectionTime = millis();
  }
  else {
    Serial.println("connection failed");
    client.stop();
  }
}

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  for (int i = 0; i < ledMax; i++) {
    pinMode(Led[i].pin, OUTPUT);
  }
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while(true);
  }
}

void loop()
{
  // Read the web page
  pageStart = false;
  while (client.available()) {
    char c = client.read();
    if (pageStart) {
      Serial.print(c);
      // Each letter corresponds to an led
      if ((c > 64) && (c < (65 + ledMax))) {
        Led[c-65].on = true;
      }
    }
    if (c == '~') {
      pageStart = true;
      // Set all LED off
      for (int i = 0; i < ledMax; i++) {
        Led[i].on = false;
      }
    }
  }
  // Disconect the client
  if (!client.connected() && lastConnected) {
    Serial.println("disconnecting.");
    Serial.println();
    client.stop();
  }
  // If not connected do the request
  if (!client.connected()) {
    httpRequest();
  }
  lastConnected = client.connected();
  // Turn lights ON or OFF
  for (int i = 0; i < ledMax; i++) {
    if (Led[i].on) {
      digitalWrite(Led[i].pin, HIGH);
    } else {
      digitalWrite(Led[i].pin, LOW);
    }
  }
}
