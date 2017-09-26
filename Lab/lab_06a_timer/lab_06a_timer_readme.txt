lab_06a_timer

Similar to lab_05b_wdtBlink, we want to blink an LED. In this case, 
though, we'll use TIMER_A to generate an interrupt rather than using the 
watchdog interval timer. During the interrupt routine we'll toggle the 
GPIO pin that drives an LED on our Launchpad board.

In lab_06c_timerDirectDriveLed, we will drive the LED directly from the output 
of the Timer. Due to the pinout differences between these devices, we had
to select different timers to use across our various LaunchPads:
         'F5529 Launchpad uses TA0
         'FR4133 Launchpad uses TA1
         'FR5969 Launchpad uses TA1
         'FR6989 Launchpad uses TA1
	 'P401 (MSP432) LaunchPad uses uses TA0

If you compare the two solutions, you should see that the timer setup code
only differs in the function argument that indicates which timer you're using.
In other words, it's very minimal.

As we write the ISR code, you should see that TIMER_A has two interupts: 
- One is dedicated to CCR0 (Capture and Compare Register 0).
- The second handles all the other timer interrupts

This first TIMER_A lab will use the main timer/counter rollover interrupt 
(called TA0IFG or TA1IFG). As with GPIO interrupts, our timer ISR should read 
the Timer IV register (TA0IV or TA1IV) and decipher the correct response using 
a switch/case statement.

The goal is to create a timer interrupt every 2 seconds. If we toggle the LED
inside each ISR, it should take 4 seconds to blink on and off.


Basic Steps:
- Import project from previous lab solution
- Delete old, unneccessary code
- Create the Timer Setup code -- using Continuous mode
- Write the Timer ISR
- Run and observe the LED flashing


File source code in this readme file:
- main.c
- myGpio.c
- myTimers.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_06a_timer project) (MSP432 Launchpad)
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
// myGpio.c  (MSP432 Launchpad)
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
        InitContParam.clockSource =                 TIMER_A_CLOCKSOURCE_ACLK;       // Use ACLK (slower clock)
        InitContParam.clockSourceDivider =          TIMER_A_CLOCKSOURCE_DIVIDER_1;  // Input clock = ACLK / 1 = 32KHz
        InitContParam.timerInterruptEnable_TAIE =   TIMER_A_TAIE_INTERRUPT_ENABLE;  // Enable TAR -> 0 interrupt
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
    MAP_Timer_A_clearInterruptFlag( TIMER_A0_BASE );                               // Clear TA0IFG interrupt pending flag bit
    //MAP_Timer_A_enableInterrupt( TIMER_A0_BASE );                                // Enable TA0IFG interrupt on counter (TAR) rollover - already set by MAP_Timer_A_configureContinuousMode()

    // Clear and enable individual IRQ interrupts                                  // Set appropriate bits in NVIC->IPR and NVIC->IER)
	NVIC_ClearPendingIRQ ( TA0_N_IRQn );                                           // (CMSIS) Clear pending bit for Timer_A0 all other IRQs
    NVIC_EnableIRQ( TA0_N_IRQn );                                                  // (CMSIS) Enable NVIC for individual Timer_A0 all other IRQs

    // Start the timer; make sure you specify the correct counting mode
    MAP_Timer_A_startCounter(
        TIMER_A0_BASE,
        TIMER_A_CONTINUOUS_MODE
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


