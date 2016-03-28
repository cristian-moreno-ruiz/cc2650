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
 *  ======== lab1-main.c ========
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

/* Example/Board Header files */
#include "Board.h"

/* Driverlib CPU functions, used here for CPUdelay*/
#include <driverlib/cpu.h>

/* Could be anything, like computing primes */
#define FakeBlockingSlowWork()   CPUdelay(12e6)
#define FakeBlockingFastWork()   CPUdelay(3e6)


/* Pin driver handles */
static PIN_Handle pinHandle;

/* Global memory storage for a PIN_Config table */
static PIN_State pinState;

Task_Struct workTask;
static uint8_t workTaskStack[256];

/*
 * Initial pin configuration table
 *   - LEDs Board_LED0 & Board_LED1 are off after the pin table is initialized.
 *   - Button is set to input with pull-up. On SmartRF06, BUTTON0 is UP, for the
 *     others it's the LEFT button.
 */
PIN_Config pinTable[] = {
    Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_BUTTON0 | PIN_INPUT_EN | PIN_PULLUP,
    PIN_TERMINATE
};

void doWork(void)
{
	PIN_setOutputValue(pinHandle, Board_LED1, 1);
    FakeBlockingSlowWork(); /* Pretend to do something useful but time-consuming */
	PIN_setOutputValue(pinHandle, Board_LED1, 0);
}

Void workTaskFunc(UArg arg0, UArg arg1)
{
    while (1) {

    	/* Do work */
    	doWork();

    	/* Wait a while, because doWork should be a periodic thing, not continuous.*/
    	CPUdelay(24e6);
    }
}

/*
 *  ======== main ========
 *
 */
int main(void)
{
    /* Call board init functions */
    PIN_init(BoardGpioInitTable);

    /* Open LED pins */
    pinHandle = PIN_open(&pinState, pinTable);
    if(!pinHandle) {
        System_abort("Error initializing board pins\n");
    }

    /* Set up the led task */
	Task_Params workTaskParams;
	Task_Params_init(&workTaskParams);
	workTaskParams.stackSize = 256;
	workTaskParams.priority = 2;
	workTaskParams.stack = &workTaskStack;

	Task_construct(&workTask, workTaskFunc, &workTaskParams, NULL);

    /* Start kernel. */
    BIOS_start();

    return (0);
}
