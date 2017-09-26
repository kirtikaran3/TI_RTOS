// ----------------------------------------------------------------------------
// main.c  (for lab_04a_clock project) (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"
#include "myClocks.h"


//***** Prototypes ************************************************************


//***** Defines ***************************************************************
#define ONE_SECOND   myMCLK_FREQUENCY_IN_HZ
#define HALF_SECOND  myMCLK_FREQUENCY_IN_HZ / 2


//***** Global Variables ******************************************************


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Initialize GPIO
    initGPIO();

    // Initialize clocks
    initClocks();

    while(1) {
        // Turn on LED
        MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait about a second
        __delay_cycles( HALF_SECOND );

        // Turn off LED
        MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait another second
        __delay_cycles( HALF_SECOND );
    }
}
