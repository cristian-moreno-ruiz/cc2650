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
#include "Board.h"
//#include "movementservice.h"
#include "Drivers/sensor_mpu9250.h"
#include "Drivers/sensor.h"
//#include "Drivers/util.h"
#include "string.h"



/*******************************************************************************
 * CONSTANTS
 */




// Task configuration
#define MV_TASK_PRIORITY                      1
#define MV_TASK_STACK_SIZE                    256



/*******************************************************************************
 * TYPEDEFS
 */




/*******************************************************************************
 * GLOBAL VARIABLES
 */




/*******************************************************************************
 * LOCAL VARIABLES
 */

Task_Struct movementTask;
static uint8_t movementTaskStack[MV_TASK_STACK_SIZE];


/*******************************************************************************
 * LOCAL FUNCTIONS
 */

#define FakeBlockingSlowWork()   CPUdelay(12e6)




/*******************************************************************************
 * PUBLIC FUNCTIONS
 */
void Movement_taskFxn(UArg arg0, UArg arg1){
	while(1){

		//FakeBlockingSlowWork();

		Task_sleep(1000 * (1000 / Clock_tickPeriod));

	}
}


void Movement_createTask(void){

    /* Set up the movement task */
	Task_Params movementTaskParams;

	Task_Params_init(&movementTaskParams);
	movementTaskParams.stackSize = MV_TASK_STACK_SIZE;
	movementTaskParams.priority = MV_TASK_PRIORITY;
	movementTaskParams.stack = movementTaskStack;

	Task_construct(&movementTask, Movement_taskFxn, &movementTaskParams, NULL);
}



