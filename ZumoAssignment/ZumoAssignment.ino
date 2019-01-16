#include <NewPing.h>
#include <Wire.h>
#include <ZumoShield.h>

//white 5v
//purple ground
#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.grey
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor. black
#define MAX_DISTANCE 50 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

ZumoMotors motors; //declare a variable called motors of type ZumoMotors so that we can set the speeds of the motors to control the robot
ZumoBuzzer buzzer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

const int ledPin = 13; // the pin that the LED is attached to    
String incomingBytes; // a variable to read incoming serial data into

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {

  if (Serial.available() > 0) // see if there's incoming serial data:
  {
    incomingBytes = Serial.readString(); // read the serial string data into the incoming bytes variable
    incomingBytes.trim(); // trim the string in the incomingBytes variable to remove any whitespace at the end of the string
       
    if (incomingBytes == "w") // if the incomingBytes string is set to w
    {
      motors.setSpeeds(200, 200); // Set both the motor speeds to 200
    }
    else if (incomingBytes == "a") // else if the incomingBytes string is set to a
    {
      motors.setSpeeds(150, -150); // Set left motor to 150 but set right motor to 150 in reverse making the Zumo turn left
    }   
    else if (incomingBytes == "s") // else if the incomingBytes string is set to s
    {
      motors.setSpeeds(-150, -150); // Set both motors to 150 in reverse
    }
    else if (incomingBytes == "d") // else if incomingBytes string is set to d
    {
      motors.setSpeeds(-150, 150); // Set the left motor speed to 150 in reverse but the right motor to 150 forward making the Zumo turn right
    }
    else if (incomingBytes == "") // else if the incomingBytes string is empty
    { 
      motors.setSpeeds(0, 0); // Set the motor speeds to 0 bringing the Zumo to a stop
    }
    else if (incomingBytes == "c") // else if the incomingBytes string is set to c
    {
      
    }
    else if (incomingBytes == "Ro R") // else if the incomingBytes string to set to 'Ro R'
    {
      motors.setSpeeds(200, 200);
      Serial.println("hello world");
    }
    else if (incomingBytes == "Ro L") // else if the incomingBytes string is set to 'Ro L'
    {
      
    }
    else if (incomingBytes == "e") // else if the incomingBytes string is set to e
    {
      
    }

    
  }


  
}
