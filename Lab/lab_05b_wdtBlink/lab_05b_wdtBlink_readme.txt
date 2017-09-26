lab_05b_wdtBlink

The goal of this lab is to blink the LED. Rather than using a __delay_cycles() 
function, we’ll actually use the watchdog timer to tell us when to toggle the LED.

In the last lab we used the Watchdog timer as a … well, watchdog timer. In all 
other exercises, thus far, we just turned it off with WDT_A_hold().

In this lab exercise, we’re going to use it as a standard timer (called 
‘interval’ timer) to generate a periodic interrupt. In the interrupt service 
routine, we’ll toggle the LED.

As we write the ISR code, you may notice that the Watchdog Interval Timer 
interrupt has a dedicated interrupt vector. (Whereas the GPIO Port interrupt 
had 8 interrupts that shared one vector.) 

Basic Steps:
- Import project from DriverLib Examples
- Review the example's code
- Run and observe the LED flashing
- Change the WDT_A timers rate
- Run, to watch the timer run slower!


File source code in this readme file:
- wdt_a_interval_mode.c

Final code ... you can copy from this if you want to save typing time & effort.


/*
 * -------------------------------------------
 *    MSP432 DriverLib - v3_10_00_09 
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
/*******************************************************************************
 * MSP432 WDT - Interval Mode
 *
 * Description: In this example, the WDT module is setup in interval mode. This
 * turns the watchdog timer into a normal 16-bit up counter that can be operated
 * in LPM3 mode. Given that the WDT can be operated in LPM3 mode, this is useful 
 * to wake up processor once put to LPM3. In this example, a simple LED is 
 * blinked at a constant interval using the WDT.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P1.0  |---> P1.0 LED
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 * Author: Timothy Logan
 ******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>

#include <stdbool.h>

int main(void)
{
    /* Halting the Watchdog (while we set it up) */
    MAP_WDT_A_holdTimer();

    /* Setting MCLK to REFO at 128Khz for LF mode */
    MAP_CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);
    MAP_CS_initClockSignal(CS_MCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_PCM_setPowerState(PCM_AM_LF_VCORE0);

    /* Enabling SRAM Bank Retention. This is done to minimize power consumption
        in LPM3 mode. With only SRAM bank 1 enabled, when the device is in LPM3
         only the first bank of SRAM memory has to be retained (this minimizing
         power consumption. */
    MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);

    /* Configuring GPIO1.0 as an output  */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring WDT in interval mode to trigger every 32K clock iterations.
     * This comes out to roughly every 3.5 seconds */
    MAP_WDT_A_initIntervalTimer(WDT_A_CLOCKSOURCE_VLOCLK,
                                        WDT_A_CLOCKITERATIONS_32K);
    

    /* Enabling interrupts and starting the watchdog timer */
    MAP_Interrupt_enableInterrupt(INT_WDT_A);
    MAP_Interrupt_enableMaster();
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_WDT_A_startTimer();

    /* LPM3ing when not in use */
    while (1)
    {
        MAP_PCM_gotoLPM3();
    }
}

/* WDT ISR - This ISR toggles the LED on P1.0 */
void WDT_A_IRQHandler(void)
{
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}


