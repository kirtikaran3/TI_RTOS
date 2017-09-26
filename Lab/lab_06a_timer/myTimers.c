// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06a_timer project) (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
Timer_A_ContinuousModeConfig gInitContParam = {
        TIMER_A_CLOCKSOURCE_ACLK,                                              // Use ACLK (slower clock)
        TIMER_A_CLOCKSOURCE_DIVIDER_1,                                         // Input clock = ACLK / 1 = 32KHz
        TIMER_A_TAIE_INTERRUPT_ENABLE,                                         // Enable TAR -> 0 interrupt
        TIMER_A_DO_CLEAR                                                       // Clear TAR & clock divider
};

//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers(void)
{
    //*************************************************************************
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA0 in Continuous mode using ACLK
    //    Toggle LED2 (Green) on/off every 2 seconds using timer interrupt (TA0IFG)
    //    Note: We show two ways to create the configuration parameter (global or local); you can experiment by choosing either one for MAP_Timer_A_configureContinuousMode()
    //*************************************************************************
    Timer_A_ContinuousModeConfig InitContParam = { 0 };
        InitContParam.clockSource =                 ________________________;       // Use ACLK (slower clock)
        InitContParam.clockSourceDivider =          _____________________________;  // Input clock = ACLK / 1 = 32KHz
        InitContParam.timerInterruptEnable_TAIE =   _____________________________;  // Enable TAR -> 0 interrupt
        InitContParam.timerClear =                  TIMER_A_DO_CLEAR;               // Clear TAR & clock divider
    MAP_Timer_A_configureContinuousMode( TIMER_A0_BASE, &InitContParam );


    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
    // This example does not use these features


    //*************************************************************************
    // 3. Clear/enable interrupt flags and start timer
    //*************************************************************************
	// Clear and enable interrupt flags inside TimerA peripheral
    MAP_Timer_A___________________( TIMER_A0_BASE );                               // Clear TA0IFG interrupt pending flag bit
    //MAP_Timer_A_enableInterrupt( TIMER_A0_BASE );                                // Enable TA0IFG interrupt on counter (TAR) rollover - already set by MAP_Timer_A_configureContinuousMode()

    // Clear and enable individual IRQ interrupts                                  // Set appropriate bits in NVIC->IPR and NVIC->IER)
	NVIC_ClearPendingIRQ ( TA0_N_IRQn );                                           // (CMSIS) Clear pending bit for Timer_A0 all other IRQs
    NVIC_EnableIRQ( TA0_N_IRQn );                                                  // (CMSIS) Enable NVIC for individual Timer_A0 all other IRQs

    // Start the timer; make sure you specify the correct counting mode
    MAP_Timer_A_startCounter(
        TIMER_A0_BASE,
        TIMER_A_ _______________
    );
}


//*****************************************************************************
// Interrupt Service Routine
//*****************************************************************************
void myTimer0_ISR (void)
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
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

