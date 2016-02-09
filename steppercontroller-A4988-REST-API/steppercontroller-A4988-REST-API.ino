

/* * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * steppercontroller-A4988-REST-API
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * REST-API
 * GET /arduino/:function/:operation/:parameter
 * 
 * GET /arduino/stepper/up
 * GET /arduino/stepper/up/:steps
 * GET /arduino/stepper/down
 * GET /arduino/stepper/down/:steps
 * 
 * GET /arduino/stepper/stop
 * GET /arduino/stepper/sleep
 * GET /arduino/stepper/awake
 * GET /arduino/stepper/enable
 * GET /arduino/stepper/disable
 *
 * GET /arduino/stepper/fullstep
 * GET /arduino/stepper/halfstep
 * GET /arduino/stepper/quarterstep
 * GET /arduino/stepper/eigththstep
 * GET /arduino/stepper/sixteenthstep
 * 
 * GET /arduino/stepper/rpm/:rpm
 * GET /arduino/stepper/stepdelay/:delay
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Default REST-API
 * /arduino/digital/13 : calls digitalRead(13);
 * /arduino/digital/13/1 : calls digitalWrite(13,1);
 * /arduino/analog/9/123 : analogWrite(9,123);
 * /arduino/analog/2 : analogRead(2);
 * /arduino/mode/13/input : pinMode(13, INPUT);
 * /arduino/mode/13/output : pinMode(13, OUTPUT); 
 * 
 */

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

/*
 * Arduino Yun
 * REST-API
 * - Server accepts REQUESTs
 * - a REQUEST is a new Client
 * - Clients can RESPONSE
 */
BridgeServer server;
BridgeClient client;

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
int dirPin = 12; 

/*
 * ENABLE
 * - HIGH input = turn OFF all outputs
 * - LOW input = turn ON all outputs
 */
int enablePin = 11;

/*
 * RESET
 * - sets the translator to a predefined Home state (e.g. 45° Full Step)
 * - turns of all outputs
 * - all STEPS inputs ignored until the RESET input is set to HIGH
 */
int resetPin = 10; // usually connected with SLEEP (Jumper)

/*
 * SLEEP
 * - minimize energy consumption when the motor is not in use
 * - LOW input = SLEEP mode
 * - HIGH input = NORMAL operation
 */
int sleepPin = 9; // usually connected with RESET (Jumper)

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
 * Timeouts
 */
int stepDelay = 500; // MicroSeconds
int initDelay = 30; // MicroSeconds
const int minDelay = 100; // MicroSeconds

/*
 * State configuration
 */
boolean moveUp = true;
boolean stops = true;
boolean isAsleeped = false;
boolean isEnabled = false;
boolean isNewRequest = false;

/*
 * Step resolution
 */
enum StepMode {
  NONE, // = FULL
  FULL,
  HALF,
  QUARTER,
  EIGTHTH,
  SIXTEENTH
};

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

  // clear init 
  digitalWrite(stepPin,LOW);
  digitalWrite(dirPin,LOW);
  digitalWrite(enablePin,LOW);
  digitalWrite(resetPin,LOW);
  digitalWrite(sleepPin,LOW);
  digitalWrite(ms1Pin,LOW);
  digitalWrite(ms2Pin,LOW);
  digitalWrite(ms3Pin,LOW);

  // disable Controller while starting the Bridge
  disable();

  Bridge.begin();
  
  server.listenOnLocalhost();
  server.begin();
}

void loop() {

  // Is the last client request completet?
  if ( !isNewRequest ) {
    // Wait for new Request
    client = server.accept();
  }
  
  if (client) { 
    enable();
              
    process(client);   
    client.stop();

    disable();
  }
}

/*
 * -------------------------------------------------
 */

void process(BridgeClient client) {

  // 1. Read function 
  // /arduino/:function
  String function = client.readStringUntil('/');
  function.trim();

  logger("function", function);
  
  // Stepper
  if (function == "stepper") {

    // 2. Read operation 
    // /arduino/stepper/:operation
    String operation = client.readStringUntil('/');
    operation.trim();

    logger("operation", operation);

    // 3. Read parameter 
    // /arduino/stepper/:operation/:parameter
    String parameter = client.readStringUntil('/');
    parameter.trim();

    logger("parameter", parameter);

    // 4. Process operation
    if (operation == "up") {     
      if (parameter != NULL) { processUp( parameter.toInt() ); } else { processUp(); }
      
    } else if (operation == "down") {     
      if (parameter != NULL) { processDown( parameter.toInt() ); } else { processDown(); } 
           
    } else if (operation == "stop") {
      processStop(); 
      
    } else if (operation == "sleep") {
      if (parameter != NULL) { processSleep( parameter.toInt() ); } else { processSleep(); } 
      
    } else if (operation == "enable") {
      processEnable();
      
    } else if (operation == "disable") {
      processDisable();
      
    } else if (operation == "reset") {
      processReset();
      
    } else if (operation == "awake") {
      processAwake();
      
    } else if (operation == "fullstep") {
      processFullStep();
      
    } else if (operation == "halfstep") {
      processHalfStep();
      
    } else if (operation == "quarterstep") {
      processQuarterStep();
      
    } else if (operation == "eigththstep") {
      processEigththStep();
      
    } else if (operation == "sixteenthstep") {
      processSixteenthStep();
      
    } else if (operation == "rpm") {
      if (parameter != NULL) { processRPM( parameter.toInt() ); }
      
    } else if (operation == "stepdelay") {
      if (parameter != NULL) { processStepDelay( parameter.toInt() ); }
      
    }
    
  }

  // Request processed and wait for a new one
  isNewRequest = false; 
}

/*
 * -------------------------------------------------
 */

/*
 * REST ROUTES
 */
// GET arduino/stepper/up
void processUp() {

  while ( !hasNextRequest() ) {
    // do one round
    processUp(stepsPerRound);
  }   
}

// GET arduino/stepper/up/:steps
void processUp(int steps) {
  
  up();
   
  for(int x = 0; x < steps; x++) {
    doStep(); 
  }  
}

// GET arduino/stepper/down
void processDown() {
  while ( !hasNextRequest() ) {
    // do one round
    processDown(stepsPerRound);
  } 
}

// GET arduino/stepper/down/:steps
void processDown(int steps) {
  
  down();
   
  for(int x = 0; x < steps; x++) {
    doStep(); 
  }  
}

// GET arduino/stepper/stop
void processStop() {
  //sleep();
}

// GET arduino/stepper/reset 
void processReset() {
  reset();
}

// GET arduino/stepper/enable 
void processEnable() {
  enable();
}

// GET arduino/stepper/disable 
void processDisable() {
  disable();
}

// GET arduino/stepper/sleep 
void processSleep() {
  sleep();
}

// GET arduino/stepper/sleep/:delay 
// delay = Milliseconds
void processSleep(int delayMilliSeconds) {
  sleep(delayMilliSeconds);
}

// GET arduino/stepper/awake 
void processAwake() {
  awake();
}

// GET arduino/stepper/fullstep 
void processFullStep() {
  setStepMode(FULL);
}

// GET arduino/stepper/halfstep 
void processHalfStep() {
  setStepMode(HALF);
}

// GET arduino/stepper/quarterstep 
void processQuarterStep() {
  setStepMode(QUARTER);
}

// GET arduino/stepper/eighthstep 
void processEigththStep() {
  setStepMode(EIGTHTH);
}

// GET arduino/stepper/sixteenthstep 
void processSixteenthStep() {
  setStepMode(SIXTEENTH);
}

// GET arduino/stepper/rpm/:rpm 
void processRPM(int rpm) {
  setRPM(rpm);
}

// GET arduino/stepper/stepdelay/:delay 
void processStepDelay(int delayMicroSeconds) {
  setStepDelay(delayMicroSeconds);
}

/*
 * -------------------------------------------------
 */

/*
 * STEP PIN Methods
 * - possible singleSteps in different resolutions QUARTER,QUARTER,HALF,HALF,FULL, ...
 */
void doStep() {
  
  digitalWrite(stepPin,HIGH); 
  delayMicroseconds(stepDelay); 
  digitalWrite(stepPin,LOW); 
  delayMicroseconds(stepDelay); 
  
}

/*
 * SLEEP PIN Methods
 */
void sleep(int milliSeconds) {
  
  sleep();
  delay(milliSeconds);
  awake(); 
}

void awake() {
  digitalWrite(sleepPin,HIGH); 
  isAsleeped = false; 
}

void sleep() {  
  digitalWrite(sleepPin,LOW); 
  isAsleeped = true; 
}

/*
 * ENABLE PIN Methods
 */ 
void enable() {
  digitalWrite(enablePin,LOW); 
  isEnabled = true;
}

void disable() {
  digitalWrite(enablePin,HIGH); 
  isEnabled = false;
}

/*
 * RESET PIN Methods
 */
void reset() {
  digitalWrite(resetPin,LOW); 
  delayMicroseconds(minDelay);
  digitalWrite(resetPin,LOW); 
}

/*
 * DIRECTION PIN Methods
 */
void up() {  
   moveUp = true;
   digitalWrite(dirPin,LOW);
}

void down() {
  moveUp = false;
  digitalWrite(dirPin,HIGH);
}

/*
 * MS PINs Methods
 * Step resolution
 */
 void setStepMode(StepMode mode) {

    switch(mode) {
      case FULL: setFullStepMode(); break;
      case HALF: setHalfStepMode(); break;
      case QUARTER: setQuarterStepMode(); break;
      case EIGTHTH: setEigThthStepMode(); break;
      case SIXTEENTH: setSixteenthStepMode(); break;
      
      default:
        setFullStepMode();
    }
 }

void setFullStepMode() {
  digitalWrite(ms1Pin,LOW);
  digitalWrite(ms2Pin,LOW);
  digitalWrite(ms3Pin,LOW);
  logger("stepmode","full");
}

void setHalfStepMode() {
  digitalWrite(ms1Pin,HIGH);
  digitalWrite(ms2Pin,LOW);
  digitalWrite(ms3Pin,LOW);
  logger("stepmode","half");
}

void setQuarterStepMode() {
  digitalWrite(ms1Pin,LOW);
  digitalWrite(ms2Pin,HIGH);
  digitalWrite(ms3Pin,LOW);
  logger("stepmode","quarter");
}

void setEigThthStepMode() {
  digitalWrite(ms1Pin,HIGH);
  digitalWrite(ms2Pin,HIGH);
  digitalWrite(ms3Pin,LOW);
  logger("stepmode","eigthth");
}

void setSixteenthStepMode() {
  digitalWrite(ms1Pin,HIGH);
  digitalWrite(ms2Pin,HIGH);
  digitalWrite(ms3Pin,HIGH);
  logger("stepmode","sixteenth");
}

/*
 * -------------------------------------------------
 */

boolean hasNextRequest() {
  
  client = server.accept();
  isNewRequest = true;
  
  return client;
  
}
/*
 * Define stepDelay based on RPM
 */
void setRPM(int rpm) {

  long minute = 60000000; // MicroSeconds  
  long stepsPerMinute = rpm * stepsPerRound;

  // Delay between steps
  // stepDelay > minimum delay 
  stepDelay = (minute / stepsPerMinute) < minDelay ? minDelay : (minute / stepsPerMinute); // MicroSeconds
}

void setStepDelay(int delayMicroSeconds) {
  stepDelay = delayMicroSeconds;
}

/*
 * Calculates the neccessary steps per round
 */
void setStepsPerRound(double stepAngle) {
  stepsPerRound = (360 / stepAngle);
}

void logger(String key, String value) {
  //Bridge.put(key, value); // write on flash memory
  //logging[key] = value;
}



