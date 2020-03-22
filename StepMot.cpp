#include "StepMot.h"

StepMot::StepMot(float steps_per_revolution, uint8_t microsteps, uint8_t step_pin, uint8_t dir_pin, uint8_t en_pin):
  _stepsPerRevolution(steps_per_revolution),
  _microsteps(microsteps),
  _stepPin(step_pin),
  _dirPin(dir_pin),
  _enPin(en_pin),
  _anglePerStep(360.0 / _stepsPerRevolution / _microsteps),
  _stepsPerAngle(_stepsPerRevolution * _microsteps / 360.0)
{
  pinMode(_stepPin, OUTPUT);
  pinMode(_dirPin, OUTPUT);
  pinMode(_enPin, OUTPUT);

  digitalWrite(_enPin, 1);
  if (_inverted) digitalWrite(_dirPin, 0);
  else digitalWrite(_dirPin, 1);
  digitalWrite(_stepPin, 0);
}

void StepMot::setRPM(float rpm) {
  if(rpm <= 0) {
    _targetSteps = 0;
    _stopped = 1;
    return;
  }
  else {
    _stopped = 0;
    _stepPeriod = 1000000.0 / (_stepsPerRevolution * _microsteps * rpm / 60.0);
  }
}

void StepMot::enable() {
  if (!_enabled) {
    digitalWrite(_enPin, 0);
    _enabled = 1;
  }
}

void StepMot::disable() {
  if (_enabled) {
    digitalWrite(_enPin, 1);
    _enabled = 0;
  }
}

void StepMot::setMode(bool mode) {
  _mode = mode;
}

void StepMot::setSteps(uint32_t steps) {
  _targetSteps = steps;
  _moving = true;
}

void StepMot::rotate(bool dir) {
  StepMot::setDir(dir);
  _targetSteps = 1;
  StepMot::move();
}

void StepMot::resetPos(){
  _targetSteps = 0;
  _currentSteps = 0;
}

void StepMot::setAngle(float newAngle) {
  if (_mode == ABSOLUTE) {
    if (newAngle > StepMot::getAngle()) {
      _targetAngle = newAngle - StepMot::getAngle();
      StepMot::setDir(CW);
    }
    else if (newAngle < StepMot::getAngle()) {
      _targetAngle = StepMot::getAngle() - newAngle;
      StepMot::setDir(CCW);
    }
    else {
      _targetAngle = 0;
    }
    _targetSteps = round(_targetAngle * _stepsPerAngle);
  }
  else if (_mode == RELATIVE) {
    _targetSteps = round(abs(newAngle * _stepsPerAngle));
    if (newAngle > 0) StepMot::setDir(CW);
    else if (newAngle < 0) StepMot::setDir(CCW);
  }
  StepMot::setSteps(_targetSteps);
}

float StepMot::getAngle(){
  return _currentSteps * _anglePerStep; // current Angle
}

void StepMot::setDir(bool dir) {
  if(_dir != dir){
    _dir = dir;
    if (_inverted) digitalWrite(_dirPin, !_dir);
    else digitalWrite(_dirPin, _dir);
    _dir ? _stepCounter = 1 : _stepCounter = -1;
  }
}

void StepMot::invertDir(bool invertState) {
  _inverted = invertState;
}

bool StepMot::step() {
  if(_stopped) return 0;
  if (_targetSteps > 0) {
    if(_autoPower && !_enabled) StepMot::enable();
    digitalWrite(_stepPin, HIGH);
    digitalWrite(_stepPin, LOW);
    _targetSteps--;
    _currentSteps += _stepCounter;
    return 1;
  }
  else {
    if(_autoPower && _enabled) StepMot::disable();
    return 0;
  }
}

void StepMot::autoPower(bool status){
  _autoPower = status;
}

bool StepMot::move() {
  if (micros() - _prevStepTime >= _stepPeriod) {
    _prevStepTime = micros();
    _moving = StepMot::step();
  }
  return _moving;
}
