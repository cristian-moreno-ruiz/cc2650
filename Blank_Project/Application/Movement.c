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
#include "Drivers/bsp_i2c.h"

/*******************************************************************************
 * CONSTANTS
 */

// Task configuration (Priority and stack size)
#define MV_TASK_PRIORITY                      2
#define MV_TASK_STACK_SIZE                    1024

// Sampling behaviour when accelerometer is active
#define INACTIVE_COUNT		5		// # of inactive periods to go idle
#define SAMPLE_PERIOD		500		// Sampling period when active

/*******************************************************************************
 * LOCAL VARIABLES
 */

Task_Struct movementTask;
static uint8_t movementTaskStack[MV_TASK_STACK_SIZE];

// Pin config of Movement task, only using LED0 (Green)
PIN_Config pinTable[] = {
    Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

// Wake on motion threshold (in mg)
#define WOM_THR                   10

// Interrupt status
static uint8_t mpuIntStatus;

// PIN handle and state
static PIN_Handle pinHandle;
static PIN_State pinState;

/*******************************************************************************
 * LOCAL FUNCTIONS
 */

static void Movement_taskFxn(UArg arg0, UArg arg1);
void pinInterruptHandler(PIN_Handle handle, PIN_Id pinId);
void motionInterrupt(void);

/*******************************************************************************
 * PUBLIC FUNCTIONS
 */

void Movement_createTask(void){

    /* Set up the movement task */
	Task_Params movementTaskParams;

	Task_Params_init(&movementTaskParams);
	movementTaskParams.stackSize = MV_TASK_STACK_SIZE;
	movementTaskParams.priority = MV_TASK_PRIORITY;
	movementTaskParams.stack = movementTaskStack;

	Task_construct(&movementTask, Movement_taskFxn, &movementTaskParams, NULL);
}

void Movement_init(void){

	System_flush();
	// Initialize Board PINs
    pinHandle = PIN_open(&pinState, pinTable);
    if(!pinHandle) {
        System_abort("Error initializing board pins\n");
    }

    // Initialize accelerometer
    if (sensorMpu9250Init()){
      //SensorTagMov_reset();
      sensorMpu9250RegisterCallback(motionInterrupt);
      PIN_setOutputValue(pinHandle, Board_LED0, 1);
      System_printf("Successful accelerometer Test\n");
      System_flush();
    } else{
    	System_printf("Unsuccessful accelerometer Test\n");
    	System_flush();
    	return;
    }

    // Set accelerometer in low power mode and WOM
    if (sensorMpu9250Reset()){
      sensorMpu9250WomEnable(WOM_THR);
    }

    // Set WOM callback function
    mpuIntStatus = sensorMpu9250IntStatus();

    // Init process finished successfully
	Task_sleep(500 * (1000 / Clock_tickPeriod));
	PIN_setOutputValue(pinHandle, Board_LED0, 0);
}

void Movement_taskFxn(UArg arg0, UArg arg1){

	Movement_init();

	// Acceleration variables declaration
	uint8_t accData[6];
	int16_t accel_x, accel_y, accel_z;
	float accelx, accely, accelz;

	// Counter of periods without detecting activity
	int inactive;

	while(1){

		// Wait for movement detected (When starting the program, an interrupt is thrown once so it posts the semaphore)
		Semaphore_pend(motionSem, BIOS_WAIT_FOREVER);

		// Check which type of interrupt is triggered (WOM or DataReady)
		mpuIntStatus = sensorMpu9250IntStatus();

		if((mpuIntStatus & MPU_MOVEMENT) ){						// WOM interrupt
			inactive = 0;										// Start inactive counter
			sensorMpu9250SwitchInterruptMode(FALSE,WOM_THR);	// Enable Data ready interrupt
			PIN_setOutputValue(pinHandle, Board_LED0, 1);		// Switch green LED on
		} else {//if(wom){										// Data Ready interrupt

			// Read and acomodate accelerometer raw data
			sensorMpu9250AccRead((uint16_t*) &accData);
			accel_x = (((int16_t)accData[1]) << 8) | accData[0];
			accel_y = (((int16_t)accData[3]) << 8) | accData[2];
			accel_z = (((int16_t)accData[5]) << 8) | accData[4];

			// Convert accelerometer raw data to acceleration units (g)
			accelx = sensorMpu9250AccConvert(accel_x);
			accely = sensorMpu9250AccConvert(accel_y);
			accelz = sensorMpu9250AccConvert(accel_z);

			// Print measured acceleration
			System_printf("Accel data x: %1.2f\n", accelx);
			System_printf("Accel data y: %1.2f\n", accely);
			System_printf("Accel data z: %1.2f \n", accelz);
			System_flush();

			// If the inactivity timeout expires, enable WOM and low power mode
			if (inactive == INACTIVE_COUNT){
				sensorMpu9250SwitchInterruptMode(TRUE,WOM_THR);

				// Ignore first interrupt, produced just after activating WOM and low power mode
				Task_sleep(250 * (1000 / Clock_tickPeriod));
				Semaphore_pend(motionSem, BIOS_WAIT_FOREVER);
				mpuIntStatus = sensorMpu9250IntStatus();
				PIN_setOutputValue(pinHandle, Board_LED0, 0);	//Switch green LED off
			}

			// Increase inactive timer
			inactive++;
			Task_sleep(SAMPLE_PERIOD * (1000 / Clock_tickPeriod));
		}
	}
}

void motionInterrupt(void){
	// Post semaphore when interrupt is thrown by the accelerometer
	Semaphore_post(motionSem);
}
