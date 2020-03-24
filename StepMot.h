#pragma once

/*
  Arduino library for stepper motor using STEP/DIR
  Github: https://github.com/VICLER/StepMot
  v 1.0

  by VICLER
*/

#include <Arduino.h>


enum MODE {
  RELATIVE,
  ABSOLUTE
};

enum DIR {
  CCW,
  CW
};

enum STATE {
  OFF,
  ON
};

class StepMot
{
  public:
    StepMot(float steps_per_revolution, uint8_t microsteps, uint8_t step_pin, uint8_t dir_pin, uint8_t en_pin);
    void setMode(bool mode);
    void autoPower(bool status);
    void setRPM(float rpm);
    void enable();
    void disable();
    void setDir(bool dir);
    void invertDir(bool invertState);
    void setSteps(uint32_t steps);
    void setAngle(float newAngle);
    void rotate(bool dir);
    float getAngle();
    void resetPos();
    bool move();
    void step();

  private:
    const float _stepsPerRevolution = 0.0;
    const uint8_t _microsteps = 1;
    const float _stepsPerAngle = 0.0;
    const float _anglePerStep = 0.0;
    const uint8_t _stepPin;
    const uint8_t _dirPin;
    const uint8_t _enPin;
    uint16_t _stepPeriod = 1000;
    uint32_t _prevStepTime = 0;
    uint32_t _targetSteps = 0;
    int32_t _currentSteps = 0;
    int8_t _stepCounter = 1;
    float _targetAngle = 0.0;
    bool _mode = RELATIVE;
    bool _autoPower = false;
    bool _inverted = false;
    bool _enabled = false;
    bool _moving = false;
    bool _dir = CW;
};
