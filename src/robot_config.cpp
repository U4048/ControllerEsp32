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

void motorsInit(){
motorRigth = new Motor();
motorLeft = new Motor();
motorRigth->setup(motorRigthPinA, motorRigthPinB, motorRigthPwmChannelA, motorRigthPwmChannelB);
motorLeft->setup(motorLeftPinA, motorLeftPinB, motorLeftPwmChannelA, motorLeftPwmChannelB);
}

void motorSetSpeedLeft(int speed){
    if (motorLeft != NULL) {
        if(speed > 0) {
            motorLeft->forward(speed);
        } else {
            motorLeft->reverse(speed *-1);
        }
    }
}

void motorSetSpeedRigth(int speed){
    if (motorRigth != NULL) {
        if(speed > 0) {
            motorRigth->forward(speed);
        } else {
            motorRigth->reverse(speed *-1);
        }
    }
}

void motorsSetSpeed(int leftSpeed, int rightSpeed){
  motorSetSpeedLeft(leftSpeed);
  motorSetSpeedRigth(rightSpeed);
}