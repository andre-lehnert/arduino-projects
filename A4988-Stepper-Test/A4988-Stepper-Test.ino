#include <Stepper-A4988.h>

A4988 stepper_1(26, 24, 22); // step, direction, enable pin
A4988 stepper_2(25, 23, 27); // step, direction, enable pin
A4988 stepper_3(30, 32, 28); // step, direction, enable pin

void setup() { 
  stepper_1.enable();
  stepper_1.up();

  stepper_2.enable();
  stepper_2.up();
  
  stepper_3.enable();
  stepper_3.up();


  stepper_3.disable();
}


void loop() {

  for (int i = 0; i < 2000; i++) { // 200 steps
    stepper_1.doStep();  
    //stepper_2.doStep();
    //stepper_3.doStep();
  }
  
  

  stepper_1.changeDirection();
  //stepper_2.changeDirection();
  //stepper_3.changeDirection();

  stepper_2.disable();
  stepper_3.disable();

  delay(1000);
}
