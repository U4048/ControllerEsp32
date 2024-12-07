#include <Arduino.h>
#include "robot_config.h"
#include "pid_controller.h"
#include "AsyncStream.h"
#include "StringUtils.h"
#include "command.h"

AsyncStream<100> serial(&Serial, '\r');

int arg1 = 0;
int arg2 = 0;

void setup() {
  Serial.begin(BAUDRATE);
  serial.setTimeout(nextPID);	// установить другой таймаут
  serial.setEOL('\r');		// установить другой терминатор (EOL)
  motorsInit();
  encodersInit();
  resetPID();
}

void runCommand(const char* buf){
  Text parser(buf);
  char cmd = parser.getSub(0, CHAR_DIV).trim().charAt(0);
  if (cmd=='\0') return;
  Serial.print("command : ");
  Serial.println(buf);
  switch (cmd) {
    case PING: 
      Serial.println(parser.getSub(1, CHAR_DIV).toInt16());
      break;
    case READ_ENCODERS:
      Serial.print(encoderReadLeft());
      Serial.print(" ");
      Serial.println(encoderReadRigth());
    break;
   case RESET_ENCODERS:
      encodersReset();
      resetPID();
      Serial.println("OK");
    break;
    case MOTOR_SPEEDS:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    arg1 = parser.getSub(1, CHAR_DIV).toInt16();
    arg2 = parser.getSub(2, CHAR_DIV).toInt16();
    if (arg1 == 0 && arg2 == 0) {
      motorsSetSpeed(0,0);
      resetPID();
      moving = 0;
    }
    else moving = 1;
    leftPID.TargetTicksPerFrame = arg1;
    rightPID.TargetTicksPerFrame = arg2;
    Serial.println("OK"); 
    break;
  case MOTOR_RAW_PWM:
    arg1 = parser.getSub(1, CHAR_DIV).toInt16();
    arg2 = parser.getSub(2, CHAR_DIV).toInt16();
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    resetPID();
    moving = 0; // Sneaky way to temporarily disable the PID
    motorsSetSpeed(arg1, arg2);
    Serial.println("OK"); 
    break;
  default:
    Serial.println("Invalid Command");
    break;
  }
}

void loop() {
  
  if (serial.available()) {
    if (strlen(serial.buf)>0) {
      Serial.println(strlen(serial.buf));
      runCommand(serial.buf);
      Serial.println("read");
    }
  }
   if (millis() > nextPID) {
    updatePID();
    nextPID += PID_INTERVAL;
  }
  
  // Check to see if we have exceeded the auto-stop interval
  //if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {;
  //  motorsSetSpeed(0, 0);
  //  moving = 0;
  //}  
}

