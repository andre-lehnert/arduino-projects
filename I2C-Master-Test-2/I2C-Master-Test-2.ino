#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <Wire.h>

#define NODE_MAX 1 // maximum number of slave nodes (I2C addresses) to probe
//#define START_NODE 1 // The starting I2C address of slave nodes
#define SLAVE_NODE 1


/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * REST-API
 * GET /arduino/:function/:operation/:parameter
 * 
 * GET /arduino/stepper/up
 * GET /arduino/stepper/up/:steps
 * GET /arduino/stepper/down
 * GET /arduino/stepper/down/:steps
 */

BridgeServer server;
BridgeClient client;

boolean isNewRequest = false;

void setup() {
  
  Bridge.begin();
  pinMode(13, OUTPUT);
  server.listenOnLocalhost();
  server.begin();

  Wire.begin();        // Activate I2C link
}

void loop() {
  

    // Wait for new Request
    client = server.accept();
  
  
  if (client) {               
    process(client);   
    client.stop();
  }

}

void process(BridgeClient client) {
 
  // 1. Read function 
  // /arduino/:function
  String function = client.readStringUntil('/');
  function.trim();

  digitalWrite(13, HIGH);
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
    if (operation == "moveto") {     
      if (parameter != NULL) { processMoveTo( parameter.toInt() ); } else { processMoveTo(); } 
           
    } else if (operation == "stop") {
      processStop(); 
      
    } else if (operation == "init") {
      processInit();      
    } 

  // LED
  } else if (function == "led") {

    // 2. Read operation 
    // /arduino/led/:operation
    String operation = client.readStringUntil('/');
    operation.trim();

    logger("operation", operation);
     
    // 3. Read parameter 
    // /arduino/led/:operation/:parameter
    String r = client.readStringUntil('-');
    r.trim();
    String g = client.readStringUntil('-');
    g.trim();
    String b = client.readStringUntil('/');
    b.trim();
  
    // 4. Process operation
    if (operation == "load") {     
      if (b != NULL) { processLoad( r.toInt(),g.toInt(),b.toInt() ); } else { processLoad(); }   
        
    } else  if (operation == "dimm") {     
      if (b != NULL) { processDimm( r.toInt(),g.toInt(),b.toInt() ); } else { processDimm(); }  
              
    }
    
  }

  digitalWrite(13, LOW);
}

/*
 * -------------------------------------------------
 */

boolean hasNextRequest() {
  
  client = server.accept();
  isNewRequest = true;
  
  return client;
  
}

void logger(String key, String value) {
  //Bridge.put(key, value); // write on flash memory
  //logging[key] = value;
}


/*
 * REST ROUTES
 */
// GET arduino/stepper/moveto/:parameter
void processMoveTo() {
  processMoveTo(0);
}

void processMoveTo(int parameter) {

    Wire.beginTransmission(SLAVE_NODE); // transmit to device #SLAVE_NODE
    
    if (SLAVE_NODE < 100) Wire.write(0);
    if (SLAVE_NODE < 10) Wire.write(0);
    Wire.write(SLAVE_NODE); // send id
    Wire.write(";");  

    Wire.write("stepper..."); // operation
    Wire.write(";");
  
    Wire.write("moveto...."); // operation
    Wire.write(";");

    if (parameter < 100) Wire.write(0);
    if (parameter < 10) Wire.write(0);
    Wire.write(parameter); // parameter
    Wire.write(";");    
    
    Wire.endTransmission();    // stop transmitting  
}

// GET arduino/stepper/stop
void processStop() {

    Wire.beginTransmission(SLAVE_NODE); // transmit to device #SLAVE_NODE
    
    if (SLAVE_NODE < 100) Wire.write(0);
    if (SLAVE_NODE < 10) Wire.write(0);
    Wire.write(SLAVE_NODE); // send id
    Wire.write(";");  

    Wire.write("stepper..."); // operation
    Wire.write(";");
   
    Wire.write("stop......"); // operation
    Wire.write(";");    
    
    Wire.endTransmission();    // stop transmitting  
}

// GET arduino/stepper/init
void processInit() {
  
    Wire.beginTransmission(SLAVE_NODE); // transmit to device #SLAVE_NODE
    
    if (SLAVE_NODE < 100) Wire.write(0);
    if (SLAVE_NODE < 10) Wire.write(0);
    Wire.write(SLAVE_NODE); // send id
    Wire.write(";");  

    Wire.write("stepper..."); // operation
    Wire.write(";");
   
    Wire.write("init......"); // operation
    Wire.write(";");    
    
    Wire.endTransmission();    // stop transmitting
}

// GET arduino/led/load/
void processLoad() {
    processLoad(150,0,0);
}

// GET arduino/led/load/255-255-0/
void processLoad(int r, int g, int b) {
  
    Wire.beginTransmission(SLAVE_NODE); // transmit to device #SLAVE_NODE
    
    if (SLAVE_NODE < 100) Wire.write(0);
    if (SLAVE_NODE < 10) Wire.write(0);
    Wire.write(SLAVE_NODE); // send id
    Wire.write(";");  

    Wire.write("led......."); // operation
    Wire.write(";");
   
    Wire.write("load......"); // operation
    Wire.write(";");    
    
    if (r < 100) Wire.write(0);
    if (r < 10) Wire.write(0);
    Wire.write(r); // operation


    if (g < 100) Wire.write(0);
    if (g < 10) Wire.write(0);
    Wire.write(g); // operation


    if (b < 100) Wire.write(0);
    if (b < 10) Wire.write(0);
    Wire.write(b); // operation

    Wire.endTransmission();    // stop transmitting
}

// GET arduino/led/dimm/
void processDimm() {  
    processDimm(150,150,150);
}

// GET arduino/led/dimm/255-255-0/
void processDimm(int r, int g, int b) {
  
    Wire.beginTransmission(SLAVE_NODE); // transmit to device #SLAVE_NODE
    
    if (SLAVE_NODE < 100) Wire.write(0);
    if (SLAVE_NODE < 10) Wire.write(0);
    Wire.write(SLAVE_NODE); // send id
    Wire.write(";");  

    Wire.write("led......."); // operation
    Wire.write(";");
    
    Wire.write("dimm......"); // operation
    Wire.write(";");    

    if (r < 100) Wire.write(0);
    if (r < 10) Wire.write(0);
    Wire.write(r); // operation
 

    if (g < 100) Wire.write(0);
    if (g < 10) Wire.write(0);
    Wire.write(g); // operation


    if (b < 100) Wire.write(0);
    if (b < 10) Wire.write(0);
    Wire.write(b); // operation

    Wire.endTransmission();    // stop transmitting
}


