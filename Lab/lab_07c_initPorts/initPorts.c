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

