#include "Arduino.h"
#include "DCEncoderPID.h"


double pwm = 11;// this is the PWM pin for the motor for how much we move it to correct for its error
const int dir1 = 4;//these pins are to control the direction of the motor (clockwise/counter-clockwise)
const int dir2 = 5;

double setpoint = 360;//I am setting it to move through 100 degrees
double Kp = 2;// you can set these constants however you like depending on trial & error
double Ki = 2;
double Kd = 2;



DCEncoderPID R1(dir1,dir2,pwm,2,3);

void intR1A(){ 
    R1.Achange(); 
}
void intR1B(){ 
    R1.Bchange(); 
}

void setup() {
    Serial.begin(9600);
    R1.setResolution(.04828);
    R1.setPID(Kp,Ki,Kd);
    R1.initialize();
    attachInterrupt(digitalPinToInterrupt(R1.getApin()),intR1A,CHANGE);
    attachInterrupt(digitalPinToInterrupt(R1.getBpin()),intR1B,CHANGE);
    R1.setPosition(360,2);
}

void loop(){
 R1.enable();
 R1.printInfo();

}


