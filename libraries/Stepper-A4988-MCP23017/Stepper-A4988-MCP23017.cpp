#include "Arduino.h"
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Stepper-A4988-MCP23017.h"

A4988_MCP23017::A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, byte MCP23017Adress)
{
	A4988_MCP23017(stepPin, directionPin, enablePin, -1, -1, -1, MCP23017Adress, 200, 500);
}

A4988_MCP23017::A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, byte MCP23017Adress, unsigned int stepsPerRound, unsigned int stepDelay)
{
	A4988_MCP23017(stepPin, directionPin, enablePin, -1, -1, -1, MCP23017Adress, stepsPerRound, stepDelay);
}

A4988_MCP23017::A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin, byte MCP23017Adress)
{
	A4988_MCP23017(stepPin, directionPin, enablePin, ms1Pin, ms2Pin, ms3Pin, MCP23017Adress, 200, 500);
}

A4988_MCP23017::A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin, byte MCP23017Adress, unsigned int stepsPerRound, unsigned int stepDelay)
{
	_stepPin = stepPin;
	_directionPin = directionPin;
	_enablePin = enablePin;
	_ms1Pin = ms1Pin;
	_ms2Pin = ms2Pin;
	_ms3Pin =ms3Pin;

	_mcpAdress = MCP23017Adress;
	_isUsingMCP = true;

	_stepsPerRound = stepsPerRound;
	_stepDelay = stepDelay;
	_directionChangeDelay = 100; // ms

}

void A4988_MCP23017::begin() {
	initMCP();
	initPinModes();
}

/*
 * MS PINs Methods
 * Step resolution
 */
void A4988_MCP23017::setStepMode(unsigned int stepMode)
{
	switch(stepMode) {
    case FULLSTEP: setFullStepMode(); break;
    case HALFSTEP: setHalfStepMode(); break;
    case QUARTERSTEP: setQuarterStepMode(); break;
    case EIGTHTHSTEP: setEigThthStepMode(); break;
    case SIXTEENTHSTEP: setSixteenthStepMode(); break;

    default:
      setFullStepMode();
  }
}

void A4988_MCP23017::doStep()
{
  mcp.digitalWrite(_stepPin, HIGH);
  delayMicroseconds(_stepDelay);
  mcp.digitalWrite(_stepPin, LOW);
  delayMicroseconds(_stepDelay);
}

void A4988_MCP23017::enable()
{
	mcp.digitalWrite(_enablePin,LOW);
	_isEnabled = true;
}

void A4988_MCP23017::disable()
{
  mcp.digitalWrite(_enablePin,HIGH);
	_isEnabled = false;
}

boolean A4988_MCP23017::isEnabled()
{
	return _isEnabled;
}

void A4988_MCP23017::clockWise()
{
	up();
}

void A4988_MCP23017::counterClockWise()
{
	down();
}

void A4988_MCP23017::up()
{
	disable();
  mcp.digitalWrite(_directionPin,HIGH);
	delay(_directionChangeDelay);
	enable();
	delay(_directionChangeDelay);

	_moveUp = true;
}

void A4988_MCP23017::down()
{
	disable();
  mcp.digitalWrite(_directionPin,LOW);
	delay(_directionChangeDelay);
	enable();
	delay(_directionChangeDelay);

	_moveUp = false;
}

void A4988_MCP23017::changeDirection() {
	if (_moveUp) {
		down();
	} else {
		up();
	}
}

void A4988_MCP23017::setStepsPerRound(double stepAngle)
{
	_stepsPerRound = (360 / stepAngle);
}

void A4988_MCP23017::setStepDelay(unsigned int delayMicroSeconds)
{
	_stepDelay = delayMicroSeconds;
}

void A4988_MCP23017::setRPM(unsigned int rpm)
{
	long minute = 60000000; // MicroSeconds
	long stepsPerMinute = rpm * _stepsPerRound;

	// Delay between steps
	// stepDelay > minimum delay
	_stepDelay = (minute / stepsPerMinute) < _minDelay ? _minDelay : (minute / stepsPerMinute); // MicroSeconds
}

void A4988_MCP23017::init() {
	if  (_stepPin == NULL || _stepPin <= -1 || _stepPin >= 8) _stepPin = -1;
 	if  (_directionPin == NULL || _directionPin <= -1 || _directionPin >= 8) _directionPin = -1;
	if  (_enablePin == NULL || _enablePin <= -1 || _enablePin >= 8) _enablePin = -1;
	if  (_ms1Pin == NULL || _ms1Pin <= -1 || _ms1Pin >= 8) _ms1Pin = -1;
	if  (_ms2Pin == NULL || _ms2Pin <= -1 || _ms2Pin >= 8) _ms2Pin = -1;
	if  (_ms3Pin == NULL || _ms3Pin <= -1 || _ms3Pin >= 8) _ms3Pin = -1;
}

void A4988_MCP23017::initMCP() {
 	mcp.begin(_mcpAdress);
 }

void A4988_MCP23017::initPinModes()
{
  mcp.pinMode(_stepPin, OUTPUT);
  mcp.digitalWrite(_stepPin, LOW);

  mcp.pinMode(_directionPin, OUTPUT);
  mcp.digitalWrite(_directionPin,LOW);

  mcp.pinMode(_enablePin, OUTPUT);
  mcp.digitalWrite(_enablePin,LOW);

  mcp.pinMode(_ms1Pin, OUTPUT);
  mcp.digitalWrite(_ms1Pin,LOW);

  mcp.pinMode(_ms2Pin, OUTPUT);
  mcp.digitalWrite(_ms2Pin,LOW);

  mcp.pinMode(_ms3Pin, OUTPUT);
  mcp.digitalWrite(_ms3Pin,LOW);
}



void A4988_MCP23017::setFullStepMode() {
	setMS1Pin(LOW);
  setMS2Pin(LOW);
  setMS3Pin(LOW);
}

void A4988_MCP23017::setHalfStepMode() {
	setMS1Pin(HIGH);
	setMS2Pin(LOW);
	setMS3Pin(LOW);
}

void A4988_MCP23017::setQuarterStepMode() {
	setMS1Pin(LOW);
	setMS2Pin(HIGH);
	setMS3Pin(LOW);
}

void A4988_MCP23017::setEigThthStepMode() {
	setMS1Pin(HIGH);
	setMS2Pin(HIGH);
	setMS3Pin(LOW);
}

void A4988_MCP23017::setSixteenthStepMode() {
	setMS1Pin(HIGH);
	setMS2Pin(HIGH);
	setMS3Pin(HIGH);
}

void A4988_MCP23017::setMS1Pin(uint8_t state) {
	if (_ms1Pin != -1)
    mcp.digitalWrite(_ms1Pin,state);
}

void A4988_MCP23017::setMS2Pin(uint8_t state) {
	if (_ms2Pin != -1)
    mcp.digitalWrite(_ms2Pin,state);
}

void A4988_MCP23017::setMS3Pin(uint8_t state) {
	if (_ms2Pin != -1)
    mcp.digitalWrite(_ms3Pin,state);
}
