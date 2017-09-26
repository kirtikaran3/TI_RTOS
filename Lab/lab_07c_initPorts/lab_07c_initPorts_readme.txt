lab_07c_initPorts

This lab explores the effect that GPIO initialization has on system power 
consumption. We should find that initializing all of the pins will result
in lower energy being consumed.

General procedure:
- Create lab_07c_initPorts (by renaming previously imported lab_04c_crystals_solution)
- Replace while{} loop with __low_power_mode_3() intrinsic function call
- Capture energy baseline (how much energy does program use before initializing ports)
- Add initPortsAsOutputs.c file and call its initPorts() function from main.
  Note: this MUST BE DONE before the call to our initGPIO() function
- Capture energy information and compare to previous result
- Add a second file, initPorts.c, to the project. (This sets all GPIO as inputs.)
- Set initPortsAsOutputs.c as "Exclude From Build"
- Capture energy information again, and compare to previous results

File source code in this readme file:
- main.c
- initPorts.c
- initPortsAsOutputs.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_07c_initPorts project) (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"
#include "myClocks.h"
#include "initPorts.h"


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

    // Disable interrupts (enabled by default on ARM Cortex-M processors) (set PRIMASK = 1)
    __disable_irq();                                                            // TI Compiler and CMSIS intrinsic

    // Initialize GPIO Ports
    initPorts();
    initGPIO();

    // Initialize clocks
    initClocks();

    __no_operation();

    while (1)
    {
        // Enter Low Power Mode 3 (LPM3) (deep sleep)
        MAP_PCM_gotoLPM3();
    }

//    while(1) {
//        // Turn on LED
//        MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );
//
//        // Wait about a second
//        __delay_cycles( ONE_SECOND );
//
//        // Turn off LED
//        MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
//
//        // Wait another second
//        __delay_cycles( ONE_SECOND );
//    }
}


// ----------------------------------------------------------------------------
// initPorts.c
// 
// This function drives all the I/O's as inputs tied low using the MSP's
// internal pulldown resistor. This avoids floating inputs (which causes extra 
// power to be consumed).  This file is compatible with most MSP430/MSP432 devices,
// including the F5529, FR5969, FR4133 and MSP432 Launchpads.
//
// It may not be compatible with custom hardware, if you have components
// attached to the I/Os that could be affected by these settings. So if
// you are using other boards, please verify/modify function before using it.
//
// ----------------------------------------------------------------------------
#include "driverlib.h"


#define GPIO_ALL    GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3| \
                    GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7

// Current MSP432 header file does not yet contain the 'has port' definitions
#ifdef __MSP432P401R__
  #define __MSP430_HAS_PORT1_R__ 1
  #define __MSP430_HAS_PORT2_R__ 1
  #define __MSP430_HAS_PORT3_R__ 1
  #define __MSP430_HAS_PORT4_R__ 1
  #define __MSP430_HAS_PORT5_R__ 1
  #define __MSP430_HAS_PORT6_R__ 1
  #define __MSP430_HAS_PORT7_R__ 1
  #define __MSP430_HAS_PORT8_R__ 1
  #define __MSP430_HAS_PORT9_R__ 1
#endif

// * This function drives all the I/O's as inputs, to avoid floating inputs
// * (which cause extra power to be consumed).  This setting is compatible with
// * TI FET target boards, as well as the F5529, FR4133, FR5969 and MSP432 Launchpads.
void initPorts(void)
{
//**** Initialize GPIO Ports **************************************************
// By initializing all the ports as inputs using the pull-down resistor 

#ifdef __MSP430_HAS_PORT1_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P1, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT2_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT3_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT4_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT5_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT6_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P6, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT7_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P7, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT8_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P8, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT9_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P9, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORTJ_R__
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_PJ, GPIO_ALL);
#endif

#ifdef __MSP430FR4133
    // Unlock pins (required for FRAM devices)
    PMM_unlockLPM5();
#endif

#ifdef __MSP430FR5969
    // Unlock pins (required for FRAM devices)
    PMM_unlockLPM5();
#endif

#ifdef __MSP430FR6989
    // Unlock pins (required for FRAM devices)
    PMM_unlockLPM5();
#endif
}



// ----------------------------------------------------------------------------
// initPortsAsOutputs.c
// 
// This function drives all the I/O's as output-low, to avoid floating inputs
// (which causes extra power to be consumed).  This file is compatible with
// most MSP devices, including the F5529, FR5969, FR4133 and MSP432 Launchpads.
// It may not be compatible with custom hardware, if you have components
// attached to the I/Os that could be affected by these settings. So if
// you are using other boards, please verify/modify function before using it.
//
// ----------------------------------------------------------------------------
#include "driverlib.h"


#define GPIO_ALL    GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3| \
                    GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7

// Current MSP432 header file does not yet contain the 'has port' definitions
#ifdef __MSP432P401R__
  #define __MSP430_HAS_PORT1_R__ 1
  #define __MSP430_HAS_PORT2_R__ 1
  #define __MSP430_HAS_PORT3_R__ 1
  #define __MSP430_HAS_PORT4_R__ 1
  #define __MSP430_HAS_PORT5_R__ 1
  #define __MSP430_HAS_PORT6_R__ 1
  #define __MSP430_HAS_PORT7_R__ 1
  #define __MSP430_HAS_PORT8_R__ 1
  #define __MSP430_HAS_PORT9_R__ 1
#endif

// * This function drives all the I/O's as output-low, to avoid floating inputs
// * (which cause extra power to be consumed).  This setting is compatible with
// * TI FET target boards, as well as the F5529, FR4133, FR5969 and MSP432 Launchpads.
void initPorts(void)
{
//**** Initialize GPIO Ports **************************************************
// By initializing all the ports low, the LED's are automatically setup and 
//   and turned off by default

#ifdef __MSP430_HAS_PORT1_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT2_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT3_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT4_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT5_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT6_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT7_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT8_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORT9_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_P9, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_ALL);
#endif

#ifdef __MSP430_HAS_PORTJ_R__
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_ALL);
    GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_ALL);
#endif

#ifdef __MSP430FR4133
    // Unlock pins (required for FRAM devices)
    PMM_unlockLPM5();
#endif

#ifdef __MSP430FR5969
    // Unlock pins (required for FRAM devices)
    PMM_unlockLPM5();
#endif

#ifdef __MSP430FR6989
    // Unlock pins (required for FRAM devices)
    PMM_unlockLPM5();
#endif
}


