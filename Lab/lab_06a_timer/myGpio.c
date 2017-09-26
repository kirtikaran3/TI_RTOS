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
