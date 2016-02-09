#include <Wire.h>
#include "Adafruit_MCP23017.h"

// Basic pin reading and pullup test for the MCP23017 I/O expander
// public domain!

// Connect pin #12 of the expander to Analog 5 (i2c clock)
// Connect pin #13 of the expander to Analog 4 (i2c data)
// Connect pins #15, 16 and 17 of the expander to ground (address selection)
// Connect pin #9 of the expander to 5V (power)
// Connect pin #10 of the expander to ground (common ground)
// Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

// Output #0 is on pin 21 so connect an LED or whatever from that to ground

Adafruit_MCP23017 mcp;
  
void setup() {  
  mcp.begin();      // use default address 0

  mcp.pinMode(0, OUTPUT); // GPA0
  mcp.pinMode(1, OUTPUT); // GPA1
  mcp.pinMode(6, OUTPUT); // GPA6
  mcp.pinMode(7, OUTPUT); // GPA7
  
  mcp.pinMode(8, OUTPUT); // GPB0
  mcp.pinMode(9, OUTPUT); // GPB1
  mcp.pinMode(14, OUTPUT); // GPB6
  mcp.pinMode(15, OUTPUT); // GPB7
}


// flip the pin #0 up and down

void loop() {
  delay(500);

  mcp.digitalWrite(0, HIGH);
  mcp.digitalWrite(1, HIGH);
  mcp.digitalWrite(6, HIGH);
  mcp.digitalWrite(7, HIGH);

   mcp.digitalWrite(8, LOW);
   mcp.digitalWrite(9, LOW);
  mcp.digitalWrite(14, LOW);
  mcp.digitalWrite(15, LOW);
  
  delay(500);

  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(1, LOW);
  mcp.digitalWrite(6, LOW);
  mcp.digitalWrite(7, LOW);

  mcp.digitalWrite(8, HIGH);
  mcp.digitalWrite(9, HIGH);
  mcp.digitalWrite(14, HIGH);
  mcp.digitalWrite(15, HIGH);
}
