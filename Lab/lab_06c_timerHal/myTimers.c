// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06c_timerHal project) 
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myTimers.h"
#include "hal.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
Timer_A_UpModeConfig gCfgUpMode = {
    TIMER_A_CLOCKSOURCE_ACLK,                                                  // Use ACLK (slower clock)
    TIMER_A_CLOCKSOURCE_DIVIDER_1,                                             // Input clock = ACLK / 1 = 32KHz
    0xFFFF / 2,                                                                // Period (0xFFFF/2):  8000 / 32Khz = 1 second
    TIMER_A_TAIE_INTERRUPT_ENABLE,                                             // Enable TAR -> 0 interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,                                       // No CCR0 interrupt needed
    TIMER_A_DO_CLEAR                                                           // Clear TAR & clock divider
};

Timer_A_CompareModeConfig gInitCcr2Param = {
    TIMER_A_CAPTURECOMPARE_REGISTER_2,                                         // Use CCR2 for compare
    TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,                                  // Since directly driving LED, interrup not req'd
    TIMER_A_OUTPUTMODE_TOGGLE_RESET,                                           // Toggle provides a 1 sec period based on CCR0 and CCR2 values
    0x4000                                                                     // Compare value: 4000 = 1/2 second
};

//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    //*************************************************************************
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA in Up mode using ACLK
    //    Toggle LED1 (Red) on/off every second directly from the timers CCR2 output pin
    //    Toggle LED2 (Green) on/off every second using TAxIFG
    //*************************************************************************
    MAP_Timer_A_configureUpMode( TIMER_BASE, &gCfgUpMode );                        // Set up Timer A0

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
    MAP_Timer_A_initCompare( TIMER_BASE, &gInitCcr2Param );

    //*************************************************************************
    // 3. Clear/enable flags and start timer
    //*************************************************************************
	// Clear and enable interrupt flags inside TimerA peripheral
    MAP_Timer_A_clearInterruptFlag( TIMER_BASE );                                  // Clear TAxIFG interrupt pending flag bit

    // Clear and enable individual IRQ interrupts                              // Set appropriate bits in NVIC->IPR and NVIC->IER)
	NVIC_ClearPendingIRQ ( NVIC_TIMER_BIT );                                   // (CMSIS) Clear pending bit for Timer_A0 all other IRQs
    NVIC_EnableIRQ( NVIC_TIMER_BIT );                                          // (CMSIS) Enable NVIC for individual Timer_A0 all other IRQs

    // Start the timer; make sure you specify the correct counting mode
    MAP_Timer_A_startCounter(
        TIMER_BASE,
        TIMER_A_UP_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routine
//*****************************************************************************
void timer_ISR (void)
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    switch( TIMER_IV ) {
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
          MAP_GPIO_toggleOutputOnPin( LED2_PORT, LED2_PIN );
          break;
     default: _never_executed();
    }
}

