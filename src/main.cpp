#include <Arduino.h>

struct MotorInfo
{
   byte pinIN;
   byte pinPWN;
};

MotorInfo motorLeft {26, 25};
MotorInfo motorRigth {13, 12};

void motorInit(MotorInfo motor){
  pinMode(motor.pinIN, OUTPUT);
  pinMode(motor.pinPWN, OUTPUT);
  
  digitalWrite(motor.pinIN, 0);
  digitalWrite(motor.pinPWN, 0);
}

void motorControl(int val, MotorInfo motor) {
  if (val > 0){
   //Вперед 
    analogWrite(motor.pinPWN, val);
    digitalWrite(motor.pinIN, 0); 
  } else if (val < 0) {
    //Назад
    analogWrite(motor.pinPWN, 255+val);
    digitalWrite(motor.pinIN, 1);
  } else {
    //Стоп
    digitalWrite(motor.pinIN, 0);
    analogWrite(motor.pinPWN, 0);
  }
}

void motorControlLeft(int val){
  motorControl(val, motorLeft);
}

void motorControlRight(int val){
  motorControl(val, motorRigth);
}


void setup() {
  motorInit(motorLeft);
  motorInit(motorRigth);
  Serial.begin(9600);
}

void loop() {
    Serial.println("1");
    motorControlLeft(255);
    delay(2000);
    Serial.println("-1");
    motorControlLeft(-255);
    delay(2000);   
    Serial.println("0");
    motorControlLeft(0);
    delay(2000);  

    Serial.println("1");
    motorControlRight(255);
    delay(2000);
    Serial.println("-1");
    motorControlRight(-255);
    delay(2000);   
    Serial.println("0");
    motorControlRight(0);
    delay(2000);  
    
    
}

