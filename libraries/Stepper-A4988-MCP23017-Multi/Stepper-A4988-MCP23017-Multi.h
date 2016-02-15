#ifndef Stepper-A4988-MCP23017-Multi_h
#define Stepper-A4988-MCP23017-Multi_h

#include "Arduino.h"

#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#else
#include <Wire.h>
#endif

#include "Adafruit_MCP23017.h"
#include "Stepper-A4988-MCP23017-Multi.h"



class A4988_MCP23017_Multi
{
	public:
		A4988_MCP23017_Multi();
		void begin(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t mcp);
		void begin(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin,  unsigned int stepsPerRound, unsigned int stepDelay, uint8_t mcp);
		void begin(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin, uint8_t mcp);
		void begin(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin, unsigned int stepsPerRound, unsigned int stepDelay, uint8_t mcp);
		void setStepMode(unsigned int stepMode);
		void doStep();
		void enable();
		void disable();
		boolean isEnabled();
		void up();
		void clockWise();
		void down();
		void counterClockWise();
		void changeDirection();
		void setStepsPerRound(double stepAngle);
		void setStepDelay(unsigned int delayMicroSeconds);
		void setRPM(unsigned int rpm);

		 void test(uint8_t pin, uint8_t level);

	private:
		unsigned int _stepsPerRound;
		uint8_t _stepPin;
		uint8_t _directionPin;
		uint8_t _enablePin;
		uint8_t _ms1Pin;
		uint8_t _ms2Pin;
		uint8_t _ms3Pin;
		uint8_t _mcp;
		unsigned int _stepDelay;
		unsigned int _initDelay;
		unsigned int _minDelay;
		unsigned int _directionChangeDelay;
		boolean _moveUp;
		boolean _isEnabled;
		boolean _isNewRequest;
		boolean _isUsingMCP;
		void initPinModes();
		void setFullStepMode();
		void setHalfStepMode();
		void setQuarterStepMode();
		void setEigThthStepMode();
		void setSixteenthStepMode();
		void setMS1Pin(uint8_t state);
		void setMS2Pin(uint8_t state);
		void setMS3Pin(uint8_t state);
		// Adafruit MCP23017 Library snippets
		uint8_t _regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr);
		uint8_t _bitForPin(uint8_t pin);
		//void _send(uint8_t x);
		//uint8_t _receive();
		void _pinMode(uint8_t pin, uint8_t pinMode);
		void _digitalWrite(uint8_t pin, uint8_t level);
	  uint8_t _readRegister(uint8_t addr);
	  void _writeRegister(uint8_t addr, uint8_t value);
	  void _updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);
};

#define FULLSTEP 0
#define HALFSTEP 1
#define QUARTERSTEP 2
#define EIGTHTHSTEP 3
#define SIXTEENTHSTEP 4


#define ADDRESS 0x20

// registers
#define IODIRA 0x00
#define GPIOA 0x12
#define OLATA 0x14

#define IODIRB 0x01
#define GPIOB 0x13
#define OLATB 0x15

#endif
