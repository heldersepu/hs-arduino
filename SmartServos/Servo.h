#include <Stepper.h>
#define STEPS 32 // steps on the motor
#define MOVE 2048/24 // steps per move


class Servo : public Stepper {
  public:
    Servo(int pin) : Stepper(STEPS, pin, pin+2, pin+1, pin+3) {
      setSpeed(200);
    }

    void move(char x) {
      if (x < 'a') {
        step(MOVE + MOVE*(x-'A'));
      } else {
        step(-MOVE - MOVE*(x-'a'));
      }
    }
};
