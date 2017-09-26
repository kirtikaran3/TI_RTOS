lab_07b_lpm_timer

This lab shows how you can improve power usage by replacing __delay_cycles()
function calls with a timer - as suggested by ULP Advisor.

It also makes use of the low power modes (LPM3) to let the procesor sleep
while waiting for the timer's interrupt.

General procedure:
- This labs picks up from Lab 7a by looking at the Lab 4c solution
- Turn on ULP Advisor and examine some of its advice
- Set file-specific options to avoid advice on DriverLib
- Import Lab 6b which does basically the same thing as Lab 4c but 
  uses timers instead of __delay_cycles()
- Replace while{} loop with LPM3

File source code in this readme file:
- main.c
- myTimers.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_07b_lpm_timer project) (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"
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
    MAP_Interrupt_enableSleepOnIsrExit();
    __enable_irq();                                                             // TI Compiler and CMSIS intrinsic

    __sleep();

    while(1) {
        __no_operation();                                                       // Placeholder for while loop (not required)
    }
}


// ----------------------------------------------------------------------------
// myTimers.c  (for lab_07b_lpm_timer project) (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
Timer_A_UpModeConfig gCfgUpMode = {
    TIMER_A_CLOCKSOURCE_ACLK,                                                  // Use ACLK (slower clock)
    TIMER_A_CLOCKSOURCE_DIVIDER_1,                                             // Input clock = ACLK / 1 = 32KHz
    0xFFFF / 2,                                                                // Period (0xFFFF/2):  8000 / 32Khz = 1 second
    TIMER_A_TAIE_INTERRUPT_ENABLE,                                             // Enable TAR -> 0 interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,                                        // Enable CCR0 compare interrupt
    TIMER_A_DO_CLEAR                                                           // Clear TAR & clock divider
};


//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    //*************************************************************************
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA0 in Up mode using ACLK
    //    Toggle LED1 (Red) on/off every second using CCR0IFG
    //    Toggle LED2 (Green) on/off every second using TA0IFG
    //    Note: gCfgUpMode was created as a global variable. This could have been done as a local variable, too.
    //*************************************************************************
    MAP_Timer_A_configureUpMode( TIMER_A0_BASE, &gCfgUpMode );                     // Set up Timer A0

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
       // This example does not use these features
       // CCR0 is setup by the configureUpMode function

    //*************************************************************************
    // 3. Clear/enable flags and start timer
    //*************************************************************************
	// Clear and enable interrupt flags inside TimerA peripheral
    MAP_Timer_A_clearInterruptFlag( TIMER_A0_BASE );                               // Clear TA0IFG interrupt pending flag bit
    MAP_Timer_A_clearCaptureCompareInterrupt( TIMER_A0_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0                                      // Clear CCR0IFG interrupt pending flag bit
    );

    // These two enables are already done by the configureUpMode function
    //MAP_Timer_A_enableInterrupt( TIMER_A0_BASE );                                // Enable TA0IFG interrupt on counter (TAR) rollover - already set by MAP_Timer_A_configureContinuousMode()
    //MAP_Timer_A_enableCaptureCompareInterrupt( TIMER_A0_BASE,
    //    TIMER_A_CAPTURECOMPARE_REGISTER_0                                    // Enable CCR0IFG - already set by MAP_Timer_A_configureUpMode()
    //);

    // Clear and enable individual IRQ interrupts                              // Set appropriate bits in NVIC->IPR and NVIC->IER)
    NVIC_ClearPendingIRQ ( TA0_0_IRQn );                                       // (CMSIS) Clear pending bit for Timer_A0 CCR0 IRQ
	NVIC_ClearPendingIRQ ( TA0_N_IRQn );                                       // (CMSIS) Clear pending bit for Timer_A0 all other IRQs
    NVIC_EnableIRQ( TA0_0_IRQn );                                              // (CMSIS) Enable NVIC for individual Timer_A0 CCR0 IRQ
    NVIC_EnableIRQ( TA0_N_IRQn );                                              // (CMSIS) Enable NVIC for individual Timer_A0 all other IRQs

    // Start the timer; make sure you specify the correct counting mode
    MAP_Timer_A_startCounter(
        TIMER_A0_BASE,
        TIMER_A_UP_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
void myTimer0_CCR0_ISR (void)
{
    // 4. Timer ISR and vector

    // Clear CCR0IFG
    MAP_Timer_A_clearCaptureCompareInterrupt( TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0 );

    // Toggle LED1 (Red) on/off
    //MAP_GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
}

void myTimer0_ISR (void)
{
    // 4. Timer ISR and vector

    switch( TA0IV ) {
     case 0x00: break;                 // (0x00) None
     case 0x02:                        // (0x02) CCR1 IFG
          __no_operation();
          break;
     case 0x04:                        // (0x04) CCR2 IFG
          __no_operation();
          break;
     case 0x06:                        // (0x06) CCR3 IFG
          __no_operation();
          break;
     case 0x08:                        // (0x08) CCR4 IFG
          __no_operation();
          break;
     case 0x0A: break;                 // (0x0A) Reserved
     case 0x0C: break;                 // (0x0C) Reserved
     case 0x0E:                        // (0x0E) TA0IFG - TAR overflow
          // Toggle LED2 (Green) LED on/off
          MAP_GPIO_toggleOutputOnPin( GPIO_PORT_P2, GPIO_PIN1 );
          break;
     default: _never_executed();
    }
}


