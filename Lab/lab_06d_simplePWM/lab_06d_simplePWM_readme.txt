lab_06d_simplePWM

This lab is similar to the previous lab exercise - where we drove the LED directly
from the timer's output.

The major change here is setting up the timer using the Timer_A_outputPWM()
function. This provides a simple way to generate a single PWM signal. 

By the way, if you need more flexibility - such as more channels of PWM from a 
single Timer_A - you can setup the timer (and each CCR) individually. There are
examples of "multiple PWM" in the examples that ship with DriverLib.

We added two arguments to the initTimers() function. These represent the 'Period'
and 'Duty Cycle' of the PWM waveform. We also called the initTimers() function
every second in the main while{} loop. These changes were made so that in CCS, 
you could Suspend the debugger, change the values of the two arguments, and 
in less than a second, the new values would take effect. This is a simple way to
see how differences in these values affect DriverLib's simple PWM function. 

