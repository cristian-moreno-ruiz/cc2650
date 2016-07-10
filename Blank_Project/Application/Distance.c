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

// Task configuration (Priority and stack size)
#define DST_TASK_PRIORITY                      1
#define DST_TASK_STACK_SIZE                    2048

#define SLEEP_MS		500


/*******************************************************************************
 * LOCAL VARIABLES
 */

Task_Struct distanceTask;
static uint8_t distanceTaskStack[DST_TASK_STACK_SIZE];

bool multipleMode;


/*******************************************************************************
 * LOCAL FUNCTIONS
 */

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
	// Ensure resources are available
	Task_sleep(100 * 1000 / Clock_tickPeriod);

    // INIT SRF08/SRF02s
    if(sensorSrf08InitMultiple()){
    	System_printf("Selected mode: Multiple sensors (3) \n");
    	multipleMode = true;
    }else if(sensorSrf08Init()){
    	System_printf("Selected mode: Single sensor \n");
    	multipleMode = false;
    }else{
    	System_abort("Error initializing sensors\n");
    }
	System_flush();
}

void Distance_taskFxn(UArg arg0, UArg arg1){

	Distance_init();

	// Set sequential or distributed mode for multiple sensors
	bool distributed = false;

	if(multipleMode){
		// Initialize variables for multiple sensors
		uint8_t srf08Data_1[34], srf08Data_2[34], srf08Data_3[34];
		uint16_t cm_1[17], cm_2[17], cm_3[17];

		while(1){
			if(distributed){
				// Scan in multiple distributed mode
				sensorSrf02ScanDistributed((uint16_t*) &srf08Data_1, (uint16_t*) &srf08Data_2, (uint16_t*) &srf08Data_3);
				sensorSrf08ConvertCm((uint8_t*) &srf08Data_1, (uint16_t*) &cm_1);
				sensorSrf08ConvertCm((uint8_t*) &srf08Data_2, (uint16_t*) &cm_2);
				sensorSrf08ConvertCm((uint8_t*) &srf08Data_3, (uint16_t*) &cm_3);

				// Print results
				System_printf("[Sensor 1] Pulse sent (Distributed Mode)\n");
				System_printf("[Sensor 1] First Echo found: %u  cm\n", cm_1[0]);
				System_printf("[Sensor 2] First Echo found: %u  cm\n", cm_2[0]);
				System_printf("[Sensor 3] First Echo found: %u  cm\n\n", cm_3[0]);
				System_flush();
			}else{
				// Scan in multiple sequential mode
				sensorSrf08ScanMultiple((uint16_t*) &srf08Data_1, (uint16_t*) &srf08Data_2, (uint16_t*) &srf08Data_3);
				sensorSrf08ConvertCm((uint8_t*) &srf08Data_1, (uint16_t*) &cm_1);
				sensorSrf08ConvertCm((uint8_t*) &srf08Data_2, (uint16_t*) &cm_2);
				sensorSrf08ConvertCm((uint8_t*) &srf08Data_3, (uint16_t*) &cm_3);

				// Print results
				System_printf("[Sensor 1] First Echo found: %u  cm\n", cm_1[0]);
				System_printf("[Sensor 2] First Echo found: %u  cm\n", cm_2[0]);
				System_printf("[Sensor 3] First Echo found: %u  cm\n\n", cm_3[0]);
				System_flush();

			}
			Task_sleep(SLEEP_MS * (1000 / Clock_tickPeriod));
		}
	}else{
		// Varialbes initialization for single mode
		uint8_t srf08Data[34];
		uint16_t cm[17];

		while(1){
			// Scan in single mode
	    	sensorSrf08Scan((uint16_t*) &srf08Data);
	    	sensorSrf08ConvertCm((uint8_t*) &srf08Data, (uint16_t*) &cm);

	    	// Print results
	    	System_printf("First Echo found: %u  cm\n", cm[0]);
	    	System_flush();

	    	Task_sleep(SLEEP_MS * (1000 / Clock_tickPeriod));
		}
	}
}
