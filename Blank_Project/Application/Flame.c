/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Queue.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

/* Example/Board Header files */
#include "Board.h"

/* Driverlib CPU functions, used here for CPUdelay*/
#include <driverlib/cpu.h>

// Drivers for motion sensor
//#include "Board.h"
//#include "movementservice.h"
//#include "Drivers/util.h"
#include "string.h"




/*******************************************************************************
 * CONSTANTS
 */




// Task configuration
#define FLM_TASK_PRIORITY                      1
#define FLM_TASK_STACK_SIZE                    2048



/*******************************************************************************
 * TYPEDEFS
 */




/*******************************************************************************
 * GLOBAL VARIABLES
 */




/*******************************************************************************
 * LOCAL VARIABLES
 */

Task_Struct flameTask;
static uint8_t flameTaskStack[FLM_TASK_STACK_SIZE];

PIN_Config pinTableFlame[] = {
    Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_BUTTON0 | PIN_INPUT_EN | PIN_PULLUP,
	Board_BUTTON1 | PIN_INPUT_EN | PIN_PULLUP,
    PIN_TERMINATE
};


// PIN handle and state
static PIN_Handle pinHandle;
static PIN_State pinState;


/*******************************************************************************
 * LOCAL FUNCTIONS
 */

static void Flame_taskFxn(UArg arg0, UArg arg1);




/*******************************************************************************
 * PUBLIC FUNCTIONS
 */



void Flame_createTask(void){

    /* Set up the flame detection task */
	Task_Params flameTaskParams;

	Task_Params_init(&flameTaskParams);
	flameTaskParams.stackSize = FLM_TASK_STACK_SIZE;
	flameTaskParams.priority = FLM_TASK_PRIORITY;
	flameTaskParams.stack = flameTaskStack;

	Task_construct(&flameTask, Flame_taskFxn, &flameTaskParams, NULL);
}

void Flame_init(void){

	System_flush();

    pinHandle = PIN_open(&pinState, pinTableFlame);
    if(!pinHandle) {
        System_abort("Error initializing board pins\n");
    }
    // Test LEDS
    PIN_setOutputValue(pinHandle, Board_LED1, 1);
	Task_sleep(500 * (1000 / Clock_tickPeriod));




    PIN_setOutputValue(pinHandle, Board_LED2, 1);

    // Init process finished successfully
	Task_sleep(1000 * (1000 / Clock_tickPeriod));
	PIN_setOutputValue(pinHandle, Board_LED1, 0);
	PIN_setOutputValue(pinHandle, Board_LED2, 0);

}

void Flame_taskFxn(UArg arg0, UArg arg1){

	Flame_init();

}






