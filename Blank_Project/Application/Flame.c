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

// ADC files
#include "driverlib/aux_adc.h"
#include "driverlib/aux_wuc.h"
//#include <inc/hw_aux_evctl.h>

/* Example/Board Header files */
#include "Board.h"

/* Driverlib CPU functions, used here for CPUdelay*/
#include <driverlib/cpu.h>

#include "Drivers/sensor_tmp007.h"
#include "Drivers/sensor_opt3001.h"
#include "Drivers/sensor.h"
#include "Drivers/bsp_i2c.h"

// Drivers for motion sensor
//#include "Board.h"
//#include "movementservice.h"
//#include "Drivers/util.h"
#include "string.h"




/*******************************************************************************
 * CONSTANTS
 */


#define ALS_OUTPUT IOID_23

// Task configuration
#define FLM_TASK_PRIORITY                      3
#define FLM_TASK_STACK_SIZE                    2048

// ADC Samples

#define SAMPLECOUNT 8
#define SAMPLETYPE uint16_t
#define SAMPLESIZE sizeof(SAMPLETYPE)

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
    /*Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_BUTTON0 | PIN_INPUT_EN | PIN_PULLUP,
	Board_BUTTON1 | PIN_INPUT_EN | PIN_PULLUP,*/
	ALS_OUTPUT   | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS,
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

	System_flush();

	// Standalone TAsk:
	bspI2cInit();

    pinHandle = PIN_open(&pinState, pinTableFlame);
    if(!pinHandle) {
        System_abort("Error initializing board pins\n");
    }
    // Test LEDS
    /*PIN_setOutputValue(pinHandle, Board_LED1, 1);
	Task_sleep(500 * (1000 / Clock_tickPeriod));*/

	// Set up ADC Config
	AUXWUCClockEnable(AUX_WUC_MODCLKEN0_ANAIF_M|AUX_WUC_MODCLKEN0_AUX_ADI4_M);
	AUXADCSelectInput(ADC_COMPB_IN_AUXIO7);
	AUXADCEnableSync(AUXADC_REF_FIXED, AUXADC_SAMPLE_TIME_2P7_US, AUXADC_TRIGGER_MANUAL);

	// Set interrupt in PIN Board_DP1
    PIN_registerIntCb(pinHandle, flameInterruptHandler);
    PIN_setInterrupt(pinHandle, Board_DP1 | PIN_IRQ_POSEDGE);

    //Tmp sensor
    sensorTmp007Init();

    //Opt sensor
    sensorOpt3001Init();
    sensorOpt3001Enable(false);

	// Disallow STANDBY mode while using the ADC.
	// Power_setConstraint(Power_SB_DISALLOW);

    /*PIN_setOutputValue(pinHandle, Board_LED2, 1);

    // Init process finished successfully
	Task_sleep(1000 * (1000 / Clock_tickPeriod));
	PIN_setOutputValue(pinHandle, Board_LED1, 0);
	PIN_setOutputValue(pinHandle, Board_LED2, 0);*/

}

void Flame_taskFxn(UArg arg0, UArg arg1){

	Flame_init();

	//SAMPLETYPE adcSamples[SAMPLECOUNT];
	SAMPLETYPE singleSample;
	//uint8_t currentSample = 0;

	// Tmp variables
	uint16_t tempTarget, tempLocal;
	float tTarget, tLocal;

	//Opt variables
	uint16_t opticalData;
	float lux;

	while(1){

		// Wait until flame is detected
		//Semaphore_pend(flameSem, BIOS_WAIT_FOREVER);

		//Sleep 100ms in IDLE mode
		Task_sleep(100 * 1000 / Clock_tickPeriod);

		// Trigger ADC sampling
		AUXADCGenManualTrigger();

		Task_sleep(100 * 1000 / Clock_tickPeriod);

		singleSample = AUXADCReadFifo();
		// Clear ADC_IRQ flag. Note: Missing driver for this.
		//HWREGBITW(AUX_EVCTL_BASE + AUX_EVCTL_O_EVTOMCUFLAGSCLR, AUX_EVCTL_EVTOMCUFLAGSCLR_ADC_IRQ_BITN) = 1;


		System_printf("%d mv on ADC\r\n",singleSample);

		// Temperature
	    sensorTmp007Enable(true);
	    Task_sleep(275 * 1000 / Clock_tickPeriod);
	    sensorTmp007Read(&tempLocal, &tempTarget);
	    sensorTmp007Enable(false);
	    sensorTmp007Convert(tempLocal, tempTarget, &tTarget, &tLocal);

	    System_printf("%f Local tmp\r\n",tLocal);
	    System_printf("%f Target tmp\r\n",tTarget);

	    // Ambient light
	    sensorOpt3001Enable(true);
	    Task_sleep(275 * 1000 / Clock_tickPeriod);
	    sensorOpt3001Read(&opticalData);
	    sensorOpt3001Enable(false);
	    lux = sensorOpt3001Convert(opticalData);

	    System_printf("%f Optical data\r\n",lux);
		System_flush();
	}
}

void flameInterruptHandler(PIN_Handle handle, PIN_Id pinId){
	Semaphore_post(flameSem);
}




