Example Summary
------------------
This application demonstrates how to use the SYS/BIOS Clock and Task modules in
C to create a realtime clock/calendar.

Example Usage
---------------
Run the application, the two Clock objects (id 1 and id 2) current times
within the program will be continuously output to the console.

Application Design Details
-------------------------
The C class object, Clock, is driven by a variety of SYS/BIOS objects two
Clock objects, two Task objects and an Idle object. Each SYS/BIOS object has
its own instantiation of the Clock.  When the Clock, Task, and Idle functions
execute, they call their clocks' tick function, advancing the connected timer
by one second.

Because the functions execute at different rates, the attache clocks also
advance at different rates.  For example, one of the Clock objects, cl2,
has a period of one second.  When cl2 runs, it advances its timer by
one second. This results in an accurate clock.  On the other hand,
cl runs with every pass through the idle loop.  Therefore, the number of
seconds passed for its attached timer indicates the time spent in the idle loop.

Notes
------
See the projects main function for more information on how the SYS/BIOS
objects are constructed.

You can experiment with advancing Clock at different rates
by changing the Clocktick function

Some targets utilize the RTOS analyzer to output the Clock times as
Log_info events rather than using System_printf. To view the logs, open
Tools->RTOS Analyzer->Printf and Error Logs and switch to the Live
Session tab.

Note: For IAR users using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.

References
-----------
For more help, search either the SYS/BIOS User Guide or the TIRTOS
Getting Started Guide within your TIRTOS installation.