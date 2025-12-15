/*
 * input_analog.h
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#ifndef INC_INPUT_ANALOG_H_
#define INC_INPUT_ANALOG_H_

#include "adc.h"
#include "user_interface/shell.h"

// Sensor parameters for LEM GO 10-SME/SP3
#define SENSITIVITY_V_A 0.050f
#define OFFSET_VOLTAGE  1.65f

void input_analog_init(void);
float input_analog_get_u_current(void);




#endif /* INC_INPUT_ANALOG_H_ */
