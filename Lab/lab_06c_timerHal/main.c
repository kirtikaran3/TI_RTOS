// ----------------------------------------------------------------------------
// main.c  (for lab_06c_timerHal project)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file
#include "hal.h"
#include "myClocks.h"
#include "myTimers.h"


//***** Prototypes ************************************************************


//***** Defines ***************************************************************


//***** Global Variables ******************************************************


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
    initTimers();

    // Enable interrupts globally (set PRIMASK = 0)
    __enable_irq();                                                             // TI Compiler and CMSIS intrinsic

    while(1) {
        __no_operation();                                                       // Placeholder for while loop (not required)
    }
}
