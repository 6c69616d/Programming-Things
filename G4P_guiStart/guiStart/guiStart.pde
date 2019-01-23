import processing.serial.*;
import g4p_controls.*;

Serial serialPort;
int val;
int bgcol = 15;
String message;
public void setup() {
  size(1000, 720);
  createGUI();

  String xBeePort = "COM4";
  serialPort = new Serial(this, xBeePort, 9600);
}

public void draw() {
  background(200, 200, 200);
  if (serialPort.available() > 0) 
  { 
    message = serialPort.readString();
    println(message);
    message = message.replace("\r\n", "");
    if (message!=null)
    {
      String text = textarea1.getText();
      if (text == "Messages from Zumo")
      {
        textarea1.setText("");
      }
      textarea1.appendText(message);
    }
  }
}
