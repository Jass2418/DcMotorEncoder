#include "Arduino.h"
#include "DCEncoderPID.h"


double pwm = 11;// this is the PWM pin for the motor for how much we move it to correct for its error
double pwm2 = 12;
double pwm3 = 13;

const int dir1 = 4;//these pins are to control the direction of the motor (clockwise/counter-clockwise)
const int dir2 = 5;
const int dir3 = 6;
const int dir4 = 7;
const int dir5 = 8;
const int dir6 = 9;

double setpoint = 360;//I am setting it to move through 100 degrees
double Kp = 2;// you can set these constants however you like depending on trial & error
double Ki = 2;
double Kd = 2;

SerialManager serialManager;


DCEncoderPID R1(dir1,dir2,pwm,2,3);
DCEncoderPID R2(dir4,dir3,pwm2,18,19);
DCEncoderPID R3(dir5,dir6,pwm3,20,21);



void intR1A(){ 
    R1.Achange(); 
}
void intR1B(){ 
    R1.Bchange(); 
}
void intR2A(){ 
    R2.Achange(); 
}
void intR2B(){ 
    R2.Bchange(); 
}
void intR3A(){ 
    R3.Achange(); 
}
void intR3B(){ 
    R3.Bchange(); 
}


void setup() {
    Serial.begin(9600);
    InitR1();
    InitR2();
    InitR3();
}

void loop(){

    R1.enable();
    R2.enable();
    R3.enable();
    if(serialManager.isComplete)
   {
      serialManager.isComplete=false;
      byte buffer[BufferSize];
      serialManager.ReturnInfo(buffer);
      MoveRobot(buffer);
   }
}

void serialEvent() {
  while(Serial.available())
  {
    byte rxByte=(byte) Serial.read();
    serialManager.Process(rxByte);
  }
}
void MoveRobot(byte buffer[BufferSize])
{

    int dirR1=buffer[0];
    int stepR1= (buffer[1]<<8)+buffer[2];
    MoveR1(dirR1,stepR1);

    int dirR2=buffer[3];
    int stepR2=(buffer[4]<<8)+buffer[5];
    MoveR2(dirR2,stepR2);

    int dirP1=buffer[6];
    int stepP1=(buffer[7]<<8)+buffer[8];
    MoveR3(dirP1,stepP1);

}

void MoveR1(int dir, int step)
{
    if(dir==1) step=-1*step;
    else step=step;
    R1.move(step);
}

void MoveR2(int dir, int step)
{
  if(dir==1) step=-1*step;
  else step=step;
  R2.move(step);
}

void MoveR3(int dir, int step)
{
  if(dir==1) step=-1*step;
  else step=step;
  R2.move(step);
}

void InitR1(){
    R1.setResolution(.04828);
    R1.setPID(Kp,Ki,Kd);
    R1.initialize();
    attachInterrupt(digitalPinToInterrupt(R1.getApin()),intR1A,CHANGE);
    attachInterrupt(digitalPinToInterrupt(R1.getBpin()),intR1B,CHANGE);
    R1.setPosition(0,2);
}

void InitR2(){
    R2.setResolution(.23);
    R2.setPID(2,2,2);
    R2.initialize();
    attachInterrupt(digitalPinToInterrupt(R2.getApin()),intR2A,CHANGE);
    attachInterrupt(digitalPinToInterrupt(R2.getBpin()),intR2B,CHANGE);
    R2.setPosition(0,23);
}

void InitR3(){
    R3.setResolution(0.3214);
    R3.setPID(0.3,0.1,0.5);
    R3.initialize();
    attachInterrupt(digitalPinToInterrupt(R3.getApin()),intR3A,CHANGE);
    attachInterrupt(digitalPinToInterrupt(R3.getBpin()),intR3B,CHANGE);
    R3.setPosition(0,2);
}