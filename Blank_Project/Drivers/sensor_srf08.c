// Made by: Cristian Moreno


/* -----------------------------------------------------------------------------
*                                          Includes
* ------------------------------------------------------------------------------
*/

#include "Board.h"
#include "sensor_srf08.h"
#include "sensor_opt3001.h" // For reset of I2C bus
#include "sensor.h"
#include "bsp_i2c.h"
#include <ti/sysbios/knl/Task.h>


/* -----------------------------------------------------------------------------
*                                           Constants
* ------------------------------------------------------------------------------
*/
// Sensor I2C address
#define SENSOR_I2C_ADDRESS_1			0x70
#define SENSOR_I2C_ADDRESS_2			0x70
#define SENSOR_I2C_ADDRESS_3			0x70


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

// Gain
#define MAX_GAIN						0x25

// Range
#define MAX_RANGE_6M					0x8C

// Sensor Selection/Deselection

#define SENSOR_SELECT_1()				bspI2cSelect(BSP_I2C_INTERFACE_0,SENSOR_I2C_ADDRESS_1)
#define SENSOR_SELECT_2()				bspI2cSelect(BSP_I2C_INTERFACE_0,SENSOR_I2C_ADDRESS_2)
#define SENSOR_SELECT_3()				bspI2cSelect(BSP_I2C_INTERFACE_0,SENSOR_I2C_ADDRESS_3)
#define SENSOR_DESELECT()				bspI2cDeselect()


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
bool sensorSrf08SetRange(uint8_t maxRange);
bool sensorSrf08SetMaxGain(uint8_t maxGain);

bool sensorSrf08SetRangeMultiple(uint8_t maxRange);
bool sensorSrf08SetMaxGainMultiple(uint8_t maxGain);


/* -----------------------------------------------------------------------------
*                           Local Variables
* ------------------------------------------------------------------------------
*/

static uint8_t range;

/* -----------------------------------------------------------------------------
*                            Functions
* ------------------------------------------------------------------------------
*/


/* -----------------------------------------------------------------------------
*                        SINGLE SENSOR USE
* ------------------------------------------------------------------------------
*/



bool sensorSrf08Init(void){

	if (!SENSOR_SELECT_1()){
		return false;
	}
	uint8_t data;

	bool revision = sensorReadReg(SW_REVISION, &data, 1);
	SENSOR_DESELECT();

	bool range = sensorSrf08SetRange(MAX_RANGE_6M);
	bool gain = sensorSrf08SetMaxGain(MAX_GAIN);

	return revision + range + gain;
}


bool sensorSrf08SetRange(uint8_t maxRange){
	if (!SENSOR_SELECT_1()){
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
	if (!SENSOR_SELECT_1()){
		return false;
	}
	// Set Maximum Analogue Gain
	bool success = sensorWriteReg(GAIN_REGISTER, &maxGain, 1);
	SENSOR_DESELECT();

	return success;
}


bool sensorSrf08Scan(uint16_t *data){

	// Send "Range Start" command
	if (!SENSOR_SELECT_1()){
		return 0;
	}
	uint8_t val = RANGE_CM;
	bool success = sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();

	if(!success) return 0;

	// Wait until Scan is finished (70 ms)
	Task_sleep(70 * (1000 / Clock_tickPeriod));

	// Read Echoes
	if (!SENSOR_SELECT_1()){
		return false;
	}
	success = sensorReadReg(FIRST_ECHO_H, (uint8_t*)data, DATA_SIZE);
	SENSOR_DESELECT();

	if(!success) return 0;

	return success;
}


int8_t sensorSrf08ConvertCm(uint8_t *data, uint16_t *cm){
	int8_t i, nEchoes = 0;
	for(i = 0; i < DATA_SIZE; i++){
		cm[i] = (data[2*i] << 8) + data[2*i+1];
		if(cm[i] > 0) nEchoes++;
	}
	return nEchoes;
}


/* -----------------------------------------------------------------------------
*                        TRIPLE SENSOR USE
* ------------------------------------------------------------------------------
*/



bool sensorSrf08InitMultiple(void){

	if (!SENSOR_SELECT_1()){
		return false;
	}
	uint8_t data;

	bool revision = sensorReadReg(SW_REVISION, &data, 1);
	SENSOR_DESELECT();

	bool range = sensorSrf08SetRangeMultiple(MAX_RANGE_6M);
	bool gain = sensorSrf08SetMaxGainMultiple(MAX_GAIN);

	return revision + range + gain;
}


bool sensorSrf08SetRangeMultiple(uint8_t maxRange){
	// SENSOR 1
	if (!SENSOR_SELECT_1()){
		return false;
	}
	// Set Maximum Range
	bool success_1 = sensorWriteReg(RANGE_REGISTER, &maxRange, 1);
	SENSOR_DESELECT();

	// SENSOR 2
	if (!SENSOR_SELECT_2()){
		return false;
	}
	// Set Maximum Range
	bool success_2 = sensorWriteReg(RANGE_REGISTER, &maxRange, 1);
	SENSOR_DESELECT();

	// SENSOR 3
	if (!SENSOR_SELECT_3()){
		return false;
	}
	// Set Maximum Range
	bool success_3 = sensorWriteReg(RANGE_REGISTER, &maxRange, 1);
	SENSOR_DESELECT();

	return success_1 + success_2 + success_3;
}


bool sensorSrf08SetMaxGainMultiple(uint8_t maxGain){
	// SENSOR 1
	if (!SENSOR_SELECT_1()){
		return false;
	}
	// Set Maximum Analogue Gain
	bool success_1 = sensorWriteReg(GAIN_REGISTER, &maxGain, 1);
	SENSOR_DESELECT();

	// SENSOR 2
	if (!SENSOR_SELECT_2()){
		return false;
	}
	// Set Maximum Analogue Gain
	bool success_2 = sensorWriteReg(GAIN_REGISTER, &maxGain, 1);
	SENSOR_DESELECT();

	// SENSOR 3
	if (!SENSOR_SELECT_3()){
		return false;
	}
	// Set Maximum Analogue Gain
	bool success_3 = sensorWriteReg(GAIN_REGISTER, &maxGain, 1);
	SENSOR_DESELECT();

	return success_1 + success_2 + success_3;
}


bool sensorSrf08ScanMultiple(uint16_t *data_1, uint16_t *data_2, uint16_t *data_3){

	bool success;
	uint8_t val = RANGE_CM;

	// Send "Range Start" command to SENSOR 1
	if (!SENSOR_SELECT_1()) return 0;
	success = sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();
	if(!success) return false;

	// Send "Range Start" command to SENSOR 2
	if (!SENSOR_SELECT_2()) return 0;
	success = sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();
	if(!success) return false;

	// Send "Range Start" command to SENSOR 3
	if (!SENSOR_SELECT_3()) return 0;
	success = sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();
	if(!success) return false;

	// Wait until Scan is finished (70 ms)
	Task_sleep(70 * (1000 / Clock_tickPeriod));

	// Read Echoes received in SENSOR 1
	if (!SENSOR_SELECT_1()) return 0;
	success = sensorReadReg(FIRST_ECHO_H, (uint8_t*)data_1, DATA_SIZE);
	SENSOR_DESELECT();
	if(!success) return false;

	// Read Echoes received in SENSOR 2
	if (!SENSOR_SELECT_1()) return 0;
	success = sensorReadReg(FIRST_ECHO_H, (uint8_t*)data_2, DATA_SIZE);
	SENSOR_DESELECT();
	if(!success) return false;

	// Read Echoes received in SENSOR 3
	if (!SENSOR_SELECT_1()) return 0;
	success = sensorReadReg(FIRST_ECHO_H, (uint8_t*)data_3, DATA_SIZE);
	SENSOR_DESELECT();
	if(!success) return false;

	return true;
}

