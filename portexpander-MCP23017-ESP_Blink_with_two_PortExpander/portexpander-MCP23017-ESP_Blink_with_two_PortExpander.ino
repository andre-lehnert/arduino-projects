/*
 * MCP23017 ES/P 
 * 16-Bit I/O Expander with Serial Interface
 * 
 * Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/21952b.pdf
 * Adafruit MCP23017 Arduino Library: https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
 * 
 * The sketch controls two MCP23017 chips.
 * 
 * 2016, Andre Lehnert
 * MIT Licence
 */

/*
 * First MCP23017 "A"
 * Pin 1: GPB 0 -> additional Pins 8-15
 * Pin 2: GPB 1 
 * Pin 3: GPB 2
 * Pin 4: GPB 3
 * Pin 5: GPB 4
 * Pin 6: GPB 5
 * Pin 7: GPB 6
 * Pin 8: GPB 7
 * Pin 9: VDD connect to 5V
 * Pin 10: VSS connect to GND
 * Pin 11: NC
 * Pin 12: SCL (i2c clock) connect to Arduino SCL Pin (Arduino Uno: Analog Pin 5)
 * Pin 13: SDA (i2c data) connect to Arduino SDA Pin (Arduino Uno: Analog Pin 4)
 * Pin 14: NC
 * Pin 15: A0 adress bit 0 connect to GND -> mcp.begin(0)
 * Pin 16: A1 adress bit 1 connect to GND
 * Pin 17: A2 adress bit 2 connect to GND
 * Pin 18: RESET connect to 5V with 10k Ohm resistor
 * Pin 19: INTB
 * Pin 20: INTA
 * Pin 21: GPA 0 -> additional Pins 0-7
 * Pin 22: GPA 1
 * Pin 23: GPA 2
 * Pin 24: GPA 3
 * Pin 25: GPA 4
 * Pin 26: GPA 5
 * Pin 27: GPA 6
 * Pin 28: GPA 7
 */

 /*
 * First MCP23017 "B"
 * Pin 1: GPB 0 -> additional Pins 8-15
 * Pin 2: GPB 1 
 * Pin 3: GPB 2
 * Pin 4: GPB 3
 * Pin 5: GPB 4
 * Pin 6: GPB 5
 * Pin 7: GPB 6
 * Pin 8: GPB 7
 * Pin 9: VDD connect to 5V
 * Pin 10: VSS connect to GND
 * Pin 11: NC
 * Pin 12: SCL (i2c clock) connect to Arduino SCL Pin (Arduino Uno: Analog Pin 5)
 * Pin 13: SDA (i2c data) connect to Arduino SDA Pin (Arduino Uno: Analog Pin 4)
 * Pin 14: NC
 * Pin 15: A0 adress bit 0 connect to 5V -> mcp.begin(1)
 * Pin 16: A1 adress bit 1 connect to GND
 * Pin 17: A2 adress bit 2 connect to GND
 * Pin 18: RESET connect to 5V with 10k Ohm resistor
 * Pin 19: INTB
 * Pin 20: INTA
 * Pin 21: GPA 0 -> additional Pins 0-7
 * Pin 22: GPA 1
 * Pin 23: GPA 2
 * Pin 24: GPA 3
 * Pin 25: GPA 4
 * Pin 26: GPA 5
 * Pin 27: GPA 6
 * Pin 28: GPA 7
 */

#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcpA;
Adafruit_MCP23017 mcpB;

void setup() {  
  
  // First MCP23017
  mcpA.begin(); // default address 0 [0-7]

  mcpA.pinMode(0, OUTPUT); // GPA0
  mcpA.pinMode(1, OUTPUT); // GPA1
  mcpA.pinMode(6, OUTPUT); // GPA6
  mcpA.pinMode(7, OUTPUT); // GPA7
  
  mcpA.pinMode(8, OUTPUT); // GPB0
  mcpA.pinMode(9, OUTPUT); // GPB1
  mcpA.pinMode(14, OUTPUT); // GPB6
  mcpA.pinMode(15, OUTPUT); // GPB7

  // Second MCP23017
  mcpB.begin(1); // address 1 [0-7]

  mcpB.pinMode(0, OUTPUT); // GPA0
  mcpB.pinMode(1, OUTPUT); // GPA1
  mcpB.pinMode(6, OUTPUT); // GPA6
  mcpB.pinMode(7, OUTPUT); // GPA7
  
  mcpB.pinMode(8, OUTPUT); // GPB0
  mcpB.pinMode(9, OUTPUT); // GPB1
  mcpB.pinMode(14, OUTPUT); // GPB6
  mcpB.pinMode(15, OUTPUT); // GPB7
}

void loop() {
  
  delay(500);
  
  // A: GPA-Site HIGH
  mcpA.digitalWrite(0, HIGH);
  mcpA.digitalWrite(1, HIGH);
  mcpA.digitalWrite(6, HIGH);
  mcpA.digitalWrite(7, HIGH);
  // A: GPB-Site LOW
  mcpA.digitalWrite(8, LOW);
  mcpA.digitalWrite(9, LOW);
  mcpA.digitalWrite(14, LOW);
  mcpA.digitalWrite(15, LOW);
  // B: GPA-Site LOW
  mcpB.digitalWrite(0, LOW);
  mcpB.digitalWrite(1, LOW);
  mcpB.digitalWrite(6, LOW);
  mcpB.digitalWrite(7, LOW);
  // B: GPB-Site HIGH
  mcpB.digitalWrite(8, HIGH);
  mcpB.digitalWrite(9, HIGH);
  mcpB.digitalWrite(14, HIGH);
  mcpB.digitalWrite(15, HIGH);
  
  delay(500);
  
  // A: GPA-Site LOW
  mcpA.digitalWrite(0, LOW);
  mcpA.digitalWrite(1, LOW);
  mcpA.digitalWrite(6, LOW);
  mcpA.digitalWrite(7, LOW);
  // A: GPB-Site HIGH
  mcpA.digitalWrite(8, HIGH);
  mcpA.digitalWrite(9, HIGH);
  mcpA.digitalWrite(14, HIGH);
  mcpA.digitalWrite(15, HIGH);
  // B: GPA-Site HIGH
  mcpB.digitalWrite(0, HIGH);
  mcpB.digitalWrite(1, HIGH);
  mcpB.digitalWrite(6, HIGH);
  mcpB.digitalWrite(7, HIGH);
  // B: GPB-Site LOW
  mcpB.digitalWrite(8, LOW);
  mcpB.digitalWrite(9, LOW);
  mcpB.digitalWrite(14, LOW);
  mcpB.digitalWrite(15, LOW);
}
