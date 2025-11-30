#ifndef ROBOT_CONFIGURE_H
#define ROBOT_CONFIGURE_H

// transport config
#define  BAUDRATE 115200
#define  CHAR_DIV ' '
// конфигурация датчиков и исполняющих устройств

//configure pins
#define encoderRigthPinA 34
#define encoderRigthPinB 35
#define encoderLeftPinA 39
#define encoderLeftPinB 36

#define motorRigthPinA 13
#define motorRigthPinB 12
#define motorLeftPinA 25
#define motorLeftPinB 26

// Channels
#define motorRigthPwmChannelA 0
#define motorRigthPwmChannelB 1
#define motorLeftPwmChannelA 2
#define motorLeftPwmChannelB 3

#define MAX_PWM 255
#define MIN_PWM 80


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
int getRealPWM(int speed);


#endif //ROBOT_CONFIGURE_H