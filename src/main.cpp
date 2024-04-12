#include <Arduino.h>
#include "robot_config.h"

#include <ros.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/Twist.h>
#include <WiFiManager.h>
#include <QuickPID.h>

//#define PID_TUNING


unsigned long loopTimer;


WiFiClient client;
IPAddress server(10, 0, 1, 181);


class WiFiHardware {

  public:
  WiFiHardware() {};

  void init() {
    // do your initialization here. this probably includes TCP server/client setup
    client.connect(server, 11411);
  }

  // read a byte from the serial port. -1 = failure
  int read() {
    // implement this method so that it reads a byte from the TCP connection and returns it
    //  you may return -1 is there is an error; for example if the TCP connection is not open
    return client.read();         //will return -1 when it will works
  }

  // write data to the connection to ROS
  void write(uint8_t* data, int length) {
    // implement this so that it takes the arguments and writes or prints them to the TCP connection
    for(int i=0; i<length; i++)
      client.write(data[i]);
  }

  // returns milliseconds since start of program
  unsigned long time() {
     return micros(); // easy; did this one for you
  }
};


ros::NodeHandle_<WiFiHardware> nh;
geometry_msgs::Vector3Stamped speedMsg;



float VelLeft = 0;
float VelRight = 0;
int spwmVelLeft=0; 
int spwmVelRight=0;

float cmdSpeedLeft = 0;
float cmdSpeedRight = 0;
float encSpeedLeft = 0;
float encSpeedRight = 0;

unsigned long prevTime=0;


float kpRight = 11.81, kiRight = 184.57, kdRight = 0.03;
QuickPID pidRigth(&encSpeedRight, &VelRight, &cmdSpeedRight,kpRight, kiRight, kdRight, 0.01, 0, QuickPID::DIRECT);

float kpLeft = 1000, kiLeft = 184.57, kdLeft = 0.03;
QuickPID pidLeft(&encSpeedLeft, &VelLeft, &cmdSpeedLeft, kpLeft, kiLeft, kdLeft, 0.01, 0, QuickPID::DIRECT);

void cmdVelCallBack(const geometry_msgs::Twist& twist) {
  //Расчет скорости с учетом искревления
  cmdSpeedLeft = twist.linear.x - twist.angular.z * L_BASE;
  cmdSpeedRight = twist.linear.x + twist.angular.z * L_BASE;
}

void printCmdVel(){
  Serial.print("cmd ");
  Serial.print(VelLeft);
  Serial.print(" : ");
  Serial.print(encSpeedLeft);
  Serial.print(" : ");
  Serial.print(VelRight);
  Serial.print(" : ");
  Serial.print(encSpeedRight);
  Serial.print(" : ");
  Serial.print(cmdSpeedLeft);
  Serial.print(" : ");
  Serial.println(cmdSpeedRight);
  
}

int getPwm(double speed){
  int pwm = round(speed);
  if (pwm > 255) return 255;
  if (pwm < -255) return -255;
  return pwm;
}

void setLeftMotorPwm(){
   int pwmVelLeft = getPwm(VelLeft);
  if (pwmVelLeft != spwmVelLeft) {
    motorSetSpeedLeft(pwmVelLeft);
    spwmVelLeft = pwmVelLeft;
  }
}  

void setRightMotorPwm(){
  int pwmVelRight =getPwm(VelRight);
  if ( pwmVelRight != spwmVelRight) {
    motorSetSpeedRigth(pwmVelRight);
    spwmVelRight = pwmVelRight;
  }
}  

void setMotorPwm(){
   int pwmVelLeft = getPwm(VelLeft);
   int pwmVelRight =getPwm(VelRight);
  if (pwmVelLeft != spwmVelLeft || pwmVelRight != spwmVelRight) {
    motorsSetSpeed(pwmVelLeft, pwmVelRight);
    spwmVelLeft = pwmVelLeft;
    spwmVelRight = pwmVelRight;
  }
}  

ros::Subscriber<geometry_msgs::Twist> subCmdVel("cmd_vel", cmdVelCallBack);
ros::Publisher speedPub("speed", &speedMsg); 
#ifdef PID_TUNING
bool pidLoop = false;
void pidSetupTuning(){
  pidLeft.AutoTune(tuningMethod::ZIEGLER_NICHOLS_PID);
  pidLeft.autoTune->autoTuneConfig(1, 0.0001, 0.7, 255, QuickPID::DIRECT, 1, LOOP_TIME/2);
}

float avg(float inputVal) {
  static float arrDat[16];
  static int pos;
  static float sum;
  pos++;
  if (pos >= 16) pos = 0;
  sum = sum - arrDat[pos] + inputVal;
  arrDat[pos] = inputVal;
  return sum / 16.0;
}

void pidLeftTuning(){
int pwmVelLeft = 0;
if (pidLeft.autoTune) // Avoid dereferencing nullptr after _myPID.clearAutoTune()
  {
    switch (pidLeft.autoTune->autoTuneLoop()) {
      case pidLeft.autoTune->AUTOTUNE:
         //Serial.println("AUTOTUNE");
         encSpeedLeft  = encoderReadPosLeft()*ENCODE_DELTA*(1000000/(time()-prevTime));
         prevTime = time();
         cmdSpeedLeft = encSpeedLeft;
         //pidLeft.Compute();
         setLeftMotorPwm();
        break;
      case pidLeft.autoTune->TUNINGS:
        Serial.println("TUNINGS");
        pidLeft.autoTune->setAutoTuneConstants(&kpLeft, &kiLeft, &kdLeft); // set new tunings
        pidLeft.SetMode(QuickPID::AUTOMATIC); // setup PID
        pidLeft.SetSampleTimeUs(LOOP_TIME);
        pidLeft.SetTunings(kpLeft, kiLeft, kdLeft, 0.0001, 0); // apply new tunings to PID
        motorSetSpeedLeft(0);
        break;
      case pidLeft.autoTune->CLR:
        if (!pidLoop) {
          pidLeft.clearAutoTune(); // releases memory used by AutoTune object
          pidLoop = true;
          //Tu: 0.02  td: 0.01  Ku: 20.18  Kp: 6.72  Ki: 107.56  Kd: 0.03
          //Tu: 0.03  td: 0.01  Ku: 19.68  Kp: 6.55  Ki: 62.92  Kd: 0.04
          //Tu: 0.02  td: 0.01  Ku: 19.68  Kp: 6.55  Ki: 104.84  Kd: 0.02
          //Tu: 0.03  td: 0.01  Ku: 19.08  Kp: 11.45  Ki: 134.20  Kd: 0.04
          //Tu: 0.02  td: 0.01  Ku: 19.68  Kp: 11.81  Ki: 184.57  Kd: 0.03
          //Tu: 0.01  td: 0.02  Ku: 9.63  Kp: 5.78  Ki: 270.45  Kd: 0.00
          //Tu: 0.01  td: 0.01  Ku: 9.63  Kp: 5.78  Ki: 270.62  Kd: 0.00
        }
        break;
    }
  }
}
#endif

//Публикация скорости в m/s топик.
void publishSpeed(double time){
  speedMsg.header.stamp = nh.now(); //timestamp 
  speedMsg.vector.x = encSpeedLeft;
  speedMsg.vector.y = encSpeedRight;
  speedMsg.vector.z = time;
  speedPub.publish(&speedMsg);
}

void setup() {
  Serial.begin(115200);
  motorsInit();
  encodersInit();
  motorsSetSpeed(0, 0);
  encodersReset();
  loopTimer = time() + LOOP_TIME;

  //pidRigth.SetTunings(kpRight, kiRight, kdRight);
  pidRigth.SetMode(QuickPID::TIMER);
  pidRigth.SetSampleTimeUs(LOOP_TIME);
  pidRigth.SetOutputLimits(-255, 255);

  //pidLeft.SetTunings(kpLeft, kiLeft, kdLeft);
  pidLeft.SetMode(QuickPID::TIMER);
  pidLeft.SetSampleTimeUs(LOOP_TIME);
  pidLeft.SetOutputLimits(-255, 255);
  
  #ifdef PID_TUNING
    pidSetupTuning();
  #else //PID_TUNING
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...");
        Serial.println(WiFi.localIP().toString());
        delay(2000);
        nh.initNode();
        nh.advertise(speedPub);
        bool sub = false;
        while (!sub)
        {
          Serial.println("subscribe...");
          sub = nh.subscribe(subCmdVel);
          delay(1000);
        };
    }
  #endif //PID_TUNING
  prevTime = time();
}

void loop() {
  
  #ifdef PID_TUNING
   pidLeftTuning();
  #else //PID_TUNING
  encSpeedLeft  = encoderReadPosLeft()*ENCODE_DELTA*(1000000/LOOP_TIME);
  encSpeedRight = encoderReadPosRigth()*ENCODE_DELTA*(1000000/LOOP_TIME);
  pidLeft.Compute();
  pidRigth.Compute();
  setMotorPwm();
  printCmdVel();
  publishSpeed(LOOP_TIME/1000000);
  nh.spinOnce();
  while(loopTimer > time()){;}
    loopTimer += LOOP_TIME;
  #endif //PID_TUNING


}

