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

bool sensorSrf08Init(void);
uint8_t sensorSrf08Scan(uint16_t *data);

/*******************************************************************************
*/

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_SRF08_H */
