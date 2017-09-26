lab_03a_gpio

This lab creates what is often called "The Embedded Hello World" program.

While this is just simple LED blinking code, we implement with the MSP430ware 
DriverLib library. This gives us a good example for learning to use, as well as 
link in, a library. This library will become even more important as we explore 
other peripherals in later lab exercises.

Part of learning to use a library involves creating 'Portable' CCS projects. Our 
projects need to be created such that others can easily get them up-and-running 
on their own systems. (Or, we can easily migrate to a later revision of a 
library.) To this end, we will learn how to create IDE path variables - and use 
them to point to our library's files.

Finally, along with single-stepping our program, we will explore the "Registers" 
window in CCS. This lets us view the CPU registers, watching how they change 
as we step thru our code.

One last note: The FRAM devices require an extra call to unlock the
GPIO pins. This call isn't needed for the 'F5529 (F5xx series) device.

WARNING

Our code example is a BAD way to implement a blinking light ... from an 
efficiency standpoint. The _delay_cycles() function is VERY INEFFICIENT. A 
timer, which we learn about in a later chapter, would be a much lower-power way 
to implement a delay. For our purposes in this chapter, though, this is an easy 
function to get started with.

File source code in this readme file:
- main.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_03a_gpio project) (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file


//***** Prototypes ************************************************************


//***** Defines ***************************************************************
#define ONE_SECOND  600000                                                      // Well, it's about a second
#define HALF_SECOND ONE_SECOND/2


//***** Global Variables ******************************************************


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Set pin P1.0 to output direction and turn LED off
    MAP_GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );                         // Red LED (LED1)
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    while(1) {
        // Turn on LED
        MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait about a second
        __delay_cycles( ONE_SECOND );

        // Turn off LED
        MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait another second
        __delay_cycles( ONE_SECOND );
    }
}

