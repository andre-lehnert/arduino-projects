
#include <Stepper-A4988.h>


A4988 stepper(2, 3, 4); // step, direction, enable pin
int _steps = 0;
int _maxSteps = 0;

void setup() {
  stepper.disable();
  //
  stepper.up();
  
  
  Serial.begin(9600);
}

void loop() {
  

  if (Serial.available() > 0) {
    
    stepper.enable();
 // stepper.setFullStepMode();
   stepper.setHalfStepMode();
    String input = "";
    input = Serial.readString();    
    delay(100);
    
    // initialize driveshaft
    if (input == "d") {
      moveDown(800);
      
    } else if (input == "u") {
      moveUp(800);
     
    } else if (input == "r") {
     
      _steps = 0;
    } else if (input == "U") {
      moveUp(6000);
    } else if (input == "D") {
      moveDown(6000);
    }
  }

  stepper.disable();
}


void moveUp(unsigned int steps) {
 
    stepper.up();
    delay(100);
    
    for (int i = 0; i < steps; i++) { 
      stepper.doStep();    
    } 

    _steps += 400;
}


void moveDown(unsigned int steps) {
 
    stepper.down();
    delay(100);
    
    for (int i = 0; i < steps; i++) { 
      stepper.doStep();    
    }   

    _steps -= 400;
}
