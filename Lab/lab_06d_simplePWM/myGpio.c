// ----------------------------------------------------------------------------
// myGpio.c  (for lab_06d_simplePWM project)  (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"

//***** Defines ***************************************************************


//***** Global Variables ******************************************************
// Array describing which bits/pins of Port 2 to remap from their default settings
const uint8_t myP2map[] =
{
        PM_NONE,                                                                // P2.0 (Red LED)
        PM_NONE,                                                                // P2.1 (Green LED)
        PM_TA0CCR2A,                                                            // P2.2 (Blue LED)
        PM_NONE,                                                                // P2.3
        PM_NONE,                                                                // P2.4
        PM_NONE,                                                                // P2.5
        PM_NONE,                                                                // P2.6
        PM_NONE                                                                 // P2.7
};


//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void initGPIO(void) {

    // Set pin P1.0 to output direction and turn LED off                        // Red LED (LED1)
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
    MAP_GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Set pin P2.0/2.1/2.1 to output direction and turn LED off                // RGB LED (LED2)
    MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 );
    //MAP_GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 );   // Commented out original configuration of pins P2.0/1/2
    MAP_GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN1 );                 // Configuration of P2.0 and P2.1

    // Remap pin P2.2 to TACCR0
    MAP_PMAP_configurePorts(
        ( const uint8_t * ) myP2map,                                            // Pointer to port map setting (defined above)
		PMAP_P2MAP,                                                             // Which port to remap
        1,                                                                      // Number of ports to configure
        PMAP_DISABLE_RECONFIGURATION                                            // Disallow reconfiguration of the port (until next reset)
    );

    // Configure P2.1 as a timer pin
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P2,
        GPIO_PIN2,
        GPIO_PRIMARY_MODULE_FUNCTION
    );


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
