lab_04a_clock

This lab explores system initialization, of which, setting up the clocks are a
big part. After adding code to configure the clocks, we'll run it and verify
that we're seeing the expected clock rates.

An optional step has us reconfigure MCLK to use a slower oscillator, which 
makes our LED blink very, very slowly.

General procedure:
- Import lab_03a_gpio
- Create myClocks.c to hold our clock initialization code
- Reorganize the system init code in main() to fit our chapter's 'template'
- Debug the code using Watch Expressions to view the various clock rates
  of our MSP processor
- Change MCLK to run slowly ... and wait very patiently for the LED to
  turn on & off

File source code in this readme file:
- main.c
- myClocks.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_04a_clock project) (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"
#include "myClocks.h"


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

    // Initialize GPIO
    initGPIO();

    // Initialize clocks
    initClocks();

    while(1) {
        // Turn on LED
        MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait about a second
        __delay_cycles( HALF_SECOND );

        // Turn off LED
        MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

        // Wait another second
        __delay_cycles( HALF_SECOND );
    }
}

// ----------------------------------------------------------------------------
// myClocks.c  (for lab_04a_clock project)  (MSP432 Launchpad)
//
// This routine sets up all five clocks on the MSP432.
//
// Oscillators:
//    DCO    =   3MHz  (default is 3MHz)  Internal high-frequency oscillator
//    REFO   =  32KHz  (default is 32KHz) Internal 32/128KHz reference oscillator
//    MODOSC =  24MHz                     Internal oscillator
//    SYSOSC =   5MHz                     Internal oscillator
//    VLO    = 9.4KHz                     Internal very low power, low frequency oscillator
//    LFXT   =  --KHz  (not configured)   External crystal input
//    HFXT   =  --MHz  (not configured)   External crystal input
//
// Internal Clocks:
//    ACLK   = REFO/2   =  16KHz
//    BCLK   = REFO     =  32KHz
//    MCLK   = MODOSC/4 =   6MHz
//    HSMCLK = DCO/2    = 1.5Mhz
//    SMCLK  = DCO/4    = 750kHz
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>
#include <driverlib.h>
#include "myClocks.h"


//***** Defines ***************************************************************
// See additional #defines in 'myClocks.h'


//***** Global Variables ******************************************************
uint32_t myDCO     = 0;
uint32_t myACLK    = 0;
uint32_t myBCLK    = 0;
uint32_t myHSMCLK  = 0;
uint32_t myMCLK    = 0;
uint32_t mySMCLK   = 0;


//***** initClocks ************************************************************
void initClocks(void) {

    //*************************************************************************
    // Configure Power, Waitstates, FPU
    //*************************************************************************
    // If the CPU (MCLK) is running greater than 24MHz, the core volage should
    // be set to 1 (VCORE1) otherwise VCORE0 will save power
    //MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);                                 // See Power/Voltage Regulation chapter for more info

    // Similarly, for faster MCLK settins, you need to appropriately set the Flash
    // access waitstates:  0-16MHz = 0 waits; 16-32MHz = 1 wait; 32-48MHz = 2 waits
    //MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);                               // See Non-Volatile Memory chapter for more info
    //MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    // You can improve the efficiency in DCO calculations (when  tuning or getting
    // the DCO frequency) by enabling the floating-point unit (FPU)
    //MAP_FPU_enableModule();


    //*************************************************************************
    // Configure Oscillators
    //*************************************************************************
    // Tell DriverLib what crystal frequencies are provided to the processor by
    // the LFXT and HFXT inputs; this info is used for the crystal start and the
    // 'get' clock functions
    MAP_CS_setExternalClockSourceFrequency(
            LFXT_CRYSTAL_FREQUENCY_IN_HZ,                                       // LaunchPads LFXT crystal frequency
            HFXT_CRYSTAL_FREQUENCY_IN_HZ                                        // LaunchPads HFXT crystal frequency
    );

    // Verify if the default clock settings are as expected
    myDCO    = MAP_CS_getDCOFrequency();
    myACLK   = MAP_CS_getACLK();
    myBCLK   = MAP_CS_getBCLK();
    myMCLK   = MAP_CS_getMCLK();
    myHSMCLK = MAP_CS_getHSMCLK();
    mySMCLK  = MAP_CS_getSMCLK();

    // Set the DCO Frequency (3MHz)                                             // Default is 3MHz, but SystemInit() sets it to 1.5MHz
    MAP_CS_setDCOCenteredFrequency( CS_DCO_FREQUENCY_3 );

//    // Set Reference Oscillator (REFO) to 32KHz                                 // This function sets REFO to 32KHz (which is the default rate)
//    MAP_CS_setReferenceOscillatorFrequency( CS_REFO_32KHZ );


    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Setup ACLK to use the on-chip REFO as its oscillator source divided by 2 (16KHz)
    MAP_CS_initClockSignal(
            CS_ACLK,                                     // Clock you're configuring
            CS_REFOCLK_SELECT,                           // Clock source
            CS_CLOCK_DIVIDER_2                           // Divide down clock source by this much
    );

    // Setup BCLK to use the on-chip REFO as its oscillator source (32KHz)
    MAP_CS_initClockSignal(
            CS_BCLK,                                     // Clock you're configuring
            CS_REFOCLK_SELECT,                           // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much (ignored for BCLK)
    );

    // Set the MCLK to use the internal MODOSC oscillator source divided by 4 (6MHz)
    MAP_CS_initClockSignal(
            CS_MCLK,                                     // Clock you're configuring
            CS_MODOSC_SELECT,                            // Clock source
            CS_CLOCK_DIVIDER_4                           // Divide down clock source by this much
    );

    // Set the HSMCLK to use the internal DCO oscillator source divided by 2 (1.5MHz)
    MAP_CS_initClockSignal(
            CS_HSMCLK,                                   // Clock you're configuring
            CS_DCOCLK_SELECT,                            // Clock source
            CS_CLOCK_DIVIDER_2                           // Divide down clock source by this much
    );

    // Set the SMCLK to use the internal DCO oscillator source divided by 4 (750KHz)
    // Per hardware spec, SMCLK and HSMCLK use the same oscillator source, you should configure them with the same
    // CS_xxxCLK_SELECT value; changing either of these clocks changes the source for them both
    MAP_CS_initClockSignal(
            CS_SMCLK,                                    // Clock you're configuring
            CS_DCOCLK_SELECT,                            // Clock source
            CS_CLOCK_DIVIDER_4                           // Divide down clock source by this much
    );

    // Verify that the modified clock settings are as expected
    myDCO    = MAP_CS_getDCOFrequency();
    myACLK   = MAP_CS_getACLK();
    myBCLK   = MAP_CS_getBCLK();
    myMCLK   = MAP_CS_getMCLK();
    myHSMCLK = MAP_CS_getHSMCLK();
    mySMCLK  = MAP_CS_getSMCLK();
}


/*
 * myClocks.h
 *
 */

#ifndef MYCLOCKS_H_
#define MYCLOCKS_H_

//***** Prototypes ************************************************************
void initClocks(void);

//***** Defines ***************************************************************
#define LFXT_CRYSTAL_FREQUENCY_IN_HZ        32768                               // Freq of external crystal
#define HFXT_CRYSTAL_FREQUENCY_IN_HZ     48000000                               // Freq of external crystal

// The following oscillator and clock frequencies are set in the associated 'C' file
#define myDCO_FREQUENCY_IN_HZ             6000000
#define myREFO_FREQUENCY_IN_HZ          4 * 32768

#define myACLK_FREQUENCY_IN_HZ          32768 / 2
#define myBCLK_FREQUENCY_IN_HZ              32768
#define myMCLK_FREQUENCY_IN_HZ            6000000
#define myHSMCLK_FREQUENCY_IN_HZ          1500000
#define mySMCLK_FREQUENCY_IN_HZ        750 * 1000


#endif /* MYCLOCKS_H_ */


