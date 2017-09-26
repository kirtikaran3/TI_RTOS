/*
  Interrupt_PushButton
*/

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(RED_LED, OUTPUT);
  
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW); 

  pinMode(PUSH2, INPUT_PULLUP);
  attachInterrupt(PUSH2, myISR, FALLING);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(RED_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(RED_LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}

void myISR()
{
  digitalWrite(GREEN_LED, HIGH);
}

