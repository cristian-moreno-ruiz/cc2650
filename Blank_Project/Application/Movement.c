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




// Task configuration
#define MV_TASK_PRIORITY                      1
#define MV_TASK_STACK_SIZE                    1024



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

PIN_Config pinTable[] = {
    Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_BUTTON0 | PIN_INPUT_EN | PIN_PULLUP,
	Board_BUTTON1 | PIN_INPUT_EN | PIN_PULLUP,
    PIN_TERMINATE
};

// Wake on motion threshold
#define WOM_THR                   10

static uint8_t mpuIntStatus;


// PIN handle and state
static PIN_Handle pinHandle;
static PIN_State pinState;


/*******************************************************************************
 * LOCAL FUNCTIONS
 */

#define FakeBlockingSlowWork()   CPUdelay(12e6)
static void Movement_taskFxn(UArg arg0, UArg arg1);
void blinkRedLed(void);
void blinkGreenLed(void);
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

    pinHandle = PIN_open(&pinState, pinTable);
    if(!pinHandle) {
        System_abort("Error initializing board pins\n");
    }
    // Test LEDS
    PIN_setOutputValue(pinHandle, Board_LED1, 1);
	Task_sleep(500 * (1000 / Clock_tickPeriod));


    bspI2cInit();

    if (sensorMpu9250Init()){
      //SensorTagMov_reset();
      sensorMpu9250RegisterCallback(motionInterrupt);
      PIN_setOutputValue(pinHandle, Board_LED2, 1);
      System_printf("Successful accelerometer Test\n");
      System_flush();
    } else{
    	System_printf("Unsuccessful accelerometer Test\n");
    	System_flush();
    	return;
    }


    if (sensorMpu9250Reset())
    {
      sensorMpu9250WomEnable(WOM_THR);
    }


    // Init process finished successfully
	Task_sleep(1000 * (1000 / Clock_tickPeriod));
	PIN_setOutputValue(pinHandle, Board_LED1, 0);
	PIN_setOutputValue(pinHandle, Board_LED2, 0);


}

void Movement_taskFxn(UArg arg0, UArg arg1){

	Movement_init();
	uint8_t accData[6];
	int16_t accel_x, accel_y, accel_z;
	uint8_t range;
	float accelx, accely, accelz;

	while(1){

		//FakeBlockingSlowWork();

		//Task_sleep(1000 * (1000 / Clock_tickPeriod));

		Semaphore_pend(motionSem, BIOS_WAIT_FOREVER);
		mpuIntStatus = sensorMpu9250IntStatus();
		System_printf("Interrupt Status: %u \n", mpuIntStatus);
		System_flush();
		if(mpuIntStatus & MPU_MOVEMENT){
			System_printf("Motion detected (Interrupt Status = 0x40)");
			System_flush();


			while(0){

				sensorMpu9250Init();
				sensorMpu9250Enable(MPU_AX_ACC);
				sensorMpu9250AccRead((uint16_t*) &accData);
				System_printf("Accel Raw: %u \n", accData);
				System_flush();

				range = sensorMpu9250AccReadRange();

				System_printf("Accel Range: %u\n", range);

				accel_x = (accData[0] << 8) + accData[1];
				accel_y = (accData[2] << 8) + accData[3];
				accel_z = (accData[4] << 8) + accData[5];

				accelx = sensorMpu9250AccConvert(accel_x);
				accely = sensorMpu9250AccConvert(accel_y);
				accelz = sensorMpu9250AccConvert(accel_z);
				System_printf("Accel data x: %1.2f\n", accelx);
				System_printf("Accel data y: %1.2f\n", accely);
				System_printf("Accel data z: %1.2f \n", accelz);
				System_flush();

				Task_sleep(5000 * (1000 / Clock_tickPeriod));

			}
			//if(){

			//}

			blinkRedLed();

		} else{
			blinkGreenLed();
		}

	}
}

void pinInterruptHandler(PIN_Handle handle, PIN_Id pinId){
	// Signal the semaphore (button has been pressed)
	Semaphore_post(motionSem);
}

void motionInterrupt(void){
	// Wake up the application thread
	//mpuDataRdy = true;
	//sensorReadScheduled = true;
	Semaphore_post(motionSem);
}

void blinkRedLed(void){
	PIN_setOutputValue(pinHandle, Board_LED1, 1);
	Task_sleep(500 * (1000 / Clock_tickPeriod));
	PIN_setOutputValue(pinHandle, Board_LED1, 0);
}

void blinkGreenLed(void){
	PIN_setOutputValue(pinHandle, Board_LED2, 1);
	Task_sleep(500 * (1000 / Clock_tickPeriod));
	PIN_setOutputValue(pinHandle, Board_LED2, 0);
}




