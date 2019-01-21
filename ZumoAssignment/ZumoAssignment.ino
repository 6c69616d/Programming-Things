#include <NewPing.h>
#include <Wire.h>
#include <ZumoShield.h>

//white 5v
//purple ground
#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.grey
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor. black
#define MAX_DISTANCE 50 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define NUM_SENSORS 6
#define QTR_THRESHOLD  800

ZumoMotors motors; //declare a variable called motors of type ZumoMotors so that we can set the speeds of the motors to control the robot
ZumoBuzzer buzzer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

const int ledPin = 13; // the pin that the LED is attached to
String incomingBytes; // a variable to read incoming serial data into
int roomNumber = 0; // integer variable to store the room number
bool autonomous = true;
String roomsAndLocationsList[10];
String roomsWithObjects[10];

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  sensors.init();
  sensors.calibrate();
}

void loop() {

  commands();

  if (autonomous) {
    motors.setSpeeds(100, 100);
    autonomousMovement();
  }
  else {

  }

}

void commands()
{
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
      motors.setSpeeds(-150, 150); // Set left motor to 150 but set right motor to 150 in reverse making the Zumo turn left
    }
    else if (incomingBytes == "s") // else if the incomingBytes string is set to s
    {
      motors.setSpeeds(-150, -150); // Set both motors to 150 in reverse
    }
    else if (incomingBytes == "d") // else if incomingBytes string is set to d
    {
      motors.setSpeeds(150, -150); // Set the left motor speed to 150 in reverse but the right motor to 150 forward making the Zumo turn right
    }
    else if (incomingBytes == "") // else if the incomingBytes string is empty
    {
      motors.setSpeeds(0, 0); // Set the motor speeds to 0 bringing the Zumo to a stop
    }
    else if (incomingBytes == "c") // else if the incomingBytes string is set to c
    {
      Serial.println("Complete - resuming autonomously");
      autonomous = true;
    }
    else if (incomingBytes == "Ro R") // else if the incomingBytes string to set to 'Ro R'
    {
      motors.setSpeeds(150, -150); //turn right into the room
      roomSearch("right");
    }
    else if (incomingBytes == "Ro L") // else if the incomingBytes string is set to 'Ro L'
    {
      motors.setSpeeds(-150, 150); //turn left into the room
      roomSearch("left");
    }
    else if (incomingBytes == "e") // else if the incomingBytes string is set to e
    {

    }
  }
}

void autonomousMovement()
{
  sensors.read(sensor_values);
  Serial.println(sensor_values[0]);
  Serial.println(sensor_values[1]);
  Serial.println(sensor_values[2]);
  Serial.println(sensor_values[3]);
  Serial.println(sensor_values[4]);
  Serial.println(sensor_values[5]);

  //if (sensor_values[2] > QTR_THRESHOLD && sensor_values[3] > QTR_THRESHOLD)
  //if ((sensor_values[0] < QTR_THRESHOLD && sensor_values[5] < QTR_THRESHOLD && (sensor_values[2] > QTR_THRESHOLD || sensor_values[3] >  QTR_THRESHOLD)))
  if ((sensor_values[0] > QTR_THRESHOLD && sensor_values[5] > QTR_THRESHOLD) || (sensor_values[0] > QTR_THRESHOLD && sensor_values[1] > QTR_THRESHOLD) || (sensor_values[4] > QTR_THRESHOLD && sensor_values[5] > QTR_THRESHOLD))
  {
    Serial.println("in if - stopping");
    motors.setSpeeds(0, 0);
    delay(500);
    Serial.println("enter a command");
    autonomous = false;
  }
  else
  {
    delay(50);
    if (sensor_values[0] > QTR_THRESHOLD)
    {
      Serial.println("in else if - turning right");
      motors.setSpeeds(0, 0);
      delay(500);
      //delay(1000);
      motors.setSpeeds(150, 0);
      delay(250);
      //motors.setSpeeds(50, 50);
      //delay(250);
    }
    else if (sensor_values[5] > QTR_THRESHOLD)
    {
      Serial.println("in else if - turning left");
      //delay(1000);
      motors.setSpeeds(0, 0);
      delay(500);
      motors.setSpeeds(0, 150);
      delay(250);
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
  roomsAndLocationsList[roomNumber - 1] = text; // add the string of room and location to the array of rooms and locations
}

void roomSearch(String location) {
  roomNumber = roomNumber++; // increment the room number
  String strRoomNumber = String(roomNumber); // convert the room number to a string
  Serial.println("Here is room number " + strRoomNumber + " and is located on the " + location + "\r\n"); //number the room and state whether on the left or right of the room in the GUI

  storeRoomLocations(roomNumber, location); //Zumo retains the room numbers and locations
  motors.setSpeeds(150, 150);
  int pingcm = sonar.ping_cm();
  if (pingcm > 0)       //see if it detects anything
  {
    Serial.println("Object detected in room " + strRoomNumber + "\r\n");
    storeObjectDetected(roomNumber);
  }
  else      //if not detected go further into the room
  {

    motors.setSpeeds(150, -150);
    delay(20);
    motors.setSpeeds(0, 0);
    if (pingcm > 0)
    {
      Serial.println("Object detected in room " + strRoomNumber + "\r\n");
      storeObjectDetected(roomNumber);
    }
    else
    {
      Serial.println("Nothing detected");
    }
  }
  //Zumo to stop and wait for manual control
}
