In this lab, you create a new CCS project that contains one source file – 
hello.c – which prints “Hello World” to the CCS console window.

The purpose of this lab is to practice creating projects and getting to know the 
look and feel of CCS. If you already have experience with CCS (or the 
Eclipse) IDE, this lab will be a quick review. The workshop labs start out very 
basic, but over time, they’ll get a bit more challenging and will contain less 
“hand holding” instructions.

DANGER, DANGER, DANGER ...

In a 'real-world' MSP430 program, you would NOT want to call printf(). This 
function is slow, requires a great deal of program and data memory, and sucks 
power – all bad things for any embedded application.
 
We’re using this function since it’s the common starting point when working with 
a new processor. The next lab has us programming what is commonly called, the 
“embedded” version of “hello world”. This involves blinking an LED on the target 
board.

As a final note, real-world programs tend to replace printf() by sending data to 
a terminal via the serial port. 

