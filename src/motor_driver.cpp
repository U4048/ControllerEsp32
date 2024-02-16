#include "motor_driver.h"

Motor::Motor(){}
void Motor::setup(uint8_t pinIN, uint8_t pinPWM){
  this->pinIN = pinIN;
  this->pinPWM = pinPWM;

  pinMode(pinIN, OUTPUT);
  pinMode(pinPWM, OUTPUT);
  
  digitalWrite(pinIN, 0);
  digitalWrite(pinPWM, 0);
}

void Motor::setSpeed(int speed){
  if (speed > 0){
   //Вперед 
    analogWrite(pinPWM, speed);
    digitalWrite(pinIN, 0); 
  } else if (speed < 0) {
    //Назад
    analogWrite(pinPWM, 255+speed);
    digitalWrite(pinIN, 1);
  } else {
    //Стоп
    digitalWrite(pinIN, 0);
    analogWrite(pinPWM, 0);
  }   
}

Motor::~Motor(){}
