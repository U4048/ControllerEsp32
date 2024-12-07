#ifndef MOTOR_DRIVER
#define MOTOR_DRIVER

#include <Arduino.h>

// Управление контроллером двигателя
class Motor {
   private: 
     uint8_t pinA;
     uint8_t pinB;
     uint8_t chanA;
     uint8_t chanB;

   public : 
     Motor();
     void setup(uint8_t pinA, uint8_t pinB, uint8_t chanA, uint8_t chanB);
     void forward(int speed);
     void reverse(int speed);
     void stop();

     ~Motor();
};

#endif //MOTOR_DRIVER