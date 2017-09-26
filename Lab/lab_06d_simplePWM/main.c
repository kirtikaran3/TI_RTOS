// ----------------------------------------------------------------------------
// main.c  (for lab_06d_simplePWM project)  (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"
#include "myClocks.h"
#include "myTimers.h"


//***** Prototypes ************************************************************


//***** Defines ***************************************************************
#define ONE_SECOND  myMCLK_FREQUENCY_IN_HZ


//***** Global Variables ******************************************************
uint16_t period    = 4000;
uint16_t dutyCycle = 2000;


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Disable interrupts (enabled by default on ARM Cortex-M processors) (set PRIMASK = 1)
    __disable_irq();                                                            // TI Compiler and CMSIS intrinsic

    // Initialize GPIO
    initGPIO();

    // Initialize clocks
    initClocks();

    // Initialize timers
    initTimers( period, dutyCycle );

    // Enable interrupts globally (set PRIMASK = 0)
    __enable_irq();                                                             // TI Compiler and CMSIS intrinsic

    while(1) {
        __delay_cycles( ONE_SECOND );

        initTimers( period, dutyCycle );
    }
}
