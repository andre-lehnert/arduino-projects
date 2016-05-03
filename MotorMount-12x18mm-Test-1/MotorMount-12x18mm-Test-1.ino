#include <Stepper-A4988.h>
#include <OneWire.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define NODE_ADDRESS 1  // Change this unique address for each I2C slave node
#define PAYLOAD_SIZE 3 // Number of bytes  expected to be received by the master I2C node

// NODE ID | FUNCTION | VALUE
byte nodePayload[PAYLOAD_SIZE];
String transmissionMsg = "";


A4988 stepper(2, 3, 4); // step, direction, enable pin


OneWire  ds(7);  // 1-Wire Bus Pin

boolean autoSearch = true;
boolean isBooting = true;

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


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            8

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      40

int delayval = 50; // delay for half a second

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


/* -------------------------------------------------------------------------------------
 * DriveShaft Parameter
 */

#define MIN_POSITION 0
#define MAX_POSITION 1700 // calibrate
#define CHECKUP_STEPS 15000 // calibrate each 15000 steps

/* -------------------------------------------------------------------------------------
 * 
 */

int inPin = A3;
int buttonPin = A6;
int value = 0;

String input = "";
int _position = -MAX_POSITION;
int _steps = 0;
boolean _debugging = false;

void setup() {
  
  stepper.disable();
  stepper.setHalfStepMode();
  
  stepper.up();
  
  Serial.begin(9600);


  // Start the I2C Bus as Slave on address 9
  Wire.begin(NODE_ADDRESS);  // Activate I2C network
  //Wire.onRequest(requestEvent); // Request attention of master node
  Wire.onReceive(receiveEvent); // register event
  



  pixels.begin();
  pixels.show();

  // Init
  stepper.enable();
  
  initialize();

  // Start Debugging
  enableDebugging();

  debug("//// I2C Slave Mode ////");
  debug("\tNode ID: ", NODE_ADDRESS);
  debug("\tPayload: ", PAYLOAD_SIZE);
  debug("//// -------------- ////");
  
  debug("-Target Position-\t-Current Position-\t-Number of Steps after Calibaration-");
  
}



// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {

  debug("---Receiving I2C Message---");
  
  int i = 0, j = 0;
  transmissionMsg = "";

  String r = "",
         g = "",
         b = "";

  
  while (1 <= Wire.available()) { // loop through all but the last

    if (i < 3) {    
      int c = Wire.read(); // receive byte as a character
      transmissionMsg += String(c);
    } else if (i < 26) {    
      char c = Wire.read(); // receive byte as a character
      transmissionMsg += String(c);
    } else {    
      int c = Wire.read(); // receive byte as a character
      transmissionMsg += String(c);
    }

    i++;
  }

  int commaIndex1 = transmissionMsg.indexOf(';'),
      commaIndex2 = transmissionMsg.indexOf(';', commaIndex1 + 1),
      commaIndex3 = transmissionMsg.indexOf(';', commaIndex2 + 1);
  
  String id         = transmissionMsg.substring(0, commaIndex1),
         function   = transmissionMsg.substring(commaIndex1 + 1, commaIndex2),
         operation  = transmissionMsg.substring(commaIndex2 + 1, commaIndex3),
         parameter  = transmissionMsg.substring(commaIndex3 + 1);

  debug(transmissionMsg);
         
  debug("ID\t\t", id);
  debug("FUNCTION\t", function);
  debug("OPERATION\t", operation);
  debug("PARAMETER\t", parameter);

  // DO MESSAGE COMMANDS

  if (function == "stepper...") {
    debug(">STEPPER");
    if (operation == "moveto....") {
      debug(">moveto");
      int targetPos = parameter.substring(0, 3).toInt();
      debug("TargetPosition: ", (MAX_POSITION * targetPos) / 100);
      //moveTo( (MAX_POSITION * targetPos) / 100  );
      
    } else if (operation == "stop......") {
      //stepper.disable();
      debug(">stop");
    } else if (operation == "init......") {
      //initialize();
      debug(">init");
    }
    
  } else if (function == "led.......") {
    debug(">LED");

    r = parameter.substring(0, 3);
    g = parameter.substring(4, 6);
    b = parameter.substring(7, 9);
    
    if (operation == "load......") {
      debug(">load");      
      loadingBarsLED(r.toInt(), g.toInt(), b.toInt(), 100);
    } else if (operation == "dimm......") {
      debug(">dimm");
      dimmAllLED(r.toInt(), g.toInt(), b.toInt(), 50);
    }
  }

  debug("-------------------");
  
}




boolean hasBottomReached() {

  int minValue = 100; // button inactive = ~0
  int maxValue = 800; // button active = ~1023

  value = analogRead(buttonPin);
  //debug("Button: ", value);
  return (value >= minValue); // button active  
  
}

boolean isIRActive() {

  int minValue = 200; // button inactive = ~180
  int maxValue = 800; // button active = ~1023

  value = analogRead(inPin);
  //debugInline("IR=", value);
  return (value <= minValue); // buttoSn active  
  
}

void setPosition(unsigned int stepsAwayFromGround) {
  _position = stepsAwayFromGround;
  debugInline("\t\t", _position);

}
void changePosition(int stepIncrement) {
  _position = _position + stepIncrement;
  debugInline("\t\t", _position);
}
unsigned int getPosition() {
  return _position;
}


void initialize() {  

  // calibrated
  if (getPosition() == 0 && getSteps() == 0) return;
  
  stepper.setHalfStepMode(); delay(20);

  // move up 
  int stepsUpwards = 100;  
  moveUp(stepsUpwards); delay(50);

  // move down and calibrate
  int stepResolutionDownwardsSlow = 15;
  int stepResolutionDownwardsFast = 200;
  int _tempMAX_POSITION = MAX_POSITION; //TODO

  int additionalRounds = 0;
  
  for (int i = 0; i < _tempMAX_POSITION/stepResolutionDownwardsFast && additionalRounds < 4; i++) {     
    
    if (isIRActive()) {
      additionalRounds = 2;
    }
    moveDown(stepResolutionDownwardsFast);
    
    if (hasBottomReached()) {
      setPosition(MIN_POSITION); 
      resetSteps();
      return;
    } else if (isIRActive()) {
      additionalRounds++; // initialization complete
    } 

  } 
  
  for (int i = 0; i < _tempMAX_POSITION/stepResolutionDownwardsSlow; i++) { 
    
    moveDown(stepResolutionDownwardsSlow);
    
    if (hasBottomReached()) { // check position after "stepResolutionDownwards" steps
      setPosition(MIN_POSITION); 
      resetSteps();
      break; // initialization complete
    }
  } 

}

void addSteps(unsigned int steps) {
  _steps = _steps + steps;
  debugInline("\t\t", _steps);
}

void resetSteps() {
  _steps = 0;
}

unsigned int getSteps() {
  return _steps;
}

boolean hasCheckup() {
  return (getSteps() >= CHECKUP_STEPS); // calibrate after each CHECKUP_STEPS
}

void doCheckUp(unsigned int targetPosition) {
  debug("\t\t\t---CHECKUP---\n");
  resetSteps();
  initialize();

  moveTo(targetPosition);
}

void moveTo(unsigned int targetPosition) {

  debug(targetPosition);
  
  int currentPosition = getPosition();
  
  if (targetPosition > currentPosition) {
    int difference = targetPosition - currentPosition;
    moveUp(difference);
    
  } else if (targetPosition < currentPosition) {
    int difference = currentPosition - targetPosition;
    moveDown(difference);
    
  } else {
    // currentPosition == targetPosition -> nothing to do
  }
  
}


void moveUp(unsigned int steps) {

  if (! hasCheckup()) {

    stepper.up();
    delay(100);
    
    for (int i = 0; i < steps; i++) { 
      stepper.doStep();    
    } 
    changePosition(+steps);
    addSteps(steps);
  
  } else { // CHECKUP

    int currentPosition = getPosition();
    int targetPosition = currentPosition + steps; // up    
   
    doCheckUp(targetPosition); // checkup + move to initial target position
    
  }
  
}

void moveDown(unsigned int steps) {

   if (! hasCheckup()) {

    stepper.down();
    delay(100);
    
    for (int i = 0; i < steps; i++) { 
      stepper.doStep();    
    } 
    changePosition(-steps);
    addSteps(steps);
  
  } else { // CHECKUP

    int currentPosition = getPosition();
    int targetPosition = currentPosition - steps; // down
    
    doCheckUp(targetPosition); // checkup + move to initial target position
    
  }
  
}


void loop() {

  if (Serial.available() > 0) {

    stepper.enable();
    stepper.setFullStepMode();
    
    input = Serial.readString();    
    delay(100);
    
    // initialize driveshaft
    if (input == "init") {
      initialize();
    } else if (input == "up") {
      moveUp(400);
    } else if (input == "down") {
      moveDown(400);
    } else if (input == "f") {
      initialize();

      moveUp(MAX_POSITION);
      moveDown(MAX_POSITION);
    } else if (input == "F") {
      initialize();
      for (int i = 0; i < 10; i++) {
        moveTo(MAX_POSITION);
        moveTo(MIN_POSITION);
      }
      
    } else if (input == "p") {
       initialize();

       for (int i = 0; i < 30000; i = i + 500) {
        moveTo(i % MAX_POSITION); // do 1000 steps
       }
       
      } else if (input == "l") {

          debug("--- Show LED---");
        
          // WS2812B
         for(int i=0;i<NUMPIXELS;i++){
      
         //showEachColor(i);
          showRainbow(i);
         
        }
      
        disableAllLED();
        
        delay(300);
        
     
         disableAllLED();
        
        delay(300);
      
    } else if (input == "ds") {
      // OneWire DS2401
      
        discoverOneWireDevices();
    
    }
    
  }

  stepper.disable();

}

void loadingBarsLED(int r, int g, int b, int d) {

  debugInline("R: ", r);
  debugInline("G: ", g);
  debugInline("B: ", b);
  
  for(int i=0;i<=NUMPIXELS/4;i++){

    pixels.setPixelColor(i, pixels.Color(r,g,b));
    pixels.setPixelColor(i+10, pixels.Color(r,g,b));
    pixels.setPixelColor(i+20, pixels.Color(r,g,b));
    pixels.setPixelColor(i+30, pixels.Color(r,g,b));

    pixels.show();
    delay(d);
  }

    
}

void disableAllLED() {
  for(int i=0;i<NUMPIXELS;i++){

   pixels.setPixelColor(i, pixels.Color(0,0,0));
  
  
  }

   pixels.show(); 
}

void dimmAllLED(int r, int g, int b, int d) {

   for(int brightness=0;brightness<100;brightness=brightness+10){
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color((r*brightness)/100,(g*brightness)/100,(b*brightness)/100));  
      }
  
     pixels.show(); 
     delay(d);
   }

}

void showRainbow(int currentPosition) {

  int lastPixel = (currentPosition-4)  % NUMPIXELS;
  int currentPixel = currentPosition % NUMPIXELS;
  int nextPixel = (currentPosition-1) % NUMPIXELS;
  int next2Pixel = (currentPosition-2) % NUMPIXELS;
  int next3Pixel = (currentPosition-3) % NUMPIXELS;
  
  pixels.setPixelColor(lastPixel, pixels.Color(0,0,0));
  pixels.setPixelColor(currentPixel, pixels.Color(0,150,0)); // Moderately bright green color.
  pixels.setPixelColor(nextPixel, pixels.Color(150,150,0));
  pixels.setPixelColor(next2Pixel, pixels.Color(150,0,150));
  pixels.setPixelColor(next3Pixel, pixels.Color(0,150,150));
  
  pixels.show(); // This sends the updated pixel color to the hardware.

  delay(delayval);
}


void showEachColor(int i) {

   // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

    pixels.setPixelColor(i, pixels.Color(150,0,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval);

    pixels.setPixelColor(i, pixels.Color(0,0,150)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval);

  pixels.setPixelColor(i, pixels.Color(150,150,150)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval);

    pixels.setPixelColor(i, pixels.Color(0,0,0));

    pixels.show(); // This sends the updated pixel color to the hardware.
  
}

void discoverOneWireDevices(void) {
  byte i, sn;
  byte addr[8];

  debug("--- Search ---");

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
        debug("ID: ",serialNumber+1);
      } else {
        debug("unknown ID");
      }
      
    
    if ( OneWire::crc8( addr, 7) != addr[7]) {
      debug(" -> CRC incorrect");
      return;
    }  
  }
  
  debug("--- Search complete ---");
  ds.reset_search();
  return;
}





/* -------------------------------------------------------------------------------------
 * Debugging
 */

void enableDebugging() {
  _debugging = true;
}

void disableDebugging() {
  _debugging = false;
}

void debugInline(String data) {
  debugInline("", data, "");
}

void debugInline(int data) {
  String s = String(data);
  debugInline("", s, "");
}

void debugInline(unsigned int data) {
  String s = String(data);
  debugInline("", s, "");
}
//void debugInline(char data) {
//  debugInline("", data+"", "");
//}
//void debugInline(boolean data) {
//  debugInline("", data+"", "");
//}
//void debugInline(byte data) {
//  debugInline("", data+"", "");
//}
//

void debugInline(String praefix, String data) {
  debugInline(praefix, data, "");
}
void debugInline(String praefix, int data) {
  String s = String(data);
  debugInline(praefix, s, "");
}
void debugInline(String praefix, unsigned int data) {
  String s = String(data);
  debugInline(praefix, s, "");
}
//void debugInline(String praefix, char data) {
//  debugInline(praefix, data+"", "");
//}
//void debugInline(String praefix, boolean data) {
//  debugInline(praefix, data+"", "");
//}
//void debugInline(String praefix, byte data) {
//  debugInline(praefix, data+"", "");
//}
//

void debugInline(String praefix, int data, String postfix) {
  String s = String(data);
  debugInline(praefix, s, postfix);
}
void debugInline(String praefix, unsigned int data, String postfix) {
  String s = String(data);
  debugInline(praefix, s, postfix);
}
//void debugInline(String praefix, char data, String postfix) {
//  debugInline(praefix, data+"", postfix);
//}
//void debugInline(String praefix, boolean data, String postfix) {
//  debugInline(praefix, data+"", postfix);
//}
//void debugInline(String praefix, byte data, String postfix) {
//  debugInline(praefix, data+"", postfix);
//}


void debug(String data) {
  debug("", data, "");
}
void debug(int data) {
  String s = String(data);
  debug("", s, "");
}
void debug(unsigned int data) {
  String s = String(data);
  debug("", s, "");
}
//void debug(char data) {
//  debug("", data+"", "");
//}
//void debug(boolean data) {
//  debug("", data+"", "");
//}
//void debug(byte data) {
//  debug("", data+"", "");
//}
//
void debug(String praefix, String data) {
 
  debug(praefix, data, "");
}

void debug(String praefix, int data) {
  String s = String(data);
  debug(praefix, s, "");
}
void debug(String praefix, unsigned int data) {
  String s = String(data);
  debug(praefix, s, "");
}
//void debug(String praefix, char data) {
//  debug(praefix, data+"", "");
//}
//void debug(String praefix, boolean data) {
//  debug(praefix, data+"", "");
//}
//void debug(String praefix, byte data) {
//  debug(praefix, data+"", "");
//}
//

void debug(String praefix, int data, String postfix) {
  String s = String(data);
  debug(praefix, s, postfix);
}
void debug(String praefix, unsigned int data, String postfix) {
  String s = String(data);
  debug(praefix, s, postfix);
}
//void debug(String praefix, char data, String postfix) {
//  debug(praefix, data+"", postfix);
//}
//void debug(String praefix, boolean data, String postfix) {
//  debug(praefix, data+"", postfix);
//}
//void debug(String praefix, byte data, String postfix) {
//  debug(praefix, data+"", postfix);
//}

void debugInline(String praefix, String data, String postfix) {
  if (_debugging) {
    Serial.print("\t"); // ?!
    Serial.print(praefix);
    Serial.print(data);
    Serial.print(postfix);
  }
}

void debug(String praefix, String data, String postfix) {
  if (_debugging) {
    Serial.print("\n");
    Serial.print(praefix);
    Serial.print(data);
    Serial.print(postfix);
  }
}

/* -------------------------------------------------------------------------------------
 * 
 */
