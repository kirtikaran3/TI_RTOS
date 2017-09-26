Example Summary
------------------
This example shows the use of xdc.runtime.Error module to both catch and raise
errors.

Example Usage
---------------
Run the application, it will attempt to incorrectly run certain SYS/BIOS APIs
in order to cause errors. These failures are printed the the console as
they occur.

Application Design Details
-------------------------
An Error_Block passed to Task_create() checked to see if the create
was unsuccessful due to a memory allocation failure. In no Error_Block
is passed, then the application will terminate when an error occurs.
This is shown in the Memory_alloc() call when the ErrorBlock
passed in is NULL.

The configuration file error.cfg shows how to plug error hook function
that will get called as soon as an error occurs rather than program
termination.

Note: For IAR users using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.

References
-----------
For more help, search either the SYS/BIOS User Guide or the TIRTOS
Getting Started Guide within your TIRTOS installation.