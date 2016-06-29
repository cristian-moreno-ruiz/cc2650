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

#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

// ADC files
#include "driverlib/aux_adc.h"
#include "driverlib/aux_wuc.h"
//#include <inc/hw_aux_evctl.h>

/* Example/Board Header files */
#include "Board.h"

/* Driverlib CPU functions, used here for CPUdelay*/
#include <driverlib/cpu.h>

// Drivers to use temperature and ambient light sensors
#include "Drivers/sensor_tmp007.h"
#include "Drivers/sensor_opt3001.h"
#include "Drivers/sensor.h"
#include "Drivers/bsp_i2c.h"

#include "string.h"

/*******************************************************************************
 * CONSTANTS
 */

// Task configuration (Priority and stack size)
#define FLM_TASK_PRIORITY                      3
#define FLM_TASK_STACK_SIZE                    1024

// ADC Sample type definition
#define SAMPLETYPE uint16_t
#define SAMPLESIZE sizeof(SAMPLETYPE)

/*******************************************************************************
 * LOCAL VARIABLES
 */

Task_Struct flameTask;
static uint8_t flameTaskStack[FLM_TASK_STACK_SIZE];

// PIN config of flame task, with LED1 (Red), Board_DP1, and Board_DP2
PIN_Config pinTableFlame[] = {
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_DP2   | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS,
	Board_DP1	 | PIN_INPUT_EN,
    PIN_TERMINATE
};

// PIN handle and state
static PIN_Handle pinHandle;
static PIN_State pinState;

/*******************************************************************************
 * LOCAL FUNCTIONS
 */

static void Flame_taskFxn(UArg arg0, UArg arg1);
void flameInterruptHandler(PIN_Handle handle, PIN_Id pinId);

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

	// Task_sleep(5000 * 1000 / Clock_tickPeriod);
	// Clear console
	System_flush();

	// Initialize I2C bus (this is done here because is the highest priority task, and will be executed first)
	bspI2cInit();

	// Initialize Board PINs
    pinHandle = PIN_open(&pinState, pinTableFlame);
    if(!pinHandle) {
        System_abort("Error initializing board pins\n");
    }

    // Test Red LED
    PIN_setOutputValue(pinHandle, Board_LED1, 1);
	Task_sleep(500 * (1000 / Clock_tickPeriod));

	// Set up ADC Config in PIN Board_DP2
	AUXWUCClockEnable(AUX_WUC_MODCLKEN0_ANAIF_M|AUX_WUC_MODCLKEN0_AUX_ADI4_M);
	AUXADCSelectInput(ADC_COMPB_IN_AUXIO7);
	AUXADCEnableSync(AUXADC_REF_FIXED, AUXADC_SAMPLE_TIME_2P7_US, AUXADC_TRIGGER_MANUAL);

	// Set interrupt in PIN Board_DP1
    PIN_registerIntCb(pinHandle, flameInterruptHandler);
    PIN_setInterrupt(pinHandle, Board_DP1 | PIN_IRQ_POSEDGE);

    // Initialize Temperature sensor
    sensorTmp007Init();

    // Initialize Ambient light sensor
    sensorOpt3001Init();
    sensorOpt3001Enable(false);

	// Disallow STANDBY mode to use the ADC (Otherwise, it crashes when not debugging)
	Power_setConstraint(Power_SB_DISALLOW);

    // Init process finished successfully
    Task_sleep(500 * (1000 / Clock_tickPeriod));
    PIN_setOutputValue(pinHandle, Board_LED1, 0);
}

void Flame_taskFxn(UArg arg0, UArg arg1){

	Flame_init();

	// ADC Sample variable declaration
	SAMPLETYPE singleSample;

	// Temperature variables declaration
	uint16_t tempTarget, tempLocal;
	float tTarget, tLocal;

	// Ambient light variables declaration
	uint16_t opticalData;
	float lux;

	while(1){

		// Wait until flame detector throws an interrupt, only if flame detector output is low (otherwise continue)
		if(PIN_getInputValue(PIN_ID(Board_DP1)) == 0) Semaphore_pend(flameSem, BIOS_WAIT_FOREVER);

		// Sleep 100ms in IDLE mode
		Task_sleep(100 * 1000 / Clock_tickPeriod);

		// Trigger ADC sampling and collect sample value
		AUXADCGenManualTrigger();
		Task_sleep(100 * 1000 / Clock_tickPeriod);
		singleSample = AUXADCReadFifo();
		System_printf("%d mv on ADC\r\n",singleSample);

		// Get data from Temperature sensor
	    sensorTmp007Enable(true);
	    Task_sleep(275 * 1000 / Clock_tickPeriod);
	    sensorTmp007Read(&tempLocal, &tempTarget);
	    sensorTmp007Enable(false);
	    sensorTmp007Convert(tempLocal, tempTarget, &tTarget, &tLocal);

	    // Print value in debug console
	    System_printf("%f Local tmp\r\n",tLocal);

	    // Get data from Ambient light sensor
	    sensorOpt3001Enable(true);
	    Task_sleep(275 * 1000 / Clock_tickPeriod);
	    sensorOpt3001Read(&opticalData);
	    sensorOpt3001Enable(false);
	    lux = sensorOpt3001Convert(opticalData);

	    // Print value in debug console
	    System_printf("%f Optical data\r\n",lux);
		System_flush();

		// Blink Red LED
		PIN_setOutputValue(pinHandle, Board_LED1, 1);
		Task_sleep(500 * (1000 / Clock_tickPeriod));
		PIN_setOutputValue(pinHandle, Board_LED1, 0);
	}
}

void flameInterruptHandler(PIN_Handle handle, PIN_Id pinId){
	// Post semaphore when interrupt is thrown by the flame detector
	Semaphore_post(flameSem);
}




