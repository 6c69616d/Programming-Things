/* =========================================================
 * ====                   WARNING                        ===
 * =========================================================
 * The code in this tab has been generated from the GUI form
 * designer and care should be taken when editing this file.
 * Only add/edit code inside the event handlers i.e. only
 * use lines between the matching comment tags. e.g.

 void myBtnEvents(GButton button) { //_CODE_:button1:12356:
     // It is safe to enter your event code here  
 } //_CODE_:button1:12356:
 
 * Do not rename this tab!
 * =========================================================
 */

public void textarea1_change1(GTextArea source, GEvent event) { //_CODE_:textarea1:653858:
  println("textarea1 - GTextArea >> GEvent." + event + " @ " + millis());
  String messaPort.readString();
  textarea1.setText(message);
} //_CODE_:textarea1:653858:

public void button1_click1(GButton source, GEvent event) { //_CODE_:button1:500642:
  println("button1 - GButton >> GEvent." + event + " @ " + millis());
  String text = textfield1.getText();
  println(text);
  serialPort.write(text);
  textfield1.setText("");
} //_CODE_:button1:500642:

public void textfield1_change1(GTextField source, GEvent event) { //_CODE_:textfield1:568846:
  println("textfield1 - GTextField >> GEvent." + event + " @ " + millis());
} //_CODE_:textfield1:568846:
//left
public void button2_click1(GButton source, GEvent event) { //_CODE_:button2:308221:
  println("button2 - GButton >> GEvent." + event + " @ " + millis());
   serialPort.write("a"); 
} //_CODE_:button2:308221:
//right
public void button3_click1(GButton source, GEvent event) { //_CODE_:button3:604378:
  println("button3 - GButton >> GEvent." + event + " @ " + millis());
    serialPort.write("d");
} //_CODE_:button3:604378:
//forward
public void button4_click1(GButton source, GEvent event) { //_CODE_:button4:348760:
  println("button4 - GButton >> GEvent." + event + " @ " + millis());
  serialPort.write("w");
} //_CODE_:button4:348760:
//back
public void button5_click1(GButton source, GEvent event) { //_CODE_:button5:984108:
  println("button5 - GButton >> GEvent." + event + " @ " + millis());
  serialPort.write("s");
} //_CODE_:button5:984108:
public void button6_click1(GButton source, GEvent event) { //_CODE_:button6:984108:
  println("button6 - GButton >> GEvent." + event + " @ " + millis());
  serialPort.write(" ");
} //_CODE_:button6:984108:
public void button7_click1(GButton source, GEvent event) { //_CODE_:button7:984108:
  println("button7 - GButton >> GEvent." + event + " @ " + millis());
  serialPort.write("r");
} //_CODE_:button:984108:


// Create all the GUI controls. 
// autogenerated do not edit
public void createGUI(){
  G4P.messagesEnabled(false);
  G4P.setGlobalColorScheme(GCScheme.BLUE_SCHEME);
  G4P.setCursor(ARROW);
  surface.setTitle("Sketch Window");
  textarea1 = new GTextArea(this, 237, 11, 198, 199, G4P.SCROLLBARS_BOTH | G4P.SCROLLBARS_AUTOHIDE);
  textarea1.setText("Messages from Zumo");
  textarea1.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  textarea1.setOpaque(true);
  textarea1.addEventHandler(this, "textarea1_change1");
  button1 = new GButton(this, 19, 275, 91, 60);
  button1.setTextAlign(GAlign.LEFT, GAlign.TOP);
  button1.setText("Send message to Zumo");
  button1.setTextBold();
  button1.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  button1.addEventHandler(this, "button1_click1");
  textfield1 = new GTextField(this, 110, 300, 160, 30, G4P.SCROLLBARS_NONE);
  textfield1.setText("Messages To Zumo");
  textfield1.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  textfield1.setOpaque(true);
  textfield1.addEventHandler(this, "textfield1_change1");
  button2 = new GButton(this, 11, 169, 80, 30);
  button2.setText("Left");
  button2.setTextBold();
  button2.addEventHandler(this, "button2_click1");
  button3 = new GButton(this, 127, 171, 80, 31);
  button3.setText("Right");
  button3.setTextBold();
  button3.addEventHandler(this, "button3_click1");
  button4 = new GButton(this, 71, 121, 80, 30);
  button4.setText("Forward");
  button4.setTextBold();
  button4.addEventHandler(this, "button4_click1");
  button5 = new GButton(this, 69, 225, 80, 30);
  button5.setText("Back");
  button5.setTextBold();
  button5.addEventHandler(this, "button5_click1");
  button6 = new GButton(this, 10, 10, 80, 30);
  button6.setText("Stop");
  button6.setTextBold();
  button6.addEventHandler(this, "button6_click1");
  button7 = new GButton(this, 20, 20, 80, 30);
  button7.setText("90");
  button7.setTextBold();
  button7.addEventHandler(this, "button7_click1");
}

// Variable declarations 
// autogenerated do not edit
GTextArea textarea1; 
GButton button1; 
GTextField textfield1; 
GButton button2; 
GButton button3; 
GButton button4; 
GButton button5; 
GButton button6;
GButton button7;
