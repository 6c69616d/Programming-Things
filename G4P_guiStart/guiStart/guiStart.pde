import processing.serial.*;
import g4p_controls.*;

Serial serialPort;

final char LF = 10;
String message;

public void setup() {
  size(650, 400);
  createGUI();

  String xBeePort = "COM4";
  serialPort = new Serial(this, xBeePort, 9600);
}

public void draw() {
  background(200, 200, 200);
  if (serialPort.available() > 0) 
  { 
    message = serialPort.readStringUntil(LF);
    println(message);
    //message = message.replace("\r\n", "");
    if (message!=null)
    {
      String text = textarea1.getText();
      if (text == "Messages from Zumo")
      {
         textarea1.setText(message);
      }
      else
      {
             textarea1.appendText(message); 
      }

    }
  }
}
