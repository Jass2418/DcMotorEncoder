#include "Arduino.h"
#include "DCEncoderPID.h"

double count = 0; //set the counts of the encoder
double angle = 0;//set the angles
boolean A,B;
byte state, statep;

double pwm = 11;// this is the PWM pin for the motor for how much we move it to correct for its error
const int dir1 = 4;//these pins are to control the direction of the motor (clockwise/counter-clockwise)
const int dir2 = 5;

double setpoint = 360;//I am setting it to move through 100 degrees
double Kp = 0.31;// you can set these constants however you like depending on trial & error
double Ki = 0.1;
double Kd = 0.3;

float last_error = 0;
float error = 0;
float changeError = 0;
float totalError = 0;
float pidTerm = 0;
float pidTerm_scaled = 0;// if the total gain we get is not in the PWM range we scale it down so that it's not bigger than |255|


DCEncoderPID R1(dir1,dir2,pwm,2,3);


void setup() {
    Serial.begin(9600);
    R1.setResolution(.04828);
    R1.setPID(Kp,Ki,Kd);
    R1.initialize();
    R1.setPosition(setpoint);
}

void loop(){
 R1.enable();
 R1.printInfo();
 delay(30);
}


