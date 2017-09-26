lab_05a_buttonInterrupt

When Lab 5a is complete, you should be able to push a button to toggle 
the LED on or off.

We begin by importing the solution from Lab 4a. After which we’ll need to 
delete a bit of ‘old’ code and add the following:
  - Setup the interrupt vector
  - Enable interrupts
  - Create an interrupt service routine (ISR)

Basic Steps:
- Import previous project's solution
- Add 'unused_interrupts.c' to project (MSP430 devices only)
- Delete the old LED blink code from while{} loop
- Add code to enable interrupts
- Run code checking interrupt IE and IFG registers,
  as well as seeing the interrupt occur and branch to the
  default ISR (MSP430: unused_interrupts.c; MSP432: startup_msp432p401r_ccs.c)
- Create an ISR and debug it (noticing that the IFG flag is never cleared)
- Go back and 'upgrade' the ISR, using the PORT1 IV register 
- Run and watch it work!


File source code in this readme file:
- main.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_05a_buttonInterrupt project) (MSP432 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <stdint.h>                                                             // Standard include file
#include <driverlib.h>                                                          // DriverLib include file
#include "myGpio.h"


//***** Prototypes ************************************************************


//***** Defines ***************************************************************


//***** Global Variables ******************************************************


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Disable interrupts (enabled by default on ARM Cortex-M processors) (set PRIMASK = 1) (Note that we show two different methods for accomplishing this)
    //__disable_irq();                                                          // Compiler and CMSIS intrinsic
    MAP_Interrupt_disableMaster();                                                  // DriverLib function

    // Initialize GPIO
    initGPIO();

    // Clear and enable individual IRQ interrupts (set appropriate bits in NVIC->IPR and NVIC->IER) (Note that we show two different methods for accomplishing this)
	//NVIC_ClearPendingIRQ ( PORT1_IRQn );                                      // (CMSIS) Clear pending bit for Port1 IRQ
    //NVIC_EnableIRQ( PORT1_IRQn );                                             // (CMSIS) Enable NVIC for individual Port1 IRQ
	Interrupt_unpendInterrupt ( INT_PORT1 );                                    // (DriverLib) Clear pending bit for Port1 IRQ
    MAP_Interrupt_enableInterrupt( INT_PORT1 );                                     // (DriverLib) Enable NVIC for individual Port1 IRQ

    // Enable interrupts globally (set PRIMASK = 0)
    //__enable_irq();                                                           // Compiler and CMSIS intrinsic
    MAP_Interrupt_enableMaster();                                                   // DriverLib function


    while(1) {
        __no_operation();                                                       // Placeholder for while loop (not required)
    }
}

//*****************************************************************************
//
// MSP432 Family Interrupt Vector Table for:  lab_05a_buttonInterrupt
//
//****************************************************************************

#include <stdint.h>
#include "myGpio.h"


/* Forward declaration of the default fault handlers. */
static void resetISR(void);
static void nmiISR(void);
static void faultISR(void);
static void defaultISR(void);


/* External declaration for the reset handler that is to be called when the */
/* processor is started                                                     */
extern void _c_int00(void);

/* External declaration for system initialization function                  */
extern void SystemInit(void);

/* Linker variable that marks the top of the stack. */
extern unsigned long __STACK_END;


/* External declarations for the interrupt handlers used by the application. */

/* To be added by user */
// See #included file(s) above


/* Interrupt vector table.  Note that the proper constructs must be placed on this to  */
/* ensure that it ends up at physical address 0x0000.0000 or at the start of          */
/* the program if located at a start address other than 0.                            */
#pragma RETAIN(interruptVectors)
#pragma DATA_SECTION(interruptVectors, ".intvecs")
void (* const interruptVectors[])(void) =
{
    (void (*)(void))((uint32_t)&__STACK_END),
                                            /* The initial stack pointer */
    resetISR,                               /* The reset handler         */
    nmiISR,                                 /* The NMI handler           */
    faultISR,                               /* The hard fault handler    */
    defaultISR,                             /* The MPU fault handler     */
    defaultISR,                             /* The bus fault handler     */
    defaultISR,                             /* The usage fault handler   */
    0,                                      /* Reserved                  */
    0,                                      /* Reserved                  */
    0,                                      /* Reserved                  */
    0,                                      /* Reserved                  */
    defaultISR,                             /* SVCall handler            */
    defaultISR,                             /* Debug monitor handler     */
    0,                                      /* Reserved                  */
    defaultISR,                             /* The PendSV handler        */
    defaultISR,                             /* The SysTick handler       */
    defaultISR,                             /* PSS ISR                   */
    defaultISR,                             /* CS ISR                    */
    defaultISR,                             /* PCM ISR                   */
    defaultISR,                             /* WDT ISR                   */
    defaultISR,                             /* FPU ISR                   */
    defaultISR,                             /* FLCTL ISR                 */
    defaultISR,                             /* COMP0 ISR                 */
    defaultISR,                             /* COMP1 ISR                 */
    defaultISR,                             /* TA0_0 ISR                 */
    defaultISR,                             /* TA0_N ISR                 */
    defaultISR,                             /* TA1_0 ISR                 */
    defaultISR,                             /* TA1_N ISR                 */
    defaultISR,                             /* TA2_0 ISR                 */
    defaultISR,                             /* TA2_N ISR                 */
    defaultISR,                             /* TA3_0 ISR                 */
    defaultISR,                             /* TA3_N ISR                 */
    defaultISR,                             /* EUSCIA0 ISR               */
    defaultISR,                             /* EUSCIA1 ISR               */
    defaultISR,                             /* EUSCIA2 ISR               */
    defaultISR,                             /* EUSCIA3 ISR               */
    defaultISR,                             /* EUSCIB0 ISR               */
    defaultISR,                             /* EUSCIB1 ISR               */
    defaultISR,                             /* EUSCIB2 ISR               */
    defaultISR,                             /* EUSCIB3 ISR               */
    defaultISR,                             /* ADC14 ISR                 */
    defaultISR,                             /* T32_INT1 ISR              */
    defaultISR,                             /* T32_INT2 ISR              */
    defaultISR,                             /* T32_INTC ISR              */
    defaultISR,                             /* AES ISR                   */
    defaultISR,                             /* RTC ISR                   */
    defaultISR,                             /* DMA_ERR ISR               */
    defaultISR,                             /* DMA_INT3 ISR              */
    defaultISR,                             /* DMA_INT2 ISR              */
    defaultISR,                             /* DMA_INT1 ISR              */
    defaultISR,                             /* DMA_INT0 ISR              */
    Port1_ISR,                              /* PORT1 ISR                 */
    defaultISR,                             /* PORT2 ISR                 */
    defaultISR,                             /* PORT3 ISR                 */
    defaultISR,                             /* PORT4 ISR                 */
    defaultISR,                             /* PORT5 ISR                 */
    defaultISR,                             /* PORT6 ISR                 */
    defaultISR,                             /* Reserved 41               */
    defaultISR,                             /* Reserved 42               */
    defaultISR,                             /* Reserved 43               */
    defaultISR,                             /* Reserved 44               */
    defaultISR,                             /* Reserved 45               */
    defaultISR,                             /* Reserved 46               */
    defaultISR,                             /* Reserved 47               */
    defaultISR,                             /* Reserved 48               */
    defaultISR,                             /* Reserved 49               */
    defaultISR,                             /* Reserved 50               */
    defaultISR,                             /* Reserved 51               */
    defaultISR,                             /* Reserved 52               */
    defaultISR,                             /* Reserved 53               */
    defaultISR,                             /* Reserved 54               */
    defaultISR,                             /* Reserved 55               */
    defaultISR,                             /* Reserved 56               */
    defaultISR,                             /* Reserved 57               */
    defaultISR,                             /* Reserved 58               */
    defaultISR,                             /* Reserved 59               */
    defaultISR,                             /* Reserved 60               */
    defaultISR,                             /* Reserved 61               */
    defaultISR,                             /* Reserved 62               */
    defaultISR                              /* Reserved 63               */
};


/* This is the code that gets called when the processor first starts execution */
/* following a reset event.  Only the absolutely necessary set is performed,   */
/* after which the application supplied entry() routine is called.  Any fancy  */
/* actions (such as making decisions based on the reset cause register, and    */
/* resetting the bits in that register) are left solely in the hands of the    */
/* application.                                                                */
void resetISR(void)
{
    SystemInit();

    /* Jump to the CCS C Initialization Routine. */
    __asm("    .global _c_int00\n"
          "    b.w     _c_int00");
}


/* This is the code that gets called when the processor receives a NMI.  This  */
/* simply enters an infinite loop, preserving the system state for examination */
/* by a debugger.                                                              */
static void nmiISR(void)
{
    /* Fault trap exempt from ULP advisor */
    #pragma diag_push
    #pragma CHECK_ULP("-2.1")

    /* Enter an infinite loop. */
    while(1)
    {
    }

    #pragma diag_pop
}


/* This is the code that gets called when the processor receives a fault        */
/* interrupt.  This simply enters an infinite loop, preserving the system state */
/* for examination by a debugger.                                               */
static void faultISR(void)
{
    /* Fault trap exempt from ULP advisor */
    #pragma diag_push
    #pragma CHECK_ULP("-2.1")

    /* Enter an infinite loop. */
    while(1)
    {
    }

    #pragma diag_pop
}


/* This is the code that gets called when the processor receives an unexpected  */
/* interrupt.  This simply enters an infinite loop, preserving the system state */
/* for examination by a debugger.                                               */
static void defaultISR(void)
{
    /* Fault trap exempt from ULP advisor */
    #pragma diag_push
    #pragma CHECK_ULP("-2.1")

    /* Enter an infinite loop. */
    while(1)
    {
    }

    #pragma diag_pop
}

