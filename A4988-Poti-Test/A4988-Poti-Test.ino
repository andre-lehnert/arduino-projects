/*
 * MOTOR SETTING
 */
int stepsPerRound = 200;

/*
 * STEPS
 * - LOW-to-HIGH transition = one step
 */
int stepPin = 13; 

/*
 * DIRECTION
 * - LOW input state/ direction
 * - HIGH input state/ direction
 */
int dirPin = 22; 

/*
 * ENABLE
 * - HIGH input = turn OFF all outputs
 * - LOW input = turn ON all outputs
 */
int enablePin = 26;

/*
 * RESET
 * - sets the translator to a predefined Home state (e.g. 45° Full Step)
 * - turns of all outputs
 * - all STEPS inputs ignored until the RESET input is set to HIGH
 */
int resetPin = 10;

/*
 * SLEEP
 * - minimize energy consumption when the motor is not in use
 * - LOW input = SLEEP mode
 * - HIGH input = NORMAL operation
 */
int sleepPin = 9;

/*
 * MS1   MS2   MS3   Microstep Resolution
 * L     L     L     Full Step
 * H     L     L     Half Step
 * L     H     L     Quarter Step
 * H     H     L     Eighth Step
 * H     H     H     Sixteenth Step
 */
int ms1Pin = 8;
int ms2Pin = 7;
int ms3Pin = 6;


/*
 * -------------------------------------------------
 */

void setup() {
  
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(enablePin,OUTPUT);
  pinMode(resetPin,OUTPUT); 
  pinMode(sleepPin,OUTPUT); 
  pinMode(ms1Pin,OUTPUT); 
  pinMode(ms2Pin,OUTPUT); 
  pinMode(ms3Pin,OUTPUT); 

  digitalWrite(stepPin,HIGH);
  digitalWrite(dirPin,LOW);
  digitalWrite(enablePin,LOW);
  digitalWrite(resetPin,LOW);
  digitalWrite(sleepPin,LOW);
  digitalWrite(ms1Pin,LOW);
  digitalWrite(ms2Pin,LOW);
  digitalWrite(ms3Pin,LOW);

}

void loop() {

}

