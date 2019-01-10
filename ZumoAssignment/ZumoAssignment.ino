#include <NewPing.h>

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define rev_distance 100

#include <Wire.h>
#include <ZumoShield.h>


ZumoMotors motors;
ZumoBuzzer buzzer;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

const int ledPin = 13; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into
String incomingBytes;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {

  delay(250);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
//  Serial.print("Ping: ");
//  Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
//  Serial.println("cm");

//  if(sonar.ping() < MAX_DISTANCE){
//    Serial.print("CLOSE");
//    motors.setSpeeds(-100, -100);
//  }
//  else {
//    motors.setSpeeds(100,100);
//  }
  
  
  // see if there's incoming serial data:
  if (Serial.available() > 0) 
  {
    // read the oldest byte in the serial buffer:
    //incomingByte = Serial.read();
    incomingBytes = Serial.readString();
    incomingBytes.trim();
    
    //Serial.println(incomingByte);
    Serial.println("'" + incomingBytes + "'");
    
    if (incomingBytes == "w") {
      motors.setSpeeds(200, 200);
      
    }
    else if (incomingBytes == "a") {
      motors.setSpeeds(150, -150);
    }   
    else if (incomingBytes == "s") {
      motors.setSpeeds(-150, -150);
    }

    else if (incomingBytes == "d") {
      motors.setSpeeds(-150, 150);
    }
    //STOP ZUMO
    else if (incomingBytes == "") {
      motors.setSpeeds(0, 0);
    }
    
    else if (incomingBytes == "c") {
      
    }

    else if (incomingBytes == "Ro R") {
      motors.setSpeeds(200, 200);
      Serial.println("hello world");
    }

    else if (incomingBytes == "Ro L") {
      
    }

    else if(incomingBytes == "e"){
      
    }

    
  }


  
}
