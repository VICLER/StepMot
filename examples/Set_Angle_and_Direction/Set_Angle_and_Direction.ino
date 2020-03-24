#include "StepMot.h"

/* Example of using StepMot Library
 * read about another methods in StepMot.h
 */

#define STEPS_PER_REVOLUTION 2037.88642 // 28BYJ-48 geared stepper motor
#define MICROSTEPS  4
#define STEP_PIN  16
#define DIR_PIN   10
#define EN_PIN    14

#define ENDSTOP_PIN 7

StepMot motor(STEPS_PER_REVOLUTION, MICROSTEPS, STEP_PIN, DIR_PIN, EN_PIN);

void setup() {
  Serial.begin(9600);
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  digitalWrite(5, 0);

  
  motor.setMode(ABSOLUTE);      // set to ABSOLUTE or RELATIVE modes (default RELATIVE)
  //motor.autoPower(ON);          // when ON motor will be enabled only while moving (default OFF)
  //motor.invertDir(ON);          // software direction inverting 
  motor.setRPM(5);             // speed in revolutions per minute
  motor.enable();               // power on the motor

  motor.rotate(CCW);
  Serial.println("go home..");
  while(digitalRead(ENDSTOP_PIN) && motor.move()) {}
  motor.resetPos();
  motor.setRPM(10);
}

void loop() {

  if(!motor.move()) {
    static uint16_t angle = 0;
    
    Serial.print("Reached angle: ");
    Serial.print(motor.getAngle());
    Serial.print("  Error: ");
    Serial.println(angle - motor.getAngle());
    Serial.println();
    
    angle = random(0,360);
    motor.setAngle(angle);
    Serial.print("Target -> ");
    Serial.println(angle);
    delay(1000);
  }
}
