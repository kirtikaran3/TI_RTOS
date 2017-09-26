// ----------------------------------------------------------------------------
// myClocks.c  (MSP432 Launchpad)
//
// This routine sets up all five clocks on the MSP432.
//
// Oscillators:
//    DCO    =   6MHz  (default is 3MHz)  Internal high-frequency oscillator
//    REFO   = 128KHz  (default is 32KHz) Internal 32/128KHz reference oscillator
//    MODOSC =  24MHz                     Internal oscillator
//    SYSOSC =   5MHz                     Internal oscillator
//    VLO    = 9.4KHz                     Internal very low power, low frequency oscillator
//    LFXT   =  32KHz                     External crystal input
//    HFXT   =  48MHz                     External crystal input
//
// Internal Clocks:
//    ACLK   = LFXT     =  32KHz
//    BCLK   = REFO     = 128KHz
//    MCLK   = DCO      =   6MHz
//    HSMCLK = HFXT/8   =   6Mhz
//    SMCLK  = DCO/16   =   3MHz
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>
#include <stdbool.h>
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

    // Initialize the LFXT crystal oscillator (using a timeout in case there is a problem with the crystal)
    // - This requires PJ.0 and PJ.1 pins to be connected (and configured) as "crystal" pins.
    // - Another alternative is to use the non-timeout function which "hangs" if LFXT doesn't get configured:
    MAP_CS_startLFXT( CS_LFXT_DRIVE0 );

    // Starting HFXT in non-bypass mode with a timeout. Returns STATUS_SUCCESS if initializes successfully.
    MAP_CS_startHFXT( false );

    // Set the DCO Frequency (6MHz)
    MAP_CS_setDCOCenteredFrequency( CS_DCO_FREQUENCY_6 );

    // Set Reference Oscillator (REFO) to 128KHz
    MAP_CS_setReferenceOscillatorFrequency( CS_REFO_128KHZ );


    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Set ACLK to use LFXT as its oscillator source (32KHz)
    // With a 32KHz crystal and a divide by 1, ACLK should run at that rate
    MAP_CS_initClockSignal(
            CS_ACLK,                                     // Clock you're configuring
            CS_LFXTCLK_SELECT,                           // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much
    );

    // Setup BCLK to use the on-chip REFO as its oscillator source (128KHz)
    MAP_CS_initClockSignal(
            CS_BCLK,                                     // Clock you're configuring
            CS_REFOCLK_SELECT,                           // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much (ignored for BCLK)
    );

    // Set the MCLK to use the internal DCO oscillator (DCO was configured earlier in this function for 6MHz)
    MAP_CS_initClockSignal(
            CS_MCLK,                                     // Clock you're configuring
            CS_DCOCLK_SELECT,                            // Clock source
            CS_CLOCK_DIVIDER_1                           // Divide down clock source by this much
    );

    // Set the HSMCLK to use the HFXT (high freq external) crystal oscillator source divided by 8 (6MHz)
    MAP_CS_initClockSignal(
            CS_HSMCLK,                                   // Clock you're configuring
            CS_HFXTCLK_SELECT,                           // Clock source
            CS_CLOCK_DIVIDER_8                           // Divide down clock source by this much
    );

    // Set the SMCLK to use the HFXT (high freq external) crystal oscillator source divided by 16 (3MHz)
    // Per hardware spec, SMCLK and HSMCLK use the same oscillator source, you should configure them with the same
    // CS_xxxCLK_SELECT value; changing either of these clocks changes the source for them both
    MAP_CS_initClockSignal(
            CS_SMCLK,                                    // Clock you're configuring
            CS_HFXTCLK_SELECT,                           // Clock source
            CS_CLOCK_DIVIDER_16                          // Divide down clock source by this much
    );

    // Verify that the modified clock settings are as expected
    myDCO    = MAP_CS_getDCOFrequency();
    myACLK   = MAP_CS_getACLK();
    myBCLK   = MAP_CS_getBCLK();
    myMCLK   = MAP_CS_getMCLK();
    myHSMCLK = MAP_CS_getHSMCLK();
    mySMCLK  = MAP_CS_getSMCLK();
}

