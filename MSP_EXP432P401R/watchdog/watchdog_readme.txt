Example Summary
----------------
This application demonstrates how to use the TI-RTOS Watchdog driver to call a
function on expiration of the watchdog timer.

Peripherals Exercised
---------------------
Board_LED0      Indicator LED
Board_BUTTON0   Used to control the application
Board_WATCHDOG0 Timer to reset the device

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| MSP-EXP430F5529LP | N/A                                                    |
| MSP-EXP430FR5969  |                                                        |
| MSP-EXP430FR6989  |                                                        |
| MSP-EXP432P401R   |                                                        |
| DK-TM4C123G       |                                                        |
| DK-TM4C129X       |                                                        |
| EK-TM4C123GXL     |                                                        |
| EK-TM4C1294XL     |                                                        |
| EK-TM4C129EXL     |                                                        |
| TMDXDOCK28M36     |                                                        |
| TMDXDOCKH52C1     |                                                        |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
The application turns ON Board_LED0 to indicate TI-RTOS driver initialization
is complete.

Board_LED0 is toggled repeatedly until Board_BUTTON0 is pressed. A flag is
toggled once Board_BUTTON0 is pushed. Once this occurs, the watchdog timer will
be continuously cleared and Board_LED0 will stay lit.

Application Design Details
--------------------------
The application's main function opens a Watchdog driver object. The watchdog
task toggles a flag depending on whether Board_BUTTON0 has been pushed. If the
flag is set, the task will continously call Watchdog_clear() and set Board_LED0.
If the flag is not set, then the timer will expire and trigger an interrupt to
toggle Board_LED0.

For GNU and IAR users, please read the following website for details
about semi-hosting
http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting