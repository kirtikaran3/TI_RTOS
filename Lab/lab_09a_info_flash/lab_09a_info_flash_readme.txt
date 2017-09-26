lab_09a_info_flash  (MSP432)

This lab uses non-volatile memory to store a data value so that it will be 
available after a power-cycle.

In our MSP430 NVM solutions, we placed the persistent data into a Flash INFO block 
of memory. Unfortunately, the MSP432P401R device requires all of the Info memory
for other purposes. Therefore, we'll use the last segment of flash
to hold our persistent, non-volatile data. In other words, we'll use Bank1/Sector31
in Main flash. 

The code will keep track of how many power-cycles (BOR's) have occurred.
After power up and initializing the GPIO, the code looks for a count value in
NVM, it then increments the count value and:

  - Writes the updated value back to Flash
  - Prints out the # of power-cycle counts with printf()
  - Blinks the LED count # of times
  
If the write fails, then the program will:

  - Print out an error statement
  - Quickly blink the LED 1000 times

To minimize your typing, we have created the project for you. The "hello.c"
file in this project is an amalgam of labs:

  - lab_03a_gpio for the gpio setup
  - lab_04b_wdt  for the printf functionality

To this we've added:
  - Logic to manage the "count" value
  - F5529 and MSP432: Uses function to write to Flash (FRAM doesn't require complex write algorithm)
  - FR4133: Here, we use a function to write to FRAM since this device does not contain
            a full fledged MPU. The FRAM write function turns off the memory protection flag,
	    performs a write, and turns the protection flag back on. While this function 
	    works with the FR5xx/6xx devices, the MPU can be configured to allow full access
	    to portions of FRAM while still leaving others protected.
  - You will need to fill in a few answers from your Lab 9a worksheet

There is no MPU "protection" setup for the 'FR5969/6989 FRAM devices in this exercise. Instead,
this is shown in lab_09b_mpu_gui or lab_09b_mpu_with_driverlib. 
(Once again, note that the F5529 and FR4133 devices don't have an MPU.)


Basic Steps:
- Import project from lab folder
- Fill in the blanks
- ('FR5xx/6xx only) Modify setting in linker command file
- Build and launch debugger
- Open memory window
- Step thru code to verify it works
- Restart program
- Step through code again ... hopefully it retained its count value
- Terminate the debugger
- Reset the Launchpad with the reset button ... does the LED blink 1-more-time
  each time its reset or power-cycled?


File source code in this readme file:
- hello.c

Final code ... you can copy from this if you want to save typing time & effort.


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

