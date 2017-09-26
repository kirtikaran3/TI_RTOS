/*
 * ======== hal.c ========
 *
 */
#include "driverlib.h"
#include "hal.h"


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

  // Some MSP432 DriverLib functions were ported from an older version of
  // the MSP430 API and have not yet been updated
  #define  GPIO_selectInterruptEdge       GPIO_interruptEdgeSelect
  #define  GPIO_clearInterrupt            GPIO_clearInterruptFlag
  #define  MAP_GPIO_selectInterruptEdge   MAP_GPIO_interruptEdgeSelect
  #define  MAP_GPIO_clearInterrupt        MAP_GPIO_clearInterruptFlag

#endif

// * This function drives all the I/O's as output-low, to avoid floating inputs
// * (which cause extra power to be consumed).  This setting is compatible with
// * TI FET target boards, as well as the F5529, FR4133, FR5969 and MSP432 Launchpads.
void initGPIO(void) {

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

//**** Initialize LED's *******************************************************

    // Setup LED1 to output direction and then turn it off
    GPIO_setAsOutputPin(    LED1_PORT, LED1_PIN );
    GPIO_setOutputLowOnPin( LED1_PORT, LED1_PIN );

    // Setup LED2 to output direction and then turn it off
    GPIO_setAsOutputPin(    LED2_PORT, LED2_PIN );
    GPIO_setOutputLowOnPin( LED2_PORT, LED2_PIN );


//**** Unlock GPIO Pins** *****************************************************

    // a. This function is required for most FRAM devices; it unlocks the I/O,
    //    thus connecting the GPIO control registers to the actual I/O pins.
    // b. The devices which support LPMx.5 modes utilize the lock feature to
    //    maintain their values when the device is put ulta-low power mode.
    // c. The latest FRAM devices made this the default mode, therefore these
    //    you must call this function for all these devices.
    // d. Since unlocking the pins can create spureous interrupts, we suggest
    //    calling this function before initializing GPIO interrupts (as we
    //    do in the following section for the pins connected to the Launchpad's
    //    buttons
    // e. Finally, the "UNLOCK_PINS" symbol is defined in "hal.h".
    UNLOCK_PINS           // PMM_unlockLPM5();


//**** Initialize Buttons *****************************************************

    GPIO_setOutputHighOnPin(              BUTTON1_PORT, BUTTON1_PIN );
    GPIO_setAsInputPinWithPullUpResistor( BUTTON1_PORT, BUTTON1_PIN );
    GPIO_selectInterruptEdge(             BUTTON1_PORT, BUTTON1_PIN, GPIO_LOW_TO_HIGH_TRANSITION );
    GPIO_clearInterrupt(                  BUTTON1_PORT, BUTTON1_PIN );
    GPIO_enableInterrupt(                 BUTTON1_PORT, BUTTON1_PIN );

    GPIO_setOutputHighOnPin(              BUTTON2_PORT, BUTTON2_PIN );
    GPIO_setAsInputPinWithPullUpResistor( BUTTON2_PORT, BUTTON2_PIN );
    GPIO_selectInterruptEdge(             BUTTON1_PORT, BUTTON1_PIN, GPIO_LOW_TO_HIGH_TRANSITION );
    GPIO_clearInterrupt(                  BUTTON2_PORT, BUTTON2_PIN );
    GPIO_enableInterrupt(                 BUTTON2_PORT, BUTTON2_PIN );


//**** Initialize Timer Pins **************************************************

    // Setup timer pin (Note: TIMER_PIN defines the pin and pin/function
    //   depending on the CPU being used.)
#ifdef __MSP432P401R__
    GPIO_setAsPeripheralModuleFunctionOutputPin( TIMER_PORT, TIMER_PIN, GPIO_PRIMARY_MODULE_FUNCTION );
#else
    GPIO_setAsPeripheralModuleFunctionOutputPin( TIMER_PORT, TIMER_PIN );
#endif

//**** Initialize Clock Pins **************************************************

#ifdef LFXT_DO_SETUP
  #ifdef __MSP432P401R__
    GPIO_setAsPeripheralModuleFunctionInputPin( LFXTIN_PORT,  LFXTIN_PIN, GPIO_PRIMARY_MODULE_FUNCTION );
    GPIO_setAsPeripheralModuleFunctionInputPin( LFXTOUT_PORT, LFXTOUT_PIN, GPIO_PRIMARY_MODULE_FUNCTION );
  #else
    GPIO_setAsPeripheralModuleFunctionInputPin( LFXTIN_PORT,  LFXTIN_PIN );
    GPIO_setAsPeripheralModuleFunctionInputPin( LFXTOUT_PORT, LFXTOUT_PIN );
  #endif
#endif

#ifdef HFXT_DO_SETUP
  #ifdef __MSP432P401R__
    GPIO_setAsPeripheralModuleFunctionInputPin( HFXTIN_PORT,  HFXTIN_PIN, GPIO_PRIMARY_MODULE_FUNCTION );
    GPIO_setAsPeripheralModuleFunctionInputPin( HFXTOUT_PORT, HFXTOUT_PIN, GPIO_PRIMARY_MODULE_FUNCTION );
  #else
    GPIO_setAsPeripheralModuleFunctionInputPin( HFXTIN_PORT,  HFXTIN_PIN );
    GPIO_setAsPeripheralModuleFunctionInputPin( HFXTOUT_PORT, HFXTOUT_PIN );
  #endif
#endif

}

