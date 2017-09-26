Example Summary
------------------
This application shows how to use the SYS/BIOS ti.sysbios.knl.Swi module by
exercising some of the various Swi APIs.

Example Usage
---------------
Run the application, two Swis, each configured differently, will run their
prospective functions. At that time, data such as when the application entered
and exitied the respective Swis will be output to the console.

Application Design Details
-------------------------
The SYS/BIOS objects, such as the two Swis with different priorities and
trigger counts, used within the application are constructed within main().

The applications` task, taskFxn will run and call a series of Swi APIs
designed to alter the Swis trigger counts and eventually run the Swi
functions.
BIOS_exit() is called from the second Swi to terminate the application.

See the projects` main function for more information on how the SYS/BIOS
objects are constructed.

Note: For IAR users using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.

References
------------
For more help, search either the SYS/BIOS User Guide or the TIRTOS
Getting Started Guide within your TIRTOS installation.