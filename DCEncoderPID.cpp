#include "Arduino.h"
#include "DCEncoderPID.h"

//Constructors
DCEncoderPID::DCEncoderPID(int dirL,int dirR,int pwm,int aPin, int bPin){
    _dir1=dirL;
    _dir2=dirR;
    _pwm=pwm;
    _Apin=aPin;
    _Bpin=bPin;
    _resolution=0;
    _count=0;
    _angle=0;
    _tolerance=0;
}
///Public Methods

void DCEncoderPID::initialize(){
//   attachInterrupt(digitalPinToInterrupt(_Apin),(void*)&DCEncoderPID::Achange,CHANGE);//interrupt pins for encoder
//   attachInterrupt(digitalPinToInterrupt(_Bpin),(void*)&DCEncoderPID::Bchange,CHANGE); 
  pinMode(_pwm, OUTPUT);
  pinMode(_dir1, OUTPUT);
  pinMode(_dir2, OUTPUT);
}

void DCEncoderPID::setResolution(double resolution)
{
    _resolution=resolution;
}

void DCEncoderPID::setPID(double Kp,double Ki, double Kd){
    _Kp=Kp;
    _Ki=Ki;
    _Kd=Kd;
}

void DCEncoderPID::setPosition(int setpoint){
    _setpoint=setpoint;
}

void DCEncoderPID::setPosition(int setpoint , double tolerance){
    _setpoint=setpoint;
    _tolerance=tolerance;

}

void DCEncoderPID::enable(){
    PIDCalculation();
    moveMotor();
}

void DCEncoderPID::printInfo(){
  Serial.print("Angle: "); Serial.println(_angle);
  Serial.print("count: "); Serial.print(_count);

}

int DCEncoderPID::getApin(){
    return _Apin;
}

int DCEncoderPID::getBpin(){
    return _Bpin;
}

///Private Methods

void DCEncoderPID::PIDCalculation(){
  _angle = (_resolution * _count);//_count to angle conversion
  _error = _setpoint - _angle;
  
  _changeError = _error - _last_error; // derivative term
  _totalError += _error; //accumalate errors to find integral term
  _pidTerm = (_Kp * _error) + (_Ki * _totalError) + (_Kd * _changeError);//total gain
  _pidTerm = constrain(_pidTerm, -255, 255);//constraining to appropriate value
  _pidTerm_scaled = abs(_pidTerm);//make sure it's _A positive value

  _last_error = _error;
}

void DCEncoderPID::Achange(){
  _A = digitalRead(_Apin);
  _B = digitalRead(_Bpin);

  if ((_A==HIGH)&&(_B==HIGH)) _state = 1;
  if ((_A==HIGH)&&(_B==LOW)) _state = 2;
  if ((_A==LOW)&&(_B==LOW)) _state = 3;
  if((_A==LOW)&&(_B==HIGH)) _state = 4;
  switch (_state)
  {
    case 1:
    {
      if (_statep == 2) _count++;
      if (_statep == 4) _count--;
      break;
    }
    case 2:
    {
      if (_statep == 1) _count--;
      if (_statep == 3) _count++;
      break;
    }
    case 3:
    {
      if (_statep == 2) _count --;
      if (_statep == 4) _count ++;
      break;
    }
    default:
    {
      if (_statep == 1) _count++;
      if (_statep == 3) _count--;
    }
  }
  _statep = _state;
}

void DCEncoderPID::Bchange(){
  _A = digitalRead(_Apin);
  _B = digitalRead(_Bpin);

  if ((_A==HIGH)&&(_B==HIGH)) _state = 1;
  if ((_A==HIGH)&&(_B==LOW)) _state = 2;
  if ((_A==LOW)&&(_B==LOW)) _state = 3;
  if((_A==LOW)&&(_B==HIGH)) _state = 4;
  switch (_state)
  {
    case 1:
    {
      if (_statep == 2) _count++;
      if (_statep == 4) _count--;
      break;
    }
    case 2:
    {
      if (_statep == 1) _count--;
      if (_statep == 3) _count++;
      break;
    }
    case 3:
    {
      if (_statep == 2) _count --;
      if (_statep == 4) _count ++;
      break;
    }
    default:
    {
      if (_statep == 1) _count++;
      if (_statep == 3) _count--;
    }
  }
  _statep = _state;
}

void DCEncoderPID::moveMotor(){
  
  if(_angle <=_setpoint+_tolerance && _angle>= _setpoint-_tolerance){
    digitalWrite(_dir1, LOW);//No motion
    digitalWrite(_dir2, LOW);
  }

  else if (_angle < _setpoint) {
    digitalWrite(_dir1, HIGH);// Forward motion
    digitalWrite(_dir2, LOW);
  } 
  else if(_angle > _setpoint) {
    digitalWrite(_dir1, LOW);//Reverse motion
    digitalWrite(_dir2, HIGH);
  }

  analogWrite(_pwm, _pidTerm_scaled);
}

