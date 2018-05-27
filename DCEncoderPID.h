#ifndef DCEncoderPID_H
#define DCEncoderPID_H
#include <Arduino.h>

class DCEncoderPID
{
    public:
        DCEncoderPID(int dirL,int dirR,int pwm,int aPin, int bPin);
        void setResolution(double resolution);
        void setPID(double Kp,double Ki, double Kd);
        void setPosition(int angle);
        void initialize();
        void enable();
        void printInfo();
        
    private:
        double _resolution;
        double _count; //set the counts of the encoder
        double _angle;//set the angles
        boolean _A,_B;
        byte _state, _statep;

        double _pwm;// this is the PWM pin for the motor for how much we move it to correct for its error
        int _dir1;//these pins are to control the direction of the motor (clockwise/counter-clockwise)
        int _dir2;
        int _Apin;
        int _Bpin;

        double _setpoint;//I am setting it to move through 100 degrees
        double _Kp;// you can set these constants however you like depending on trial & error
        double _Ki;
        double _Kd;

        float _last_error = 0;
        float _error = 0;
        float _changeError = 0;
        float _totalError = 0;
        float _pidTerm = 0;
        float _pidTerm_scaled = 0;// if the total gain we get is not in the PWM range we scale it down so that it's not bigger than |255|

        void PIDCalculation();
        void Achange();
        void Bchange();
        void moveMotor();

};



#endif