#include "motor_driver.h"

Motor::Motor(){}
void Motor::setup(uint8_t pinA, uint8_t pinB, uint8_t chanA, uint8_t chanB){
  this->pinA = pinA;
  this->pinB = pinB;
  this->chanA = chanA;
  this->chanB = chanB;

  ledcSetup(chanA, 5000, 8);
  ledcSetup(chanB, 5000, 8);
  
  ledcAttachPin(pinA, chanA);
  ledcAttachPin(pinB, chanB);
}

void Motor::forward(int speed){
  ledcWrite(chanA, speed);
  ledcWrite(chanB, 0);
}

void Motor::reverse(int speed){
  ledcWrite(chanA, 0);
  ledcWrite(chanB, speed);
}

void Motor::stop(){
  ledcWrite(chanA, 0);
  ledcWrite(chanB, 0);
}

Motor::~Motor(){}
