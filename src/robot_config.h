#ifndef ROBOT_CONFIGURE_H
#define ROBOT_CONFIGURE_H

// конфигурация датчиков и исполняющих устройств

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
void encodersInit();
void encodersReset();
void encoderResetLeft();
void encoderResetRigth();
long encoderReadLeft();
long encoderReadRigth();

void motorsInit();
void motorSetSpeedLeft(int speed);
void motorSetSpeedRigth(int speed);
void motorsSetSpeed(int leftSpeed, int rightSpeed);


#endif //ROBOT_CONFIGURE_H