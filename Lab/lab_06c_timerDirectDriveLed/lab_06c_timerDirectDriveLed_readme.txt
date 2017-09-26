lab_06c_timerDirectDriveLed

This lab is a minor adaptation of the code from the previous exercise. The main
difference is that you will connect the GPIO pin to the output of the Timer_A CCR2 
pin (TA1.2).

Unfortunately, the Launchpad does not have an LED connected directly to a
timer output pin, therefore we'll need to use a jumper in order to make the 
proper connection - here's an excerpt from the lab solution:

        // When running this lab exercise, you will need to pull the JP8 jumper and
        // use a jumper wire to connect signal from P1.3 (on boosterpack pinouts) to
        // JP6.2 (bottom pin) of LED1 jumper ... this lets the TA1.2 signal drive 
        // LED1 directly (without having to use interrupts)

The rate that we're toggling the LED was a convenient, if arbitrary, choice.
What rate will we toggle the LED with the timer in Lab6c? We answer that by 
comparing it with the LED toggle rates for our preceeding two labs:

  - Lab6a toggled the LED at a 4 second rate (an interrupt every 2 seconds)
  - Lab6b toggled the LED at a 2 second rate (an interrupt every second)
  - Lab6c will toggle the LED every second (no interrupt needed)


Basic Steps:
- Copy project from lab_06b_upTimer (your solution or the workshop solution)
- Delete (or comment out) GPIO pin setup for P1.0
- Add GPIO setup to connect P1.3 to peripheral module function (i.e. timer)
- In timer setup, add code to configure CCR2 for compare mode
- Turn off CCR0 interrupt (as this isn't needed this for this exercise)
- Set a breakpoint - if you hit it, then something isn't correct
- Run and observe both LED's are (still) flashing


File source code in this readme file:
- main.c
- myGpio.c
- myTimers.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_06c_timerDirectDriveLed project)  (MSP432 Launchpad)
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
    __enable_irq();                                                             // TI Compiler and CMSIS intrinsic

    while(1) {
        __no_operation();                                                       // Placeholder for while loop (not required)
    }
}


// ----------------------------------------------------------------------------
// myGpio.c  (for lab_06c_timerDirectDriveLed project)  (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************


//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void initGPIO(void) {

    // Set pin P1.0 to output direction and turn LED off                        // Red LED (LED1)
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
    MAP_GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Set pin P2.0/2.1/2.1 to output direction and turn LED off                // RGB LED (LED2)
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 );
    MAP_GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 );                   

//    // Set P1.1 as input with pull-up resistor (for push button S1)
//    //  First, configure the interrupt to trigger on low-to-high transition
//    //  and then clear flag and enable the interrupt
//    MAP_GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN1 );
//    MAP_GPIO_interruptEdgeSelect ( GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION );
//    MAP_GPIO_clearInterruptFlag ( GPIO_PORT_P1, GPIO_PIN1 );
//    MAP_GPIO_enableInterrupt ( GPIO_PORT_P1, GPIO_PIN1 );

//    // Set P1.4 as input with pull-up resistor (for push button S2)
//    //  First, configure the interrupt to trigger on low-to-high transition
//    //  and then clear flag and enable the interrupt
//    MAP_GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN4 );
//    MAP_GPIO_interruptEdgeSelect ( GPIO_PORT_P1, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION );
//    MAP_GPIO_clearInterruptFlag ( GPIO_PORT_P1, GPIO_PIN4 );
//    MAP_GPIO_enableInterrupt ( GPIO_PORT_P1, GPIO_PIN4 );

    // When running this lab exercise, you will need to pull the J6 jumper and
    // use a jumper wire to connect signal from P2.5 (on boosterpack pinouts) to
    // J2.19 (upper right-corner pin) of LED1 jumper ... this lets the TA0.2 signal drive
    // LED1 directly (without having to use interrupts)
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin( GPIO_PORT_P2, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION );

    // Set LFXT (low freq crystal pins) and HFXT (high freq crystal pins) as 
    // clock crystal inputs (rather than GPIO).
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_PJ,
        GPIO_PIN0 +                                                             // LFXIN  on PJ.0
        GPIO_PIN1 +                                                             // LFXOUT on PJ.1
        GPIO_PIN3 +                                                             // HFXOUT on PJ.2
        GPIO_PIN2 ,                                                             // HFXTIN on PJ.3
        GPIO_PRIMARY_MODULE_FUNCTION
    );

//  Output the ACLK, MCLK and HSMCLK signals to their respective pins to allow watching them with logic analyzer
//    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(
//                    GPIO_PORT_P4,
//                    GPIO_PIN2 +                                               // ACLK on P4.2
//                    GPIO_PIN3 +                                               // MCLK on P4.3
//                    GPIO_PIN4 ,                                               // HSMCLK on P4.4
//                    GPIO_PRIMARY_MODULE_FUNCTION
//    );
}


//*****************************************************************************
// Interrupt Service Routine
//*****************************************************************************
//void Port1_ISR (void)
//{
//    switch( P1IV ) {
//        case P1IV__NONE:   break;                                // None
//        case P1IV__P1IFG0:                                       // Pin 0
//             __no_operation();
//             break;
//        case P1IV__P1IFG1:                                       // Pin 1 (button 1)
//             MAP_GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
//             break;
//        case P1IV__P1IFG2:                                       // Pin 2
//             __no_operation();
//             break;
//        case P1IV__P1IFG3:                                       // Pin 3
//             __no_operation();
//             break;
//        case P1IV__P1IFG4:                                       // Pin 4 (button 2)
//             MAP_GPIO_toggleOutputOnPin( GPIO_PORT_P2, GPIO_PIN1 );
//             break;
//        case P1IV__P1IFG5:                                       // Pin 5
//             __no_operation();
//             break;
//        case P1IV__P1IFG6:                                       // Pin 6
//             __no_operation();
//             break;
//        case P1IV__P1IFG7:                                       // Pin 7
//             __no_operation();
//             break;
//        default:   break;
//    }
//}


// ----------------------------------------------------------------------------
// myTimers.c  (for lab_06c_timerDirectDriveLed project) (MSP432 Launchpad)
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
    //    TimerA0 in Up mode using ACLK
    //    Toggle LED1 (Red) on/off every second directly from the timers CCR2 output pin
    //    Toggle LED2 (Green) on/off every second using TA0IFG
    //    Note: gCfgUpMode was created as a global variable. This could have been done as a local variable, too.
    //*************************************************************************
    MAP_Timer_A_configureUpMode( TIMER_A0_BASE, &gCfgUpMode );                     // Set up Timer A0

    //*************************************************************************
    // 2. Setup Capture & Compare features
    //*************************************************************************
    MAP_Timer_A_initCompare( TIMER_A0_BASE, &gInitCcr2Param );

    //*************************************************************************
    // 3. Clear/enable flags and start timer
    //*************************************************************************
	// Clear and enable interrupt flags inside TimerA peripheral
    MAP_Timer_A_clearInterruptFlag( TIMER_A0_BASE );                               // Clear TA0IFG interrupt pending flag bit
    //MAP_Timer_A_enableInterrupt( TIMER_A0_BASE );                                // Enable TA0IFG interrupt on counter (TAR) rollover - already set by MAP_Timer_A_configureContinuousMode()

    // Clear and enable individual IRQ interrupts                              // Set appropriate bits in NVIC->IPR and NVIC->IER)
	NVIC_ClearPendingIRQ ( TA0_N_IRQn );                                       // (CMSIS) Clear pending bit for Timer_A0 all other IRQs
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
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    // Clear CCR0IFG
    //MAP_Timer_A_clearCaptureCompareInterrupt( TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0 );

    // Toggle LED1 (Red) on/off
    //MAP_GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
}

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


