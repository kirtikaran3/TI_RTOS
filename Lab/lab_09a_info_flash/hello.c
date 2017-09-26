// ----------------------------------------------------------------------------
// hello.c  (for lab_09a_info_flash project)  (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                            // Standard include file
#include <driverlib.h>                                                         // DriverLib include file
#include <stdio.h>
#include "myGpio.h"                                                            // GPIO setup for LED's
#include "myClocks.h"
#include "myFlash.h"
#include "myFlash_sectors.h"                                                   // Defines start address for each flash sector


//***** Defines ***************************************************************
#define ONE_SECOND        3000000                                              // Well, it's about a second
#define HALF_SECOND       1500000
#define ONE_THIRD_SECOND  1000000
#define ONE_100TH_SECOND  30000


//***** Global Variables ******************************************************
#pragma LOCATION ( myPersistentData, FLASH_START_ADDRESS_BANK_1_SECTOR_31 )    // Specify 'myPersistentData' should be located in flash (Bank 1, Sector 31)
#pragma NOINIT   ( myPersistentData )                                          // Compiler should not try to initialize 'myPersistentData' at startup


    //*************************************************************************
    // Define new structure data type:  myFlash_Data
    //
    // This data type includes all of the information we want to be persistent,
    // that is, all the data we want to keep in flash so that it remains, even
    // when the device is reset and/or the power is removed.
    //
    // All we really need is the single variable 'count'. The other variable
    // was included just to provide an example where you might need more than
    // one persistent variable.
    //*************************************************************************
	struct myFlash_Data                                                        // New data type is name 'myFlash_Data'
	{
		uint32_t count;                                                        // Variable/location that holds the number of times device is reset (since code was loaded)
		uint32_t someOtherData;                                                // A second variable - provided an example just-in-case you need more than one persistent variable
	} values;                                                                  // Specifies a variable called 'values' that utilizes the new structure definition

    //*************************************************************************
    // Define new union data type:  myFlash_Record
    //
	// This type creates a union of the data we want to be persistent in our
	// program (i.e. the 'myFlash_Data' type defined above), as well as an
	// array that equals the length of one flash sector on the MSP432.
	//
    // Since a union is as big as its largest value, this data type allocates
	// an entire flash sector.

	// While the program only requires a single persistent integer, flash memory
    // must be erased one sector at a time. Therefore, the program must take care
    // not to accidentally erase any other data in the sector. We've chosen to
    // handle this by not letting any other data be allocated to the sector.
    // (You could also do this by temporarily copying everything from the sector
    // to SRAM and then writing it back.)
    //*************************************************************************
	union myFlash_Record                                                       // Define a new union data type named 'myFlash_Record'
	{
		struct myFlash_Data  values;                                           // Persistent 'values' from previously defined 'myFlash_Data' structure
		uint8_t              sector[ FLASH_SECTOR_SIZE_IN_BYTES ];             // Array named 'sector' that allocates sector-length number of bytes
	} myPersistentData;                                                        // Create a global variable instance of our union named 'myPersistentdata'


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    //*************************************************************************
    // Initialization code
    //*************************************************************************
	uint32_t i = 0;                                                            // Loop counter
	uint32_t c = 0;                                                            // Local copy of 'count'
	bool status = true;

    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Initialize GPIO
    initGPIO();

    // Initialize clocks
    initClocks();

    //*************************************************************************
    // Read and increment 'count' variable which is located in flash
    //*************************************************************************
    if ( myPersistentData.values.count == 0xFFFFFFFF ) {                       // Reads 'count' from flash and compares it to value of erased flash memory (0xFFFFFFFF)
    	c = 0;                                                                 // Set local count to 0, if flash location is equal to erased flash memory value
    }
	else {
		c = myPersistentData.values.count;                                     // Set local count variable to 'count'
	}

    c++;                                                                       // Increment local count value

    // Write local value of count back to origianl 'count' location in flash
    status = myFlash_write( &c, &myPersistentData.values.count, sizeof( myPersistentData.values.count ) );

    // If write function exited successfully:
    // -> Then printf() the number of times and the device has been reset and blink the LED that many times
    // -> Else printf() an error message and quickly blink the LED 1000 times
    if ( status )
    {
		//*********************************************************************
		// Printf() and blink the LED to indicate number of device resets
		//*********************************************************************
		// Inform the programmer how many times the board has been power-on reset
		//   which isn't possible without some form of non-volatile memory
		printf( "This program has been power cycled %d times\n", c );

		// Flash the LED 'count' number of times ... this way you can 'see' how many
		//   times a power-on reset has occurred, even if you can't read the printf()
		for (i = 0; i < myPersistentData.values.count; i++)
		{
			MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );                // Turn on LED
			__delay_cycles( HALF_SECOND );                                     // Wait a half second

			MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );                 // Turn off LED
			__delay_cycles( ONE_THIRD_SECOND );                                // Wait a third of a second
		}
    }
    else
    {
		//*********************************************************************
		// Printf() and quick blink the LED to indicate an error
		//*********************************************************************
		// Inform the programmer that the write failed
		printf( "This write to flash failed\n" );

		// Flash the LED 'count' number of times ... this way you can 'see' how many
		//   times a power-on reset has occurred, even if you can't read the printf()
		for (i = 0; i < 1000; i++)
		{
			MAP_GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );                // Turn on LED
			__delay_cycles( ONE_100TH_SECOND );                                // Wait 1/100th of a second

			MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );                 // Turn off LED
			__delay_cycles( ONE_100TH_SECOND );                                // Wait 1/100th of a second
		}
    }

    // Trap program here, so the program doesn't exit
    while(1);
}
