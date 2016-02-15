#ifndef Stepper-A4988-MCP23017_h
#define Stepper-A4988-MCP23017_h

#include "Arduino.h"

#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#else
#include <Wire.h>
#endif

#include "Adafruit_MCP23017.h"
#include "Stepper-A4988-MCP23017.h"



class A4988_MCP23017
{
	public:
		A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, byte MCP23017Adress);
//		A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, byte stepPinMCPAdress, byte directionPinMCPAdress, byte enablePinMCPAdress);
		A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, byte MCP23017Adress, unsigned int stepsPerRound, unsigned int stepDelay);
//	A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, byte stepPinMCPAdress, byte directionPinMCPAdress, byte enablePinMCPAdress, unsigned int stepsPerRound, unsigned int stepDelay);
		A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin, byte MCP23017Adress);
//		A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin, byte stepPinMCPAdress, byte directionPinMCPAdress, byte enablePinMCPAdress, byte ms1PinMCPAdress, byte ms2PinMCPAdress, byte ms3PinMCPAdress);
		A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin, byte MCP23017Adress, unsigned int stepsPerRound, unsigned int stepDelay);
//		A4988_MCP23017(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin, uint8_t ms1Pin, uint8_t ms2Pin, uint8_t ms3Pin, byte stepPinMCPAdress, byte directionPinMCPAdress, byte enablePinMCPAdress, byte ms1PinMCPAdress, byte ms2PinMCPAdress, byte ms3PinMCPAdress, unsigned int stepsPerRound, unsigned int stepDelay);
		void begin();
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

	private:
		unsigned int _stepsPerRound;
		uint8_t _stepPin;
		uint8_t _directionPin;
		uint8_t _enablePin;
		uint8_t _ms1Pin;
		uint8_t _ms2Pin;
		uint8_t _ms3Pin;
		byte _mcpAdress;
		Adafruit_MCP23017 mcp;
		unsigned int _stepDelay;
		unsigned int _initDelay;
		unsigned int _minDelay;
		unsigned int _directionChangeDelay;
		boolean _moveUp;
		boolean _isEnabled;
		boolean _isNewRequest;
		boolean _isUsingMCP;
		void init();
//		void setMCPAdressPins(byte stepPinMCPAdress, byte directionPinMCPAdress, byte enablePinMCPAdress, byte ms1PinMCPAdress, byte ms2PinMCPAdress, byte ms3PinMCPAdress);
		void initMCP();
		void initPinModes();
		void setFullStepMode();
		void setHalfStepMode();
		void setQuarterStepMode();
		void setEigThthStepMode();
		void setSixteenthStepMode();
		void setMS1Pin(uint8_t state);
		void setMS2Pin(uint8_t state);
		void setMS3Pin(uint8_t state);
};

#define FULLSTEP 0
#define HALFSTEP 1
#define QUARTERSTEP 2
#define EIGTHTHSTEP 3
#define SIXTEENTHSTEP 4

#endif
