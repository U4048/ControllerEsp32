#include <Arduino.h>
#include "robot_config.h"

void setup() {
  Serial.begin(115200);
  motorsInit();
  encodersInit();
}

void printEncoders(){
   Serial.print(encoderReadLeft());
   Serial.print(" ");
   Serial.println(encoderReadRigth());
}
void loop() {

    
    motorsSetSpeed(-255, 255);
    encodersReset();
    delay(2000);  
    printEncoders();
    
    motorsSetSpeed(255, -255);
    encodersReset();
    delay(2000);  
    printEncoders();
        
    motorsSetSpeed(255, 0);
    encodersReset();
    delay(2000);  
    printEncoders();
    
    motorsSetSpeed(0, 255);
    encodersReset();
    delay(2000);  
    printEncoders();

    motorsSetSpeed(0, 0);
    encodersReset();
    delay(2000);  
    printEncoders();

}

