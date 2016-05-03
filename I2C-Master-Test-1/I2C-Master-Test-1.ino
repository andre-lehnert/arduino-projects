#include <Wire.h>

#define PAYLOAD_SIZE 3 // how many bytes to expect from each I2C salve node
#define NODE_MAX 1 // maximum number of slave nodes (I2C addresses) to probe
#define START_NODE 1 // The starting I2C address of slave nodes
#define NODE_READ_DELAY 2000 // Some delay between I2C node reads

int nodePayload[PAYLOAD_SIZE];

void setup() {
  Serial.begin(9600);  
  Serial.println("MASTER READER NODE");
  Serial.print("Maximum Slave Nodes: ");
  Serial.println(NODE_MAX);
  Serial.print("Payload size: ");
  Serial.println(PAYLOAD_SIZE);
  Serial.println("***********************");
  
  Wire.begin();        // Activate I2C link

}

void loop() {

  
  for (int nodeAddress = START_NODE; nodeAddress <= NODE_MAX; nodeAddress++) { // we are starting from Node address 2

    Wire.beginTransmission(nodeAddress); // transmit to device #9
    
    if (nodeAddress < 100) Wire.write(0);
    if (nodeAddress < 10) Wire.write(0);
    Wire.write(nodeAddress); // send id
      
    // position %;led pattern;red;green;blue;pattern speed %
    Wire.write(";");
    Wire.write("100;");
    Wire.write("loading;");
    Wire.write("0;0;150;");
    Wire.write("10"); 
    
    Wire.endTransmission();    // stop transmitting

    delay(NODE_READ_DELAY);

  }
  delay(NODE_READ_DELAY);

}
