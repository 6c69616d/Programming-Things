import processing.serial.*;
import g4p_controls.*;

Serial serialPort;
int val;
int bgcol = 15;

public void setup() {
  size(500, 360);
  createGUI();
  
  String xBeePort = "COM4";
  serialPort = new Serial(this, xBeePort, 9600);
}

public void draw() {
  background(200, 200, 200);
  while (serialPort.available() > 0) {
    textarea1.setText("");
    String incomingBytes = serialPort.readString(); // read the serial string data into the incoming bytes variable
    //incomingBytes.trim(); // trim the string in the incomingBytes variable to remove any whitespace at the end of the string
    textarea1.setText(incomingBytes);
    
   
  }

}
