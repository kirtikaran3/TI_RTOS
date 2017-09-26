/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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
 *  ======== tcpEchoCC3X00.c ========
 */

#include <string.h>
#include <stdbool.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* SimpleLink Wi-Fi Host Driver Header files */
#include <simplelink.h>

/* Example/Board Header file */
#include "Board.h"

/* Local Platform Specific Header file */
#include "sockets.h"

/* Port number for listening for TCP packets */
#define TCPPORT         1000

#define TCPPACKETSIZE   256
#define TASKSTACKSIZE   1024

extern bool smartConfigFlag;
Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

/*
 *  ======== gpioButtonFxn ========
 *  Callback function for the GPIO interrupt on Board_BUTTON1.
 */
void gpioButtonFxn(unsigned int index)
{
    /* Begin smart config process */
    smartConfigFlag = true;
}

/*
 *  ======== echoFxn ========
 *  Echoes TCP messages.
 */
void echoFxn(int port)
{
    int         bytesRcvd;
    int         bytesSent;
    int         status;
    int         clientfd;
    int         server;
    sockaddr_in localAddr;
    sockaddr_in clientAddr;
    socklen_t   addrlen = sizeof(clientAddr);
    char        buffer[TCPPACKETSIZE];

    server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == -1) {
        System_printf("Error: socket not created.\n");
        goto shutdown;
    }

    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(port);

    status = bind(server, (const sockaddr *)&localAddr, sizeof(localAddr));
    if (status == -1) {
        System_printf("Error: bind failed.\n");
        goto shutdown;
    }

    status = listen(server, 0);
    if (status == -1){
        System_printf("Error: listen failed.\n");
        goto shutdown;
    }

    while ((clientfd =
            accept(server, (sockaddr *)&clientAddr, &addrlen)) != -1) {

        while ((bytesRcvd = recv(clientfd, buffer, TCPPACKETSIZE, 0)) > 0) {
            bytesSent = send(clientfd, buffer, bytesRcvd, 0);
            if (bytesSent < 0 || bytesSent != bytesRcvd) {
                System_printf("Error: send failed.\n");
                break;
            }
        }

        /* addrlen is a value-result param, must reset for next accept call */
        addrlen = sizeof(clientAddr);
        close(clientfd);
    }

    System_printf("Error: accept failed.\n");

shutdown:
    if (server >= 0) {
        close(server);
    }
}

/*
 *  ======== tcpEchoTask ========
 */
Void tcpEchoTask(UArg arg0, UArg arg1)
{
    void *netIF;

    /* Open WiFi and await a connection */
    netIF = socketsStartUp();

    echoFxn(TCPPORT);

    /* Close the network - don't do this if other tasks are using it */
    socketsShutDown(netIF);
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Construct BIOS objects */
    Task_Params taskParams;

    /* Call board init functions. */
    Board_initGeneral();
    Board_initGPIO();
    Board_initWiFi();

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    taskParams.priority = 1;
    Task_construct(&task0Struct, (Task_FuncPtr)tcpEchoTask, &taskParams, NULL);

    /* Install Button callback */
    GPIO_setCallback(Board_BUTTON1, gpioButtonFxn);

    /* Enable interrupts */
    GPIO_enableInt(Board_BUTTON1);

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the TCP Echo example for the CC3X00 \n"
                  "System provider is set to SysMin. Halt the target to view"
                  " any SysMin content in ROV.\n");

    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
