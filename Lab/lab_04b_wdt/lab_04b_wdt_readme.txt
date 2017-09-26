lab_04b_wdt

This lab takes lab_02a_ccs (which does a simple printf) and experiments with the
watchdog timer. In a way, we're playing "Can you beat the clock?"  ;-)

General Procedure
- Comment out WDT hold function in our lab4a code to see that the watchdog
  prevents us from ever getting to our blinking LED
- Import lab_02a_ccs solution and rename to lab_04b_wdt
- Modify printf() and put it into a while loop to count/print the # of times we 
  run thru the while{} loop
- Add the driverlib library
- Run the code and view how many times the while loop runs when invoking the
  watchdog timer with different clock rates (playing with the clock divider value)
- Finally, we import a program from the driverlib examples to see a way to 
  visualize the watchdog timer resetting the CPU

DANGER, DANGER, DANGER ...

As we said before, in a 'real-world' MSP program, you would NOT want to call 
printf(). This function is slow, requires a great deal of program and data 
memory, and uses too much power – all bad things for any embedded application.

File source code in this readme file:
- hello.c

Final code ... you can copy from this if you want to save typing time & effort.


// --------------------------------------------------------------------
// hello.c  (for lab_04b_wdt project) (MSP432)
// --------------------------------------------------------------------
#include <stdio.h>
#include <driverlib.h>

uint16_t count = 0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	                                        // Stop watchdog timer

    // Initialize the WDT as a watchdog
	MAP_WDT_A_initWatchdogTimer(
                             WDT_A_CLOCKSOURCE_ACLK,                    // Which clock should WDT use?
                             //WDT_A_CLOCKITERATIONS_64                 // WDT clock input divisor
							 WDT_A_CLOCKITERATIONS_512                  // -> Here are 4 (of 8) divisor choices
	                         //WDT_A_CLOCKITERATIONS_8192
                             //WDT_A_CLOCKITERATIONS_32K
	);

    // Start the watchdog
    WDT_A_start( WDT_A_BASE );

    while (1) {
    	//MAP_WDT_A_clearTimer();

        count++;
        printf( "I called this %d times\n", count );
    }
}


