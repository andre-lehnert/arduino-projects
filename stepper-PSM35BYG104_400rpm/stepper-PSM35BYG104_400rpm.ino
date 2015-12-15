#include <Stepper.h>


// L293D H-Bridge
// PIN 8 Bridge 1
// PIN 9
// PIN 10 Bridge 2
// PIN 11

//PIN A5 Bottom Contact
//PIN A4 Top Contact

const int stepsU = 200;

int value = 0;
int powerOn = 0;
int bottomReached = 0;
int topReached = 0;
boolean moveUp = false;
boolean moveDown = false;
boolean active = false;

Stepper psm35byg104 (stepsU, 8, 9, 10, 11);


void setup() {
  
  // 400 PRM
  psm35byg104.setSpeed(400);
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
  }
  
  if (active) {
       
    value = analogRead(2);
    bottomReached = analogRead(5);
    topReached = analogRead(4);
    
    if (bottomReached > 800){
  
      moveUp = true;
      moveDown = false;
      
    } else if (topReached > 800) {
      
      moveDown = true;
      moveUp = false;
      
    }

    // Manual setting
    // Adjust Poti
    // Push Button
    if ( value > 500) {
      
      moveUp = true;
      moveDown = false;
      
    } else  if (value > 100 && value < 500) {
      
      moveDown = true;
      moveUp = false;
      
    } else if (value < 100) {
      // do nothing
    }

    // Linear translation
    if (moveUp) {
      
      // perform a full rotation
      psm35byg104.step((stepsU));
      
    } else if ( moveDown) {
      
      // perform a full rotation
      psm35byg104.step(-(stepsU));
      
    } else {
      // do nothing
    }  
  }  
}

