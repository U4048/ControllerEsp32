#ifndef ENCODER_DRIVER
#define ENCODER_DRIVER

#include "FunctionalInterrupt.h"
#include <Arduino.h>

// Ecoder - датчик вращения двигателя
class Encoder { 
   private :  
     uint8_t pinA;
     uint8_t pinB;
     volatile long counter;
   public : 
     Encoder();
     void setup(uint8_t pinA,  uint8_t pinB, void (*intRoutine)(void));
     long read();
     long readAndReset();
     void reset();
     void inc();
     ~Encoder();
};

#endif //ENCODER_DRIVER