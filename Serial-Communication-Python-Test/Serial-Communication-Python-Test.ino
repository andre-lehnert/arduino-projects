


String in = "";
String stepper = "-1";
String dir = "none";

void setup() {
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {

    in = Serial.readString();

    stepper = in.substring(0,1);
    dir = in.substring(2);

    String myString ="Stepper: " + stepper + ", Direction: " + dir + "\r\n";
    char* buf = (char*) malloc(sizeof(char)*myString.length()+1);
    myString.toCharArray(buf, myString.length()+1);  
    Serial.println(buf);
    free(buf);

    

  }

}
