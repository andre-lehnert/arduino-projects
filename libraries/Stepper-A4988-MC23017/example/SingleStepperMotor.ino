#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include <Stepper-A4988-MCP23017.h>

// MCP23017 Pins
int stepPin = 0;          // GPA0
int directionPin = 1;     // GPA1
int enablePin = 2;        // GPA2

// Step mode pins
int ms1Pin = 8;           // GPB0
int ms2Pin = 9;           // GPB1
int ms3Pin = 10;          // GPB2

//I2C adress
byte mcpAdress = 0;       // A0 = GND, A1 = GND, A2 = GND

// Stepper config
int stepDelay = 500;      // Microseconds
int stepsPerRound = 200;  // 1.8° steps


/*
 * Constructors
 * - A4988_MCP23017 stepper(stepPin, directionPin, enablePin, mcpAdress);
 * - A4988_MCP23017 stepper(stepPin, directionPin, enablePin, mcpAdress, stepDelay, stepsPerRound);
 * - A4988_MCP23017 stepper(stepPin, directionPin, enablePin, ms1Pin, ms2Pin, ms3Pin, mcpAdress);
 * - A4988_MCP23017 stepper(stepPin, directionPin, enablePin, ms1Pin, ms2Pin, ms3Pin, mcpAdress, stepDelay, stepsPerRound);
 */
A4988_MCP23017 stepper(stepPin, directionPin, enablePin, mcpAdress);


void setup() {

  stepper.begin();

  // FULLSTEP, HALFSTEP, QUARTERSTEP, EIGTHTHSTEP, SIXTEENTHSTEP
  // msX pins have to be defined
  stepper.setStepMode(FULLSTEP);

  // define rotation direction
  stepper.up(); // = stepper.clockWise();
}

void loop() {

  stepper.changeDirection();
  stepper.enable();

  // one round
  for(int i = 0; i < stepsPerRound*20; i++) {
      stepper.doStep();
  }

  // energy saving
  stepper.disable();

  delay(1000);
}
