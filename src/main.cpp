#include <Arduino.h>
#include "robot_config.h"
#include "pid_controller.h"
#include "StringUtils.h"
#include "command.h"

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd[100];
int cmd_index  =0;
static bool ledState = false; 

int arg1 = 0;
int arg2 = 0;



void setup() {
  Serial.begin(BAUDRATE);
  motorsInit();
  encodersInit();
  resetPID();
  pinMode(LED_BUILTIN, OUTPUT);
}

void runCommand(const char* buf){
  Text parser(buf);
  char cmd = parser.getSub(0, CHAR_DIV).trim().charAt(0);
  if (cmd=='\0') return;
  //Serial.print("command : ");
  //Serial.println(buf);
  switch (cmd) {
    case GET_BAUDRATE:
      Serial.println(BAUDRATE);
    break;
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
  case UPDATE_PID:
    Kp = parser.getSub(1, CHAR_DIV).toInt16();
    Kd = parser.getSub(2, CHAR_DIV).toInt16();
    Ki = parser.getSub(3, CHAR_DIV).toInt16();
    Ko = parser.getSub(4, CHAR_DIV).toInt16();
    Serial.println("OK");
    break;  
  default:
    Serial.println("Invalid Command");
    break;
  }
}

void resetCommand() {
  cmd_index = 0;
  cmd[0] = '\0' ;
}


void loop() {
  
  if (Serial.available()>0) {
    chr = Serial.read();
    if (chr == 13){
      cmd[cmd_index]='\0';
      runCommand(cmd);
      resetCommand();
      if(ledState){
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
      ledState = !ledState;
    }
    else {
      cmd[cmd_index] = chr;
      cmd_index++;
    }
  }
  if (millis() > nextPID) {
    updatePID();
    nextPID += PID_INTERVAL;
  }
  
  // Check to see if we have exceeded the auto-stop interval
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {;
    motorsSetSpeed(0, 0);
    moving = 0;
  }  
}

