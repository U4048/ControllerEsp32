#include "encoder_driver.h"
#include <Arduino.h>

Encoder::Encoder (){
    this->counter = 0;
}

void Encoder::setup(uint8_t pinA,  uint8_t pinB, void (*intRoutine)(void)){
    this->pinA = pinA;
    this->pinB = pinB;
    this->counter = 0;
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(pinA), intRoutine , RISING);
}

long Encoder::read(){
    return counter;
}

long Encoder::readAndReset(){
    long ret_counter = counter;
    counter = 0;
    return ret_counter;
}

void Encoder::reset(){
    counter = 0;
}

void Encoder::inc(){
    if (digitalRead(pinB)==HIGH){
        counter++;
    } else {
        counter--;
    }
}

Encoder::~Encoder(){
    detachInterrupt(pinA);
}

