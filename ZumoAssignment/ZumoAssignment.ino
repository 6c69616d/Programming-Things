#include <NewPing.h>
#include <Wire.h>
#include <ZumoShield.h>

//white 5v
//purple ground
//grey trig
//black echo

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 20 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define NUM_SENSORS 6 // Number of sensors that the ZumoReflectanceSensorArray has
#define QTR_THRESHOLD  800 // Reflectance from ZumoReflectanceSensorArray threshold. Used to know when the Zumo is on black.

ZumoMotors motors; //declare a variable called motors of type ZumoMotors so that we can set the speeds of the motors to control the robot
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
int ePressed = 0; // an integer variable to hold the amount of times e has been pressed
bool ignoreCommands = false; // bool so that commands to the Zumo can be switched on and off
bool passedRoom = false; // bool so that the Zumo knows if it has passed a room or not after the first leg of the T junction having reached one end
bool mainCorridor = false; // bool so that the Zumo knows if it has passed the main corridor or not after the first leg of the T junction having reached one end

void setup()  // a function that sets up the Zumo
{
  Serial.begin(9600); // initialize the serial communication
  pinMode(ledPin, OUTPUT); // initialize the LED pin as an output
  sensors.init(QTR_NO_EMITTER_PIN); // initialise the ZumoReflectanceSensorArray sensors
  sensors.calibrate(QTR_NO_EMITTER_PIN); // calibrate the ZumoReflectanceSensorArray sensors
  button.waitForButton(); // wait until that button is pressed to start moving
}

void loop() // a loop function that is always looping
{
  commands(); // function call for the commands sent from the GUI

  if (autonomous) // if the autonomous bool is set to true
  {
    motors.setSpeeds(100, 100); // set the motor speeds to 100
    autonomousMovement(); // function call to the autonomous movement functionality
  }
}

void commands() // a function that holds the commands functionality
{
  if (Serial.available() > 0) // see if there's incoming serial data
  {
    autonomous = ignoreCommands; // set the autonomous bool to false

    incomingBytes = Serial.readString(); // read the serial string data into the incoming bytes variable
    incomingBytes.trim(); // trim the string in the incomingBytes variable to remove any whitespace at the end of the string
    if (incomingBytes == "w") // if the incomingBytes string is set to w
    {
      motors.setSpeeds(150, 150); // Set both the motor speeds to 150
    }
    else if (incomingBytes == "a") // else if the incomingBytes string is set to a
    {
      if (!ignoreCommands) // if ignoreCommands is set to false
      {
        motors.setSpeeds(-125, 125); // Set left motor to 150 but set right motor to 150 in reverse making the Zumo turn left
      }
      else // if ignoreCommands is set to true
      {
        mainCorridor = true; // Set the mainCorridor to true so that the Zumo knows it has been passed
      }
    }
    else if (incomingBytes == "s") // else if the incomingBytes string is set to s
    {
      motors.setSpeeds(-150, -150); // Set both motors to 150 in reverse
    }
    else if (incomingBytes == "d") // else if incomingBytes string is set to d
    {
      if (!ignoreCommands) // if ignoreCommands is set to false
      {
        motors.setSpeeds(125, -125); // Set the left motor speed to 150 in reverse but the right motor to 150 forward making the Zumo turn right
      }
      else // if ignore commands is set to true
      {
        mainCorridor = true; // Set the mainCorridor to true so that the Zumo knows it has been passed
      }
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
      if (!ignoreCommands) // if ignoreCommands is set to false
      {
        motors.setSpeeds(150, -150); //set the left motor to a speed of 150 forward and the right motor to 150 in reverse to turn right into the room
        delay(800); // delay for 600 milliseconds
        roomSearch("right"); // function call of roomSearch passing through right so that the messages inside relate to right hand room
      }
      else // if ignoreCommands is set to true
      {
        passedRoom = true; // set passedRoom to true so that the Zumo knows it has been passed
      }
    }
    else if (incomingBytes == "Ro L") // else if the incomingBytes string is set to 'Ro L'
    {
      if (!ignoreCommands) // if ignoreCommands is set to false
      {
        motors.setSpeeds(-150, 150); //turn left into the room
        delay(800); // delay for 600 milliseconds
        roomSearch("left"); // function call of roomSearch passing through left so that the messages inside relate to left hand room
      }
      else // if ignoreCommands is set to true
      {
        passedRoom = true; // set passedRoom to true so that the Zumo knows it has been passed
      }
    }
    else if (incomingBytes == "e") // else if the incomingBytes string is set to e
    {
      Serial.println("End of T junction reached"); // write out a message to the GUI saying it has reached the end of a T junction

      ePressed++; // increment the amount of types the e key has been pressed so we know if it is at the second end of the T junction or not
      autonomous = false; // set autonomous to false to disable the autonomous functionality

      while (!autonomous && (ePressed != 2)) // while autonomous is set to false and the e key is not on the second press
      {
        commands(); // function call for the commands sent from the GUI
      }

      if (!passedRoom && !mainCorridor) // if the room has not been passed and the mainCorridor has not been passed
      {
        ignoreCommands = true; // set ignoreCommands so that the Zumo ignores the necessary commands
      }
      else // if the room has been passed and the main corridor has been passed
      {
        ignoreCommands = false; // set ignoreCommands so that Zumo accepts all commands
      }

      if (ePressed == 2) // if the key has been pressed twice
      {
        //Serial.println("in e == 2");
        for (String roomAndLocation : roomsAndLocations) // for every room and location stored in the roomsAndLocations array
        {
          Serial.println(roomAndLocation); // print out the contents of that element of the array
        }
        for (String roomWithObject : roomsWithObjects) // for every room with an object detected in the room
        {
          Serial.println(roomWithObject); // print out the contents of that element of the array
        }
      }
    }
  }
}

void autonomousMovement() // a function that holds the autonomous movement functionality
{
  sensors.read(sensor_values); // reading the ZumoReflectanceSensorArray sensor values into the sensor_value unsigned int array

  if ((sensor_values[0] > QTR_THRESHOLD && sensor_values[5] > QTR_THRESHOLD) ||
      (sensor_values[0] > QTR_THRESHOLD && sensor_values[1] > QTR_THRESHOLD) ||
      (sensor_values[4] > QTR_THRESHOLD && sensor_values[5] > QTR_THRESHOLD)) // if the two outer sensors or the two right most sensors or the two left most sensors are
    // greater than the QTR_THRESHOLD the zumo has hit a wall
  {
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
      motors.setSpeeds(0, 0); // set the motor speeds to 0
      delay(500); // delay for 500 milliseconds
      motors.setSpeeds(150, 0); // set the left motor to a speed of 150
      delay(250); // delay for 250 miliseconds
    }
    else if (sensor_values[5] > QTR_THRESHOLD) // if the right most sensor value is greater than the QTR_THRESHOLD then the robot has slightly touched a right wall
    {
      motors.setSpeeds(0, 0); // set the motor speeds to 0
      delay(500); // delay for 500 milliseconds
      motors.setSpeeds(0, 150); // set the right motor to a speed of 150
      delay(250); // delay for 250 milliseconds
    }
  }
}
void storeObjectDetected(int roomNumber) // a function that allows the Zumo to store rooms where objects have been detected
{
  String strRoomNumber = String(roomNumber); // convert the room number to a string
  String text = "Room: " + strRoomNumber + " Object Detected"; // create a string with the room number and an object has been detected
  roomsWithObjects[roomNumber - 1] = text; // add the string of room and object detected to the array of rooms with objects detected
}

void storeRoomLocations(int roomNumber, String location) // a function that allows the Zumo to hold the room numbers and their locations either left or right of the corridor
{
  String strRoomNumber = String(roomNumber); // convert the room number to a string
  String text = "Room: " + strRoomNumber + " Location: " + location + ""; // create a string with the room number and location
  roomsAndLocations[roomNumber - 1] = text; // add the string of room and location to the array of rooms and locations
}

void roomSearch(String location) // a function that holds the room searching functionality
{
  bool object = false;
  roomNumber++; // increment the room number
  String strRoomNumber = String(roomNumber); // convert the room number to a string
  Serial.println("Here is room number " + strRoomNumber + " and is located on the " + location + "\r\n"); //number the room and state whether on the left or right of the room in the GUI

  storeRoomLocations(roomNumber, location); //Zumo retains the room numbers and locations
  motors.setSpeeds(75, 75); // set both motors to a speed of 75
  delay(500); // delay for 500 milliseconds

  for (int i = 0; i < 80; i++) // for i is less than 100
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70)) // if i is greater than 40 and i is less than or equal to 60 or i is greater than 80 or less than or equal to 100
      motors.setSpeeds(-150, 150);
    else
      motors.setSpeeds(150, -150); // set the left motor to a speed of 150 forwards and the right to 150 in reverse to turn the Zumo right
    if (sonar.ping_cm() > 0) // if the ultrasonic sensor has a reading in centimetres greater than 0
    {
      object = true; // an object has been detected in the room
    }
    delay(20); // delay for 20 milliseconds
  }
  motors.setSpeeds(0, 0); // set both the motors to 0 bringing the Zumo to a stop

  if (object) // if an object has been detected
  {
    Serial.println("Object detected in room " + strRoomNumber + "\r\n"); // write a message to the GUI that an object was detected and in what room
    storeObjectDetected(roomNumber); // call the storeObjectDetected to store that an object has been detected and the room number
  }
  else
  {
    Serial.println("Nothing Detected"); // write a message to the GUI that nothing was detected
  }
}
