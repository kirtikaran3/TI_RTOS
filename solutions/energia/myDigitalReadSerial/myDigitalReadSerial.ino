/*
  myDigitalReadSerial with on-board Pushbutton
  Reads a digital input on pushbutton 2, then
  prints the result to the serial monitor 
  
  This example code is in the public domain.
 */

// PUSH2 pin has a pushbutton attached to it. Give it a name:
int pushButton = PUSH2;  // For MSP432 LaunchPad, this is pin 74


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600); 
  // make the on-board pushbutton's pin an input pullup:
  pinMode(pushButton, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  // print out the state of the button:
  Serial.println(buttonState);
  delay(5);        // delay in between reads for stability
}



