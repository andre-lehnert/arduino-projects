#include <OneWire.h>

/*
 * One-Wire-Protocol
 * - Maxim Integrated DS2401/ TO92 Silicon Serial Number
 *  - https://datasheets.maximintegrated.com/en/ds/DS2401.pdf
 *  - Pin 1: Ground
 *  - Pin 2: Data
 *  - Pin 3: Nothing
 *  
 * - 64 Bit Serial Number
 *  - 8 Bit/ 1 Byte: CRC (addr[0])
 *  - 48 Bit/ 6 Byte: Serial Number (addr[1-6])
 *  - 8 Bit/ 1 Byte: Family Code (addr[7])
 *  
 * - 5 V arduino power supply
 * - 4k7 pullup resistor
 * 
 * 
 */


OneWire  ds(2);  // 1-Wire Bus Pin

byte serialNumbers[10][8] = {
  // family    // serialnumber             // crc
  {1,          184,   1, 79, 14, 0, 0,     216},
  {1,          190,  18, 79, 14, 0, 0,      88},
  {1,          123, 206, 78, 14, 0, 0,     220},
  {1,          203,  24, 79, 14, 0, 0,     150},
  {1,          160, 141, 79, 14, 0, 0,     200},
  {1,           76, 208, 78, 14, 0, 0,     106},
  {1,          158, 235, 78, 14, 0, 0,      52},
  {1,          168,  92, 79, 14, 0, 0,     250},
  {1,          185, 102, 79, 14, 0, 0,     175},
  {1,          225,  74, 79, 14, 0, 0,     249}
};

boolean autoSearch = true;
boolean isBooting = true;

void setup(void) {
  Serial.begin(9600);
  
}
void discoverOneWireDevices(void) {
  byte i, sn;
  byte addr[8];

  Serial.print("--- Search --- \n\n");

  while (ds.search(addr)) {
    
      for ( i = 0; i < 8; i++) {
        if (addr[i] < 16) {
          Serial.print('0');
        }
        Serial.print(addr[i], HEX);
        if (i < 7) {
          Serial.print(":");
        }
      }
         
      boolean checkSN = false;
      int serialNumber = -1;
      
      // check all serialnumnbers
      for (sn=0; sn< 10; sn++) {
        
        // check each address byte
        for (i=0; i < 8; i++) {
          
          checkSN = false;
          
          if (serialNumbers[sn][i] == addr[i]) {
            checkSN = true;
          } 
  
          if (! checkSN) {
            break;
          }
        }

        if (checkSN) {
            serialNumber = sn;
            break;
        }
      }
      
      if (checkSN) {
        Serial.print("  -> ID ");
        Serial.print(serialNumber+1);
        Serial.print("\n");
      } else {
        Serial.print("  -> unknown ID\n");
      }
      
    
    if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print(" -> CRC incorrect\n");
      return;
    }  
  }
  
  Serial.print("\n\r\n\r--- Search complete ---\r\n");
  ds.reset_search();
  return;
}


void loop(void) {

  delay(3000);

  if (autoSearch || isBooting) {
    discoverOneWireDevices();

    isBooting = false;
  } 
  
}
