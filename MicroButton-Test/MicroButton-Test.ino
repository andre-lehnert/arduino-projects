
#include <Stepper-A4988.h>

A4988 stepper(42, 44, 40, 46, 48, 50, 400, 500); // step, direction, enable pin
// A4988( stepPin, directionPin, enablePin, ms1Pin, ms2Pin,  ms3Pin, stepsPerRound, stepDelay);

int sensorPin = A0;  
int sensorValue = 0;

int stepsUpwardsAfterContact = 6 * 200;
int steps = 10; // spinning resolution

boolean slow = true;


int currentPosition = 1000; // state
int thresholdPosition = 100; // starts slow mode




void setup() {
  //Serial.begin(9600);  
}

void loop() {
  
  sensorValue = analogRead(sensorPin);

  // contact
  if (sensorValue > 800) {

      //Serial.print("Move up\n");

      stepper.up();
      stepper.setHalfStepMode();
      //stepper.setFullStepMode();
      
      // move up
      for (int i = 0; i < stepsUpwardsAfterContact; i++) {

        if (i < 200) { slow = true; } else { slow = false; }

       // if (! slow) stepper.setFullStepMode();        
        
        stepper.doStep();
      }

      // postion after moving upwards
      currentPosition = stepsUpwardsAfterContact;
    
  } else {    
    
    stepper.down();
    //stepper.setFullStepMode();
    stepper.setHalfStepMode();
    slow = false;
    
      // move down
      int smallRounds = 10;
      for (int a = 0; a < stepsUpwardsAfterContact; a++) {

        stepper.doStep();
        currentPosition--;
        
        if (currentPosition < thresholdPosition && ! slow) {          
          slow = true;

          stepper.setHalfStepMode();
        
          for (int a = 0; a < steps; a++) {
  
            stepper.doStep();
            currentPosition--;
            
            sensorValue = analogRead(sensorPin);
  
            // contact
            if (sensorValue > 800) {  
              // startposition
              currentPosition = 0;
              break;
            }
          }

          
          break;
        }
      }
     
      
 
      

      //Serial.print("Move down\n");
  }


  //Serial.print(sensorValue);
  //Serial.print("\n");
  

  //delay(10);
}
