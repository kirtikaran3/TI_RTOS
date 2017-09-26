// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06d_simplePWM project)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myTimers.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
    Timer_A_PWMConfig param = {
        TIMER_A_CLOCKSOURCE_ACLK,                                               // Use ACLK (slower clock)
        TIMER_A_CLOCKSOURCE_DIVIDER_1,                                          // Input clock = ACLK / 1 = 32KHz
        0,                                                                      // Initialized in runtime code
        TIMER_A_CAPTURECOMPARE_REGISTER_2,                                      // Use CCR2 for compare
        TIMER_A_OUTPUTMODE_RESET_SET,                                           // Toggling provides a good waveform to watch with LED
        0                                                                       // Initialized in runtime code
    };

//*****************************************************************************
// Initialize Timer
//*****************************************************************************
void initTimers( uint16_t myPeriod, uint16_t myDutyCycle )
{
    //*************************************************************************
    // 1. Setup Timer (TAR, TACTL)
    //    TimerA1, CCR0, and CCR2 are automatically setup by the Timer_A_outputPWM
    //    function. Depending upon the period/duty-cycle choices, the light will
    //    blink, or be "always on" (to the naked eye) but either bright or dim.
    //    (For dim, try: Period=40, Duty Cycle=2.)
    //*************************************************************************
      param.timerPeriod = myPeriod;                                             // Period
      param.dutyCycle = myDutyCycle;                                            // Duty cycle (20/40 = 1/2 duty cycle)
      MAP_Timer_A_generatePWM( TIMER_A0_BASE, &param );

}

