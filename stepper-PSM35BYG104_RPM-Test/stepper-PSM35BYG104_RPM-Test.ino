#include <Stepper.h>

// L293D H-Bridge
// PIN 8 Bridge 1
// PIN 9
// PIN 10 Bridge 2
// PIN 11

//PIN A5 Bottom Contact
//PIN A4 Top Contact

const int stepsU = 200;

Stepper psm35byg104 (stepsU, 8, 9, 10, 11);

int value = 0, powerOn = 0;
int bottomReached = 0, topReached = 0;
boolean moveUp = false, moveDown = false, active = false;

int bottomCounter = 0;
unsigned long startTime, timer;

int rpmSeriePointer = 0;
long rpmSeries[20] = 
  {
    //25,
    //50,
    //75,
    100,
    125,
    150,
    175,
    200,
    225,
    250,
    275,
    300,
    325,
    350,
    375,
    400,
    425,
    450,
    475,
    500,
    525,
    550,
    575
  };


void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
}

void loop() {

  // Press the Push Button
  powerOn = analogRead(1);

  // Starting
  if (powerOn > 100 && active == true) {
    active = false;
    digitalWrite(2, LOW);
    delay(1000);
    
  } else if (powerOn > 100 && active == false) {
    active = true;
    digitalWrite(2, HIGH);
    delay(1000);

    // initialize
    moveDown = false;
    moveUp = true;

    bottomCounter = 0;
    rpmSeriePointer = 14;

    psm35byg104.setSpeed(rpmSeries[rpmSeriePointer]);


    Serial.print(rpmSeries[rpmSeriePointer]);
    Serial.print(",");

    //delay(100); // short delay: rpm > 400
  }
  
  if (active) {
    
    
    value = analogRead(2);
    bottomReached = analogRead(5);
    topReached = analogRead(4);
    
    if (bottomReached > 800) {

      //delay(100); // short delay: rpm > 400
      
      moveUp = true;
      moveDown = false;

      // Counting
      bottomCounter++;

      if (bottomCounter < 4) {

        if (bottomCounter > 0) {
          // Seconds
          timer = ( millis() - startTime ) / 1000;
          Serial.print(timer);
          Serial.print(",");
        }
        
        startTime = millis();

      } else {

        Serial.print("\n");

        // Next Sequence
        rpmSeriePointer++;

        if (rpmSeriePointer >= 20) {

            // Stop
            active = false;
            
        } else {
          
          Serial.print(rpmSeries[rpmSeriePointer]);
          Serial.print(",");

          // Set RPM Speed
          psm35byg104.setSpeed(rpmSeries[rpmSeriePointer]);

          bottomCounter = 0;
        }

        
      }
      
    } else if (topReached > 800) {

      moveDown = true;
      moveUp = false;

      //delay(100); // short delay: rpm > 400
    }
    
    // Manual setting
    if ( value > 500){
      
      moveUp = true;
      moveDown = false;
      
    } else  if (value > 100 && value < 500) {
      
      moveDown = true;
      moveUp = false;

    } else if (value < 100) {
	// do nothing
    }
    
    if (moveUp) {

      psm35byg104.step((stepsU-0)); // reduced steps per loop: rpm > 400
      
    } else if ( moveDown) {

      psm35byg104.step(-(stepsU-0)); // reduced steps per loop: rpm > 400

    } else {
     // do nothing
    }
  }  
}

