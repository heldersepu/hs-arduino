#include <Stepper.h> 
#define STEPS 32 // steps on the motor

class Servo : public Stepper {
  public: 
    Servo(int pin) : Stepper(STEPS, pin, pin+1, pin+2, pin+3) {
      setSpeed(200);
    }  
};
