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
#include "Drivers/sensor_srf08.h"
#include "Drivers/sensor.h"
//#include "Drivers/util.h"
#include "string.h"
#include "Drivers/bsp_i2c.h"




/*******************************************************************************
 * CONSTANTS
 */




// Task configuration
#define DST_TASK_PRIORITY                      1
#define DST_TASK_STACK_SIZE                    2048



/*******************************************************************************
 * TYPEDEFS
 */




/*******************************************************************************
 * GLOBAL VARIABLES
 */




/*******************************************************************************
 * LOCAL VARIABLES
 */

Task_Struct distanceTask;
static uint8_t distanceTaskStack[DST_TASK_STACK_SIZE];

PIN_Config pinTableDistance[] = {
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

#define FakeBlockingSlowWork()   CPUdelay(12e6)
static void Distance_taskFxn(UArg arg0, UArg arg1);
void blinkRedLedDistance(void);
void blinkGreenLedDistance(void);




/*******************************************************************************
 * PUBLIC FUNCTIONS
 */



void Distance_createTask(void){

    /* Set up the movement task */
	Task_Params distanceTaskParams;

	Task_Params_init(&distanceTaskParams);
	distanceTaskParams.stackSize = DST_TASK_STACK_SIZE;
	distanceTaskParams.priority = DST_TASK_PRIORITY;
	distanceTaskParams.stack = distanceTaskStack;

	Task_construct(&distanceTask, Distance_taskFxn, &distanceTaskParams, NULL);
}

void Distance_init(void){

	System_flush();

    pinHandle = PIN_open(&pinState, pinTableDistance);
    if(!pinHandle) {
        System_abort("Error initializing board pins\n");
    }
    // Test LEDS
    PIN_setOutputValue(pinHandle, Board_LED1, 1);
	Task_sleep(500 * (1000 / Clock_tickPeriod));


    bspI2cInit();

    // INIT SRF08

    bool success = sensorSrf08Init();

    uint8_t srf08Data[34];

    if(!success){
    	bool srf08, accel;
    	while(1){
    		srf08 = sensorSrf08Init();
    		accel = sensorMpu9250Init();
    		sensorSrf08Scan((uint16_t*) &srf08Data);
    		//blinkGreenLedDistance();
    		//blinkRedLedDistance();
    	}
    }



    // Init process finished successfully
	Task_sleep(1000 * (1000 / Clock_tickPeriod));
	PIN_setOutputValue(pinHandle, Board_LED1, 0);
	PIN_setOutputValue(pinHandle, Board_LED2, 0);


}

void Distance_taskFxn(UArg arg0, UArg arg1){

	while(1){
		Distance_init();
	}

}



void blinkRedLedDistance(void){
	PIN_setOutputValue(pinHandle, Board_LED1, 1);
	Task_sleep(500 * (1000 / Clock_tickPeriod));
	PIN_setOutputValue(pinHandle, Board_LED1, 0);
}

void blinkGreenLedDistance(void){
	PIN_setOutputValue(pinHandle, Board_LED2, 1);
	Task_sleep(500 * (1000 / Clock_tickPeriod));
	PIN_setOutputValue(pinHandle, Board_LED2, 0);
}




