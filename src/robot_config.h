#ifndef ROBOT_CONFIGURE_H
#define ROBOT_CONFIGURE_H

// конфигурация датчиков и исполняющих устройств

const double L_BASE = 0.021; //растояние до центра бота m
const int ENCODER_TIKS = 330; //тиков на оборот
const double D_VEL = 0.085; //диаметр колеса m
const double ENCODE_DELTA = D_VEL*3.14/ENCODER_TIKS; //distance vel/tik m
#define LOOP_TIME 50000  // loop time in us. 50ms = 20Hz

//configure pins
#define encoderRigthPinA 34
#define encoderRigthPinB 35
#define encoderLeftPinA 39
#define encoderLeftPinB 36

#define motorRigthPinIN 13
#define motorRigthPinPWM 12
#define motorLeftPinIN 26
#define motorLeftPinPWM 25


// проксирующие функции
unsigned long time();
void encodersInit();
void encodersReset();
void encoderResetLeft();
void encoderResetRigth();
long encoderReadLeft();
long encoderReadRigth();
long encoderReadPosLeft();
long encoderReadPosRigth();


void motorsInit();
void motorSetSpeedLeft(int speed);
void motorSetSpeedRigth(int speed);
void motorsSetSpeed(int leftSpeed, int rightSpeed);


#endif //ROBOT_CONFIGURE_H