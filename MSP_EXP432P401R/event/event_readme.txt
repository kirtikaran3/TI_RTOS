Example Summary
------------------
The event example shows how to use the ti.sysbios.knl.Event module which
addresses the problem of a task needing to wait on multiple events prior to
continuing its execution.

Example Usage
---------------
Run the application, two tasks, the writertask and readertask, will print
messages being passed between them to the console.

Application Design Details
-------------------------
A reader task, readertask, waits on several events. It needs to wake up
when events with Ids EventId and EventId1 or if EventId2 occurs.
The ti.sysbios.knl.Event module provides an Event_pend() API that allows a
task to specify an AND mask and an OR mask. These masks determine which
event(s) must occur before returning from Event_pend().

The event related to EventId is posted by a Clock Instance using
a call to Event_post(). This is an example of the Explicit
posting of events.

EventId1 is posted from within a Semaphore Instance. This is an example
of the Implicit posting of events. This is useful when a task needs to wait on
a semaphore and an event which is explicitly posted. Eventid2 is posted from
within a Mailbox Instance which occurs wihtin the context of the writertask.
This is also an example of Implicit posting of events.

The Semaphore Module does not support events by default, as such, the
Semaphore.supportsEvents flag is set to true within the applications
configuration file event.cfg.

Note: For IAR users using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.

References
-----------
For more help, search either the SYS/BIOS User Guide or the TIRTOS
Getting Started Guide within your TIRTOS installation.