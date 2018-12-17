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

}
