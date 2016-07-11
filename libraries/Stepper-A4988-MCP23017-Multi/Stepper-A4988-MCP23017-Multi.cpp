#include "Arduino.h"
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Stepper-A4988-MCP23017-Multi.h"

A4988_MCP23017_Multi::A4988_MCP23017_Multi() {}

A4988_MCP23017_Multi::A4988_MCP23017_Multi(unsigned int stepsPerRound, unsigned int stepDelay) {
  _stepPin = -1;
  _directionPin = -1;
  _enablePin = -1;
  _ms1Pin = -1;
  _ms2Pin = -1;
  _ms3Pin = -1;

  _stepsPerRound = stepsPerRound;
  _stepDelay = stepDelay;
  _directionChangeDelay = 100; // ms

}
A4988_MCP23017_Multi::A4988_MCP23017_Multi(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin) {
  _stepPin = stepPin;
  _directionPin = directionPin;
  _enablePin = enablePin;
  _ms1Pin = -1;
  _ms2Pin = -1;
  _ms3Pin = -1;

  _stepsPerRound = 200;
  _stepDelay = 500;
  _directionChangeDelay = 100; // ms
}
A4988_MCP23017_Multi::A4988_MCP23017_Multi(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin) {
  _stepPin = stepPin;
  _directionPin = directionPin;
  _enablePin = enablePin;
  _ms1Pin = ms1Pin;
  _ms2Pin = ms2Pin;
  _ms3Pin = ms3Pin;

  _stepsPerRound = 200;
  _stepDelay = 500;
  _directionChangeDelay = 100; // ms
}
A4988_MCP23017_Multi::A4988_MCP23017_Multi(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, unsigned int stepsPerRound, unsigned int stepDelay) {
  _stepPin = stepPin;
  _directionPin = directionPin;
  _enablePin = enablePin;
  _ms1Pin = -1;
  _ms2Pin = -1;
  _ms3Pin = -1;

  _stepsPerRound = stepsPerRound;
  _stepDelay = stepDelay;
  _directionChangeDelay = 100; // ms
}
A4988_MCP23017_Multi::A4988_MCP23017_Multi(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin, unsigned int stepsPerRound, unsigned int stepDelay) {
  _stepPin = stepPin;
  _directionPin = directionPin;
  _enablePin = enablePin;
  _ms1Pin = ms1Pin;
  _ms2Pin = ms2Pin;
  _ms3Pin = ms3Pin;

  _stepsPerRound = stepsPerRound;
  _stepDelay = stepDelay;
  _directionChangeDelay = 100; // ms
}

/*
 * -----------------------------------------------------------------------------
 */

void A4988_MCP23017_Multi::begin(uint8_t mcp)
{
   _mcp = mcp;
   _stepMcp = mcp;
   _directionMcp = mcp;
   _enableMcp = mcp;
   _ms1Mcp = mcp;
   _ms2Mcp = mcp;
   _ms3Mcp = mcp;

   _begin(_mcp);
   initPinModes();
}

void A4988_MCP23017_Multi::begin(uint8_t stepMcp, uint8_t directionMcp, uint8_t enableMcp)
{
  _mcp = -1;
  _stepMcp = stepMcp;
  _directionMcp = directionMcp;
  _enableMcp = enableMcp;
  _ms1Mcp = -1;
  _ms2Mcp = -1;
  _ms3Mcp = -1;

  _begin();
  initPinModes();
}

void A4988_MCP23017_Multi::begin(uint8_t stepMcp, uint8_t directionMcp, uint8_t enableMcp, uint8_t ms1Mcp, uint8_t ms2Mcp, uint8_t ms3Mcp)
{
  _mcp = -1;
  _stepMcp = stepMcp;
  _directionMcp = directionMcp;
  _enableMcp = enableMcp;
  _ms1Mcp = ms1Mcp;
  _ms2Mcp = ms2Mcp;
  _ms3Mcp = ms3Mcp;

  _begin();
  initPinModes();
}

 /*
  * MS PINs Methods
  * Step resolution
  */
 void A4988_MCP23017_Multi::setStepMode(unsigned int stepMode)
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

 void A4988_MCP23017_Multi::doStep()
 {
   _digitalWrite(_stepPin, HIGH);
   delayMicroseconds(_stepDelay);
   _digitalWrite(_stepPin, LOW);
   delayMicroseconds(_stepDelay);
 }

 void A4988_MCP23017_Multi::enable()
 {
 	_digitalWrite(_enablePin, LOW);
 	_isEnabled = true;
 }

 void A4988_MCP23017_Multi::disable()
 {
   _digitalWrite(_enablePin, HIGH);
 	_isEnabled = false;
 }

 void A4988_MCP23017_Multi::clockWise()
 {
 	up();
 }

 void A4988_MCP23017_Multi::counterClockWise()
 {
 	down();
 }

 void A4988_MCP23017_Multi::up()
 {
 	disable();
  _digitalWrite(_directionPin, HIGH);
 	delay(_directionChangeDelay);
 	enable();
 	delay(_directionChangeDelay);

 	_moveUp = true;
 }

 void A4988_MCP23017_Multi::down()
 {
 	disable();
  _digitalWrite(_directionPin, LOW);
 	delay(_directionChangeDelay);
 	enable();
 	delay(_directionChangeDelay);

 	_moveUp = false;
 }

 void A4988_MCP23017_Multi::changeDirection() {
 	if (_moveUp) {
 		down();
 	} else {
 		up();
 	}
 }

 void A4988_MCP23017_Multi::initPinModes()
 {
   _pinMode(_stepPin, 0x1);
   _digitalWrite(_stepPin, 0x0);

   _pinMode(_directionPin, 0x1);
   _digitalWrite(_directionPin, 0x0);

   _pinMode(_enablePin, 0x1);
   _digitalWrite(_enablePin, 0x0);

   _pinMode(_ms1Pin, 0x1);
   _digitalWrite(_ms1Pin, 0x0);

   _pinMode(_ms2Pin, 0x1);
   _digitalWrite(_ms2Pin, 0x0);

   _pinMode(_ms3Pin, 0x1);
   _digitalWrite(_ms3Pin, 0x0);
 }

 void A4988_MCP23017_Multi::test(uint8_t pin, uint8_t level)
 {
   _pinMode(pin, level);
   _digitalWrite(pin, level);

 }


/*
 * -----------------------------------------------------------------------------
 */



     // minihelper to keep Arduino backward compatibility
     static inline void _send(uint8_t x) {
     #if ARDUINO >= 100
     	Wire.write((uint8_t) x);
     #else
     	Wire.send(x);
     #endif
     }

     static inline uint8_t _receive() {
     #if ARDUINO >= 100
     	return Wire.read();
     #else
     	return Wire.receive();
     #endif
     }

 /**
  * Bit number associated to a give Pin
  */
 uint8_t A4988_MCP23017_Multi::_bitForPin(uint8_t pin){
 	return pin%8;
 }

 /**
  * Register address, port dependent, for a given PIN
  */
 uint8_t A4988_MCP23017_Multi::_regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr){
 	return(pin<8) ?portAaddr:portBaddr;
 }

 /**
  * Sets the pin mode to either INPUT or OUTPUT
  */
  void A4988_MCP23017_Multi::_pinMode(uint8_t pin, uint8_t pinMode, uint8_t mcp) {
  	_updateRegisterBit(pin, (pinMode==INPUT), IODIRA, IODIRB, mcp);
  }
  /**
   * Helper to update a single bit of an A/B register.
   * - Reads the current register value
   * - Writes the new register value
   */
  void A4988_MCP23017_Multi::_updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr, uint8_t mcp) {
  	uint8_t regValue;
  	uint8_t regAddr= _regForPin(pin,portAaddr,portBaddr);
  	uint8_t bit= _bitForPin(pin);
  	regValue = _readRegister(regAddr, mcp);

  	// set the value for the particular bit
  	bitWrite(regValue,bit,pValue);

  	_writeRegister(regAddr,regValue, mcp);
  }

  /**
   * Reads a given register
   */
  uint8_t A4988_MCP23017_Multi::_readRegister(uint8_t addr, uint8_t mcp){
    // read the current GPINTEN
    Wire.beginTransmission(ADDRESS | mcp);
    _send(addr);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS | mcp, 1);
    return _receive();
  }


  /**
   * Writes a given register
   */
  void A4988_MCP23017_Multi::_writeRegister(uint8_t regAddr, uint8_t regValue, uint8_t mcp){
    // Write the register
    Wire.beginTransmission(ADDRESS | mcp);
    _send(regAddr);
    _send(regValue);
    Wire.endTransmission();
  }



    void A4988_MCP23017_Multi::_digitalWrite(uint8_t pin, uint8_t level, uint8_t mcp) {
    	uint8_t gpio;
    	uint8_t bit= _bitForPin(pin);



    	// read the current GPIO output latches
    	uint8_t regAddr= _regForPin(pin, OLATA, OLATB);
    	gpio = _readRegister(regAddr, mcp);

    	// set the pin and direction
    	bitWrite(gpio, bit, level);

    	// write the new GPIO
    	regAddr= _regForPin(pin, GPIOA, GPIOB);
    	_writeRegister(regAddr, gpio, mcp);
    }

    /**
     * Initializes the MCP23017 given its HW selected address, see datasheet for Address selection.
     */
    void A4988_MCP23017_Multi::_begin(uint8_t mcp) {
     if (mcp > 7) {
       mcp = 7;
     }

     Wire.begin();

     // set defaults!
     // all inputs on port A and B
     _writeRegister(IODIRA,0xff, mcp);
     _writeRegister(IODIRB,0xff, mcp);
    }

    void A4988_MCP23017_Multi::_begin() {

     Wire.begin();

     /*
      * TODO
      */

     // set defaults!
     // all inputs on port A and B
     _writeRegister(IODIRA,0xff, mcp);
     _writeRegister(IODIRB,0xff, mcp);
    }
   /*
    * -----------------------------------------------------------------------------
    */

 void A4988_MCP23017_Multi::setFullStepMode() {
 	 setMS1Pin(LOW);
   setMS2Pin(LOW);
   setMS3Pin(LOW);
 }

 void A4988_MCP23017_Multi::setHalfStepMode() {
 	setMS1Pin(HIGH);
 	setMS2Pin(LOW);
 	setMS3Pin(LOW);
 }

 void A4988_MCP23017_Multi::setQuarterStepMode() {
 	setMS1Pin(LOW);
 	setMS2Pin(HIGH);
 	setMS3Pin(LOW);
 }

 void A4988_MCP23017_Multi::setEigThthStepMode() {
 	setMS1Pin(HIGH);
 	setMS2Pin(HIGH);
 	setMS3Pin(LOW);
 }

 void A4988_MCP23017_Multi::setSixteenthStepMode() {
 	setMS1Pin(HIGH);
 	setMS2Pin(HIGH);
 	setMS3Pin(HIGH);
 }

 void A4988_MCP23017_Multi::setMS1Pin(uint8_t state) {
 	if (_ms1Pin != -1)
     _digitalWrite(_ms1Pin,state);
 }

 void A4988_MCP23017_Multi::setMS2Pin(uint8_t state) {
 	if (_ms2Pin != -1)
     _digitalWrite(_ms2Pin,state);
 }

 void A4988_MCP23017_Multi::setMS3Pin(uint8_t state) {
 	if (_ms2Pin != -1)
     _digitalWrite(_ms3Pin,state);
 }

 /*
  * -----------------------------------------------------------------------------
  */

 boolean A4988_MCP23017_Multi::isEnabled()
 {
 	return _isEnabled;
 }

 void A4988_MCP23017_Multi::setStepsPerRound(double stepAngle)
 {
 	_stepsPerRound = (360 / stepAngle);
 }

 void A4988_MCP23017_Multi::setStepDelay(unsigned int delayMicroSeconds)
 {
 	_stepDelay = delayMicroSeconds;
 }

 void A4988_MCP23017_Multi::setRPM(unsigned int rpm)
 {
 	long minute = 60000000; // MicroSeconds
 	long stepsPerMinute = rpm * _stepsPerRound;

 	// Delay between steps
 	// stepDelay > minimum delay
 	_stepDelay = (minute / stepsPerMinute) < _minDelay ? _minDelay : (minute / stepsPerMinute); // MicroSeconds
 }
