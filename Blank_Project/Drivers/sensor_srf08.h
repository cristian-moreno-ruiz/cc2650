#ifndef SENSOR_SRF08_H
#define SENSOR_SRF08_H

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------------
 *                                          Includes
 * -----------------------------------------------------------------------------
 */
#include "stdint.h"
#include "stdbool.h"

/* -----------------------------------------------------------------------------
 *                                          Constants
 * -----------------------------------------------------------------------------
 */

/* ----------------------------------------------------------------------------
 *                                           Typedefs
 * -----------------------------------------------------------------------------
*/

/* -----------------------------------------------------------------------------
 *                                          Functions
 * -----------------------------------------------------------------------------
 */
bool sensorSrf08ChangeAddress(uint8_t oldAddress, uint8_t newAddress);
bool sensorSrf08Init(void);
bool sensorSrf08Scan(uint16_t *data);
int8_t sensorSrf08ConvertCm(uint8_t *data, uint16_t *cm);

bool sensorSrf08InitMultiple(void);
bool sensorSrf08ScanMultiple(uint16_t *data_1, uint16_t *data_2, uint16_t *data_3);

bool sensorSrf02ScanDistributed(uint16_t *data_1, uint16_t *data_2, uint16_t *data_3);


/*******************************************************************************
*/

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_SRF08_H */
