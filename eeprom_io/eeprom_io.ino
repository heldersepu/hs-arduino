#include <EEPROM.h>
char MasterKey[8] = "123EASY";

void writeMasterKey() {
    for (int i = 1; i < 8; i++) {
        EEPROM.write(i, MasterKey[i-1]);
    }
}

void setup() {
    Serial.begin(9600);

    char TempKey[8] = "-------";
    for (int i = 1; i < 8; i++) {
        TempKey[i-1] = EEPROM.read(i);
    }

    if (strcmp(TempKey, MasterKey) == 0) {
        Serial.println(TempKey);
    } else {
        Serial.println("writeMasterKey");
        writeMasterKey();
    }
}

void loop() {
    delay(500);
}
