#include <Stepper-A4988.h>
#include <OneWire.h>
#include <Adafruit_NeoPixel.h>

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


int inPin = A5;
int buttonPin = A6;
int val = 0;

void setup() {
  
  stepper.disable();
  
  Serial.begin(9600);

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();

  stepper.enable();
}
void loop() {

  // Stepper
  stepper.enable();

  
  for (int i = 0; i < 200; i++) { // 200 steps
    stepper.doStep();    
  } 

  stepper.changeDirection();
  stepper.disable();

  // IR Sensor
  val = analogRead(inPin);
  Serial.print("\nIR: ");
  Serial.println(val);

  // Microbutton
  val = analogRead(buttonPin);
  Serial.print("\nButton: ");
  Serial.println(val);

  // OneWire DS2401
  if (autoSearch || isBooting) {
    discoverOneWireDevices();

    isBooting = false;
  } 

  // WS2812B
   for(int i=0;i<NUMPIXELS;i++){

   //showEachColor(i);
    showRainbow(i);
  }

  disableAllLED();
  
  delay(300);

  dimmAllLED();
delay(5000);
   disableAllLED();
  
  delay(300);
}

void disableAllLED() {
  for(int i=0;i<NUMPIXELS;i++){

   pixels.setPixelColor(i, pixels.Color(0,0,0));
  
  
  }

   pixels.show(); 
}

void dimmAllLED() {

   for(int brightness=0;brightness<100;brightness=brightness+10){
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(brightness,brightness,brightness));  
      }
  
     pixels.show(); 
     delay(50);
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
