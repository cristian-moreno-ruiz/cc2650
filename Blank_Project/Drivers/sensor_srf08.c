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
#define SENSOR_I2C_ADDRESS_2			0x71
#define SENSOR_I2C_ADDRESS_3			0x72

// Change Address Sequence
#define CHANGE_SEQ_1					0xA0
#define CHANGE_SEQ_2					0xAA
#define CHANGE_SEQ_3					0xA5

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
#define SCAN_CM							0x51

// Gain
#define MAX_GAIN						0x25

// Range
#define MAX_RANGE_6M					0x8C

// SRF02 Commands
#define TRANSMIT_BURST					0x5C
#define FAKE_SCAN_CM					0x57

// Sensors Selection/Deselection

#define SENSOR_SELECT_1()				bspI2cSelect(BSP_I2C_INTERFACE_0,SENSOR_I2C_ADDRESS_1)
#define SENSOR_SELECT_2()				bspI2cSelect(BSP_I2C_INTERFACE_0,SENSOR_I2C_ADDRESS_2)
#define SENSOR_SELECT_3()				bspI2cSelect(BSP_I2C_INTERFACE_0,SENSOR_I2C_ADDRESS_3)
#define SENSOR_DESELECT()				bspI2cDeselect()


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
*                        SINGLE SENSOR USE
* ------------------------------------------------------------------------------
*/

// Change address of a sensor (only one sensors can be connected to the bus)
bool sensorSrf08ChangeAddress(uint8_t oldAddress, uint8_t newAddress){

	uint8_t val;

	// Cast 7-bit address to 8-bit address
	newAddress = (newAddress << 1);

	// Write Change sequence to old address
	if(!bspI2cSelect(BSP_I2C_INTERFACE_0, oldAddress)) return false;
	val = CHANGE_SEQ_1;
	sensorWriteReg(COMMAND_REGISTER, &val, 1);

	val = CHANGE_SEQ_2;
	sensorWriteReg(COMMAND_REGISTER, &val, 1);

	val = CHANGE_SEQ_3;
	sensorWriteReg(COMMAND_REGISTER, &val, 1);

	// Write new address
	val = newAddress;
	bool success = sensorWriteReg(COMMAND_REGISTER, &val, 1);

	bspI2cDeselect();
	if(!success) return false;

	return true;
}

// Initialization of one only sensor: Check connectivity and set range and gain to maximum
bool sensorSrf08Init(void){

	if (!SENSOR_SELECT_1()){
		return false;
	}
	uint8_t data;

	bool success = sensorReadReg(SW_REVISION, &data, 1);
	SENSOR_DESELECT();

	success *= sensorSrf08SetRange(MAX_RANGE_6M);
	success *= sensorSrf08SetMaxGain(MAX_GAIN);

	return success;
}

// Set custom value of the range register
bool sensorSrf08SetRange(uint8_t maxRange){
	if (!SENSOR_SELECT_1()){
		return false;
	}
	// Set Maximum Range
	bool success = sensorWriteReg(RANGE_REGISTER, &maxRange, 1);
	SENSOR_DESELECT();

	//Store range in variable

	return success;
}

// Set custom value of the max gain register
bool sensorSrf08SetMaxGain(uint8_t maxGain){
	if (!SENSOR_SELECT_1()){
		return false;
	}
	// Set Maximum Analogue Gain
	bool success = sensorWriteReg(GAIN_REGISTER, &maxGain, 1);
	SENSOR_DESELECT();

	return success;
}

// Send Scan command and read data obtained after 70 ms
bool sensorSrf08Scan(uint16_t *data){

	// Send "Range Start" command
	if (!SENSOR_SELECT_1()){
		return 0;
	}
	uint8_t val = SCAN_CM;
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

// Convert raw binary value to cm units
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


// Initialize three sensors: Check connectivity to all sensors and set their range and gain to maximum
bool sensorSrf08InitMultiple(void){

	// SENSOR 1
	if (!SENSOR_SELECT_1()) return false;
	uint8_t data;
	bool success = sensorReadReg(SW_REVISION, &data, 1);
	SENSOR_DESELECT();

	success *= sensorSrf08SetRangeMultiple(MAX_RANGE_6M);
	success *= sensorSrf08SetMaxGainMultiple(MAX_GAIN);

	// SENSOR 2
	if (!SENSOR_SELECT_2()) return false;
	success *= sensorReadReg(SW_REVISION, &data, 1);
	SENSOR_DESELECT();

	success *= sensorSrf08SetRangeMultiple(MAX_RANGE_6M);
	success *= sensorSrf08SetMaxGainMultiple(MAX_GAIN);

	// SENSOR 3
	if (!SENSOR_SELECT_3()) return false;
	success *= sensorReadReg(SW_REVISION, &data, 1);
	SENSOR_DESELECT();

	success *= sensorSrf08SetRangeMultiple(MAX_RANGE_6M);
	success *= sensorSrf08SetMaxGainMultiple(MAX_GAIN);

	return success;
}

// Set custom value of the range register of all three sensors
bool sensorSrf08SetRangeMultiple(uint8_t maxRange){
	// SENSOR 1
	if (!SENSOR_SELECT_1()){
		return false;
	}
	// Set Maximum Range
	bool success = sensorWriteReg(RANGE_REGISTER, &maxRange, 1);
	SENSOR_DESELECT();

	// SENSOR 2
	if (!SENSOR_SELECT_2()){
		return false;
	}
	// Set Maximum Range
	success *= sensorWriteReg(RANGE_REGISTER, &maxRange, 1);
	SENSOR_DESELECT();

	// SENSOR 3
	if (!SENSOR_SELECT_3()){
		return false;
	}
	// Set Maximum Range
	success *= sensorWriteReg(RANGE_REGISTER, &maxRange, 1);
	SENSOR_DESELECT();

	return success;
}

// Set custom value of the max gain register of all three sensors
bool sensorSrf08SetMaxGainMultiple(uint8_t maxGain){
	// SENSOR 1
	if (!SENSOR_SELECT_1()){
		return false;
	}
	// Set Maximum Analogue Gain
	bool success = sensorWriteReg(GAIN_REGISTER, &maxGain, 1);
	SENSOR_DESELECT();

	// SENSOR 2
	if (!SENSOR_SELECT_2()){
		return false;
	}
	// Set Maximum Analogue Gain
	success *= sensorWriteReg(GAIN_REGISTER, &maxGain, 1);
	SENSOR_DESELECT();

	// SENSOR 3
	if (!SENSOR_SELECT_3()){
		return false;
	}
	// Set Maximum Analogue Gain
	success *= sensorWriteReg(GAIN_REGISTER, &maxGain, 1);
	SENSOR_DESELECT();

	return success;
}

// Send scan command sequentially to all sensors, waiting 70 ms between each sensor. Then, read obtained data from all sensors
bool sensorSrf08ScanMultiple(uint16_t *data_1, uint16_t *data_2, uint16_t *data_3){

	bool success;
	uint8_t val = SCAN_CM;

	// Send "Range Start" command to SENSOR 1
	if (!SENSOR_SELECT_1()) return false;
	success = sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();

	// Wait until Scan is finished (70 ms)
	Task_sleep(70 * (1000 / Clock_tickPeriod));

	// Send "Range Start" command to SENSOR 2
	if (!SENSOR_SELECT_2()) return false;
	success *= sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();

	// Wait until Scan is finished (70 ms)
	Task_sleep(70 * (1000 / Clock_tickPeriod));

	// Send "Range Start" command to SENSOR 3
	if (!SENSOR_SELECT_3()) return false;
	success *= sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();

	// Wait until Scan is finished (70 ms)
	Task_sleep(70 * (1000 / Clock_tickPeriod));

	// Read Echoes received in SENSOR 1
	if (!SENSOR_SELECT_1()) return false;
	success *= sensorReadReg(FIRST_ECHO_H, (uint8_t*)data_1, DATA_SIZE);
	SENSOR_DESELECT();

	// Read Echoes received in SENSOR 2
	if (!SENSOR_SELECT_2()) return false;
	success *= sensorReadReg(FIRST_ECHO_H, (uint8_t*)data_2, DATA_SIZE);
	SENSOR_DESELECT();

	// Read Echoes received in SENSOR 3
	if (!SENSOR_SELECT_3()) return false;
	success *= sensorReadReg(FIRST_ECHO_H, (uint8_t*)data_3, DATA_SIZE);
	SENSOR_DESELECT();

	return success;
}

// Send scan command to sensor 1, and fake scan to sensors 2 and 3. Wait 70 ms and read data obtained from all sensors.
bool sensorSrf02ScanDistributed(uint16_t *data_1, uint16_t *data_2, uint16_t *data_3){

	bool success;
	uint8_t val;

	// Transmit burst with SENSOR 1
	val = SCAN_CM;
	if (!SENSOR_SELECT_1()) return false;
	success = sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();

	// Fake Scan SENSOR 2
	val = FAKE_SCAN_CM;
	if (!SENSOR_SELECT_2()) return false;
	success *= sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();

	// Fake Scan SENSOR 3
	if (!SENSOR_SELECT_3()) return false;
	success *= sensorWriteReg(COMMAND_REGISTER, &val, 1);
	SENSOR_DESELECT();

	// Wait until Scan is finished (70 ms)
	Task_sleep(70 * (1000 / Clock_tickPeriod));

	// Read Echoes received in SENSOR 1
	if (!SENSOR_SELECT_1()) return false;
	success *= sensorReadReg(FIRST_ECHO_H, (uint8_t*)data_1, DATA_SIZE);
	SENSOR_DESELECT();

	// Read Echoes received in SENSOR 2
	if (!SENSOR_SELECT_2()) return false;
	success *= sensorReadReg(FIRST_ECHO_H, (uint8_t*)data_2, DATA_SIZE);
	SENSOR_DESELECT();

	// Read Echoes received in SENSOR 3
	if (!SENSOR_SELECT_3()) return false;
	success *= sensorReadReg(FIRST_ECHO_H, (uint8_t*)data_3, DATA_SIZE);
	SENSOR_DESELECT();

	return success;

}


