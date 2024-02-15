#ifndef MOTOR_DRIVER
#define MOTOR_DRIVER

#include <Arduino.h>

// Управление контроллером двигателя
class Motor {
   private: 
     uint8_t pinIN;
     uint8_t pinPWM;
   public : 
     Motor();
     void setup(uint8_t pinIN, uint8_t pinPWM);
     void setSpeed(int speed);
     ~Motor();
};

#endif //MOTOR_DRIVER