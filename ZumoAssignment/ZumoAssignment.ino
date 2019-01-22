#include <NewPing.h>
#include <Wire.h>
#include <ZumoShield.h>

//white 5v
//purple ground
#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.grey
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor. black
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define NUM_SENSORS 6 // Number of sensors that the ZumoReflectanceSensorArray has
#define QTR_THRESHOLD  800 // Reflectance from ZumoReflectanceSensorArray threshold. Used to know when the Zumo is on black.

ZumoMotors motors; //declare a variable called motors of type ZumoMotors so that we can set the speeds of the motors to control the robot
ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON); // declare a variable so that the Pushbutton on the zumo can be used
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN); // ZumoReflectanceSensorArray setup with no emitter pin so we can access the ZumoReflectanceSensorArray sensors.

unsigned int sensor_values[NUM_SENSORS]; // An array of type unsigned int that holds the sensor values readings from ZumoReflectanceSensorArray.
const int ledPin = 13; // the pin that the LED is attached to
String incomingBytes; // a variable to read incoming serial data into
int roomNumber = 0; // integer variable to store the room number
bool autonomous = true; // bool so the autonomous functionality can be switched on and off
String roomsAndLocations[10]; // an array to hold the rooms and locations
String roomsWithObjects[10]; // an array to hold the rooms with objects

void setup() {
  Serial.begin(9600); // initialize the serial communication
  pinMode(ledPin, OUTPUT); // initialize the LED pin as an output
  sensors.init(); // initialise the ZumoReflectanceSensorArray sensors
  sensors.calibrate(); // calibrate the ZumoReflectanceSensorArray sensors
  button.waitForButton(); // wait until that button is pressed to start moving
}

void loop() {

  commands(); // function call for the commands sent from the GUI
        Serial.print("Loop ");
        Serial.println(sonar.ping_cm());  
  if (autonomous) // if the autonomous bool is set to true
  {
    motors.setSpeeds(100, 100); // set the motor speeds to 100
    autonomousMovement(); // function call to the autonomous movement functionality
  }
  else {

  }

}

void commands()
{
  if (Serial.available() > 0) // see if there's incoming serial data:
  {
    autonomous = false; // set the autonomous bool to false
    incomingBytes = Serial.readString(); // read the serial string data into the incoming bytes variable
    incomingBytes.trim(); // trim the string in the incomingBytes variable to remove any whitespace at the end of the string

    if (incomingBytes == "w") // if the incomingBytes string is set to w
    {
      motors.setSpeeds(100, 100); // Set both the motor speeds to 200
    }
    else if (incomingBytes == "a") // else if the incomingBytes string is set to a
    {
      motors.setSpeeds(-150, 150); // Set left motor to 150 but set right motor to 150 in reverse making the Zumo turn left
    }
    else if (incomingBytes == "s") // else if the incomingBytes string is set to s
    {
      motors.setSpeeds(-150, -150); // Set both motors to 150 in reverse
    }
    else if (incomingBytes == "d") // else if incomingBytes string is set to d
    {
      Serial.println("turning right");
      motors.setSpeeds(100, -100); // Set the left motor speed to 150 in reverse but the right motor to 150 forward making the Zumo turn right
    }
    else if (incomingBytes == "") // else if the incomingBytes string is empty
    {
      motors.setSpeeds(0, 0); // Set the motor speeds to 0 bringing the Zumo to a stop
    }
    else if (incomingBytes == "c") // else if the incomingBytes string is set to c
    {
      Serial.println("Complete - resuming autonomously"); // write a message to the GUI saying Complete - resuming autonomously
      autonomous = true; // set the autonomous bool to true to switch on the autonomous functionality
    }
    else if (incomingBytes == "Ro R") // else if the incomingBytes string to set to 'Ro R'
    {
      for (int i = 0; i < 10; ++i) {
        Serial.print("Ro R ");
        Serial.println(String(sonar.ping_cm()));   
      }
      return;
      motors.setSpeeds(150, -150); //turn right into the room
      roomSearch("right"); // function call of roomSearch passing through right so that the messages inside relate to right hand room
    }
    else if (incomingBytes == "Ro L") // else if the incomingBytes string is set to 'Ro L'
    {
      motors.setSpeeds(-150, 150); //turn left into the room
      roomSearch("left"); // function call of roomSearch passing through left so that the messages inside relate to left hand room
    }
    else if (incomingBytes == "e") // else if the incomingBytes string is set to e
    {

    }
  }
}

void autonomousMovement()
{
  sensors.read(sensor_values); // reading the ZumoReflectanceSensorArray sensor values into the sensor_value unsigned int array
  //  Serial.println(sensor_values[0]);
  //  Serial.println(sensor_values[1]);
  //  Serial.println(sensor_values[2]);
  //  Serial.println(sensor_values[3]);
  //  Serial.println(sensor_values[4]);
  //  Serial.println(sensor_values[5]);

  //if (sensor_values[2] > QTR_THRESHOLD && sensor_values[3] > QTR_THRESHOLD)
  //if ((sensor_values[0] < QTR_THRESHOLD && sensor_values[5] < QTR_THRESHOLD && (sensor_values[2] > QTR_THRESHOLD || sensor_values[3] >  QTR_THRESHOLD)))
  if ((sensor_values[0] > QTR_THRESHOLD && sensor_values[5] > QTR_THRESHOLD) ||
      (sensor_values[0] > QTR_THRESHOLD && sensor_values[1] > QTR_THRESHOLD) ||
      (sensor_values[4] > QTR_THRESHOLD && sensor_values[5] > QTR_THRESHOLD)) // if the two outer sensors or the two right most sensors or the two left most sensors are
    //   greater than the QTR_THRESHOLD the zumo has hit a wall
  {
    //Serial.println("in if - stopping");
    motors.setSpeeds(0, 0); // set the motor speeds to 0 making the zumo stop
    delay(500); // delay for 500 milliseconds
    Serial.println("a wall or corner has been hit"); // write a message to the GUI saying a wall or corner has been hit
    Serial.println("enter a command"); // write a message to the GUI telling the user to enter a command
    autonomous = false; // set autonomous bool to false turn the functionality off
  }
  else
  {
    delay(50); // delay for 50 milliseconds
    if (sensor_values[0] > QTR_THRESHOLD) // if the left most sensor value is greater than the QTR_THRESHOLD then the robot has slightly touched a left wall
    {
      //Serial.println("in else if - turning right");
      motors.setSpeeds(0, 0); // set the motor speeds to 0
      delay(500); // delay for 500 milliseconds
      //delay(1000);
      motors.setSpeeds(150, 0); // set the left motor to a speed of 150
      delay(250); // delay for 250 miliseconds
      //motors.setSpeeds(50, 50);
      //delay(250);
    }
    else if (sensor_values[5] > QTR_THRESHOLD) // if the right most sensor value is greater than the QTR_THRESHOLD then the robot has slightly touched a right wall
    {
      //Serial.println("in else if - turning left");
      //delay(1000);
      motors.setSpeeds(0, 0); // set the motor speeds to 0
      delay(500); // delay for 500 milliseconds
      motors.setSpeeds(0, 150); // set the right motor to a speed of 150
      delay(250); // delay for 250 milliseconds
      //motors.setSpeeds(50, 50);
      //delay(250);
    }
    //motors.setSpeeds(50, 50);
    //else {
    //      Serial.println("in else - moving forward");
    //      motors.setSpeeds(50,50);
    //      //delay(1000);
    //      }
  }
}
void storeObjectDetected(int roomNumber)
{
  String strRoomNumber = String(roomNumber); // convert the room number to a string
  String text = "Room: " + strRoomNumber + " Object Detected"; // create a string with the room number and an object has been detected
  roomsWithObjects[roomNumber - 1] = text; // add the string of room and object detected to the array of rooms with objects detected
}

void storeRoomLocations(int roomNumber, String location)
{
  String strRoomNumber = String(roomNumber); // convert the room number to a string
  String text = "Room: " + strRoomNumber + " Location: " + location + ""; // create a string with the room number and location
  roomsAndLocations[roomNumber - 1] = text; // add the string of room and location to the array of rooms and locations
}

void roomSearch(String location) {
  bool search = true;
  roomNumber++; // increment the room number
  String strRoomNumber = String(roomNumber); // convert the room number to a string
  Serial.println("Here is room number " + strRoomNumber + " and is located on the " + location + "\r\n"); //number the room and state whether on the left or right of the room in the GUI

  storeRoomLocations(roomNumber, location); //Zumo retains the room numbers and locations
  motors.setSpeeds(75, 75);
  delay(500);
      motors.setSpeeds(100, -100);
  Serial.println("Spinning?");
  while (true)
  {
   int pingcm = sonar.ping_cm();
    int i = 0;

   Serial.println("Pingcm? " + pingcm);
    if (pingcm > 0) {
      Serial.println("Object detected in room " + strRoomNumber + "\r\n");
      //storeObjectDetected(roomNumber);
      motors.setSpeeds(0, 0);
      break;
    }
    if (i == 5) {
      motors.setSpeeds(0, 0);
      Serial.println("Nothing Detected");
      break;
    }
    i++;
  }
  motors.setSpeeds(0, 0);

}
//Zumo to stop and wait for manual control
