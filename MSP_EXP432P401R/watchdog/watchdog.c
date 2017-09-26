/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== watchdog.c ========
 */

#include <stdbool.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Watchdog.h>

/* Example/Board Header files */
#include "Board.h"

#define TASKSTACKSIZE   512

bool flag = false;

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
Watchdog_Handle watchdogHandle;

 /*
 *  ======== watchdogCallback ========
 *  Watchdog interrupt callback function. It toggles and LED, and if a button
 *  has not been pressed, clears the watchdog interrupt flag.
 */
void watchdogCallback(uintptr_t unused)
{
    /* Clear watchdog interrupt flag */
    Watchdog_clear(watchdogHandle);
    GPIO_toggle(Board_LED0);
}

/*
 *  ======== gpioButtonFxn ========
 *  Callback function for the GPIO interrupt on Board_BUTTON0.
 */
void gpioButtonFxn(unsigned int index)
{
    flag ^= true;
    GPIO_write(Board_LED0, Board_LED_ON);
}

/*
 *  ======== taskFxn ========
 *  Sets a flag and clears the watchdog timer if the button has been pressed.
 */
Void taskFxn(UArg arg0, UArg arg1)
{

    while (true) {
        /* Prevent Watchdog ISR trigger if button was pushed */
        if (flag) {
            Watchdog_clear(watchdogHandle);
        }
    }
}

/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;
    Watchdog_Params params;

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initWatchdog();

    /* Construct BIOS objects */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, taskFxn, &taskParams, NULL);

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    /* Install Button callback */
    GPIO_setCallback(Board_BUTTON0, gpioButtonFxn);

    /* Enable interrupts */
    GPIO_enableInt(Board_BUTTON0);

    System_printf("Starting the Watchdog example\nSystem provider is set to "
                  "SysMin. Halt the target to view any SysMin contents in"
                  " ROV.\n");

    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Create and enable a Watchdog with resets disabled */
    Watchdog_Params_init(&params);
    params.callbackFxn = (Watchdog_Callback)watchdogCallback;
    params.resetMode = Watchdog_RESET_OFF;

    watchdogHandle = Watchdog_open(Board_WATCHDOG0, &params);
    if (watchdogHandle == NULL) {
        System_abort("Error opening Watchdog!\n");
    }

    /* Start BIOS */
    BIOS_start();

    return (0);
}
