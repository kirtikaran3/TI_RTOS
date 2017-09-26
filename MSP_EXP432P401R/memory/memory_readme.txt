Example Summary
------------------
This application demonstrates shows the use of xdc.runtime.Memory module
and different xdc.runtime.IHeap implementations to manage memory.

Example Usage
---------------
Run the application, the two tasks that run will allocate memory from two
different IHeap implementations and print their memory statuses to the
console.

Application Design Details
-------------------------
The application dynamically constructs two tasks, both tasks use
xdc.runtime.Memory to allocate memory but use two different different
IHeap implementations or heaps.

The first task runs as taskFxn uses ti.sysbios.heaps.HeapBuf to
dynamically create its heap, taskHeap, because the allocations are of
a fixed size.

The second task run as task1Fxn uses ti.sysbios.heaps.HeapMem to
dynamically construct its heap, task1Heap, as this task allocates variable
block sizes. Both tasks print their heap status before and after allocations.

The system heap is created statically using ti.sysbios.heaps.HeapMem and is
plugged into xdc.runtime.memory as the defaultHeapInstance. Please see
the corresponding configuration file memory.cfg for details.

Note: For IAR users using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.

References
-----------
For more help, search either the SYS/BIOS User Guide or the TIRTOS
Getting Started Guide within your TIRTOS installation.