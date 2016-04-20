// Made by: Cristian Moreno


/* -----------------------------------------------------------------------------
*                                          Includes
* ------------------------------------------------------------------------------
*/

#include "Board.h"
#include "sensor_mpu9250.h"
#include "sensor_opt3001.h" // For reset of I2C bus
#include "sensor.h"
#include "bsp_i2c.h"
#include <ti/sysbios/knl/Task.h>


/* -----------------------------------------------------------------------------
*                                           Constants
* ------------------------------------------------------------------------------
*/
// Sensor I2C address
#define SENSOR_I2C_ADDRESS				0xE0


// Write Registers
#define GAIN_REGISTER 					0x01
#define RANGE_REGISTER					0x02
#define COMMAND_REGISTER				0x00

// Data Size to read
#define DATA_SIZE						34

// Read Registers
#define SW_REVISION						0x00
#define FIRST_ECHO_H					0x02

// Commands
#define RANGE_CM						0x51


// Sensor Selection/Deselection

#define SENSOR_SELECT()               bspI2cSelect(BSP_I2C_INTERFACE_1,SENSOR_I2C_ADDRESS)
#define SENSOR_DESELECT()             bspI2cDeselect()

/* -----------------------------------------------------------------------------
*                           Typedefs
* ------------------------------------------------------------------------------
*/

/* -----------------------------------------------------------------------------
*                           Macros
* ------------------------------------------------------------------------------
*/

/* -----------------------------------------------------------------------------
*                           Local Functions
* ------------------------------------------------------------------------------
*/

void sensorSrf08PowerOn(void);

/* -----------------------------------------------------------------------------
*                           Local Variables
* ------------------------------------------------------------------------------
*/

static uint8_t range;

/* -----------------------------------------------------------------------------
*                            Functions
* ------------------------------------------------------------------------------
*/

bool sensorSrf08Init(void){

	if (!SENSOR_SELECT()){
		return false;
	}

	uint8_t data;

	bool success = sensorReadReg(SW_REVISION, &data, 1);
	SENSOR_DESELECT();


	return true;
}




bool sensorSrf08SetRange(uint8_t maxRange){
	if (!SENSOR_SELECT()){
		return false;
	}
	// Set Maximum Range
	bool success = sensorWriteReg(RANGE_REGISTER, &maxRange, 1);
	SENSOR_DESELECT();

	//Store range in variable
	range = maxRange;

	return success;
}


bool sensorSrf08SetMaxGain(uint8_t maxGain){
	if (!SENSOR_SELECT()){
		return false;
	}
	// Set Maximum Analogue Gain
	bool success = sensorWriteReg(GAIN_REGISTER, &maxGain, 1);
	SENSOR_DESELECT();

	return success;
}

uint8_t sensorSrf08Scan(uint16_t *data){
	uint8_t nEchoes = 0;

	// Send "Range Start" command
	if (!SENSOR_SELECT()){
		return 0;
	}
	uint8_t val = RANGE_CM;
	bool success = sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();

	if(!success) return 0;

	// Wait until Scan is finished (70 ms)
	Task_sleep(70 * (1000 / Clock_tickPeriod));

	// Read Echoes
	if (!SENSOR_SELECT()){
		return false;
	}
	success = sensorReadReg(FIRST_ECHO_H, (uint8_t*)data, DATA_SIZE);
	SENSOR_DESELECT();

	if(!success) return 0;


	return nEchoes;
}


