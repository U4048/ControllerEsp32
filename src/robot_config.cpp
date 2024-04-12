#include "robot_config.h"
#include "encoder_driver.h"
#include "motor_driver.h"

Motor* motorRigth;
Motor* motorLeft;

Encoder* encoderRigth;
Encoder* encoderLeft;

void IRAM_ATTR encoderRigthInterrupt(){
    if (encoderRigth !=NULL) encoderRigth->inc();
}

void IRAM_ATTR encoderLeftInterrupt(){
    if (encoderLeft != NULL) encoderLeft->inc();
}

unsigned long time(){
     return micros(); // easy; did this one for you
}

void encodersInit(){
  encoderRigth = new Encoder();
  encoderLeft = new Encoder();
  encoderRigth->setup(encoderRigthPinA, encoderRigthPinB, encoderRigthInterrupt);
  encoderLeft->setup(encoderLeftPinA, encoderLeftPinB, encoderLeftInterrupt);
}

void encodersReset(){
    encoderResetRigth();
    encoderResetLeft();
}

void encoderResetLeft(){
   if (encoderLeft != NULL) encoderLeft->reset();
}
void encoderResetRigth(){
   if (encoderRigth != NULL) encoderRigth->reset();
}
long encoderReadLeft(){
   if (encoderLeft != NULL) return encoderLeft->read(); 
   else return 0;
}
long encoderReadRigth(){
   if (encoderRigth != NULL) return encoderRigth->read(); 
   else return 0;
}
long encoderReadPosLeft() {
   if (encoderLeft != NULL) return encoderLeft->readAndReset(); 
   else return 0;
}
long encoderReadPosRigth() {
   if (encoderRigth != NULL) return encoderRigth->readAndReset(); 
   else return 0;
}

void motorsInit(){
motorRigth = new Motor();
motorLeft = new Motor();
motorRigth->setup(motorRigthPinIN, motorRigthPinPWM);
motorLeft->setup(motorLeftPinIN, motorLeftPinPWM);
}
void motorSetSpeedLeft(int speed){
    if (motorLeft != NULL) motorLeft->setSpeed(speed);
}
void motorSetSpeedRigth(int speed){
    if (motorRigth != NULL) motorRigth->setSpeed(speed);
}
void motorsSetSpeed(int leftSpeed, int rightSpeed){
  motorSetSpeedLeft(leftSpeed);
  motorSetSpeedRigth(rightSpeed);
}