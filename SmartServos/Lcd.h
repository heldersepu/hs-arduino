#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//https://bitbucket.org/fmalpartida/new-liquidcrystal

#define BACKLIGHT_PIN     13

class Lcd : public LiquidCrystal_I2C {
  public:
    Lcd() : LiquidCrystal_I2C( 0x27, 2,1,0,4,5,6,7,3, POSITIVE ) {
      pinMode( BACKLIGHT_PIN, OUTPUT );
      digitalWrite( BACKLIGHT_PIN, HIGH );
    }

    void init() {
      begin(20,4);
      home();
    }

    void hello() {
      print(" HELLO ARDUINO ");
    }
};
