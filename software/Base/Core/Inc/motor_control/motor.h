/*
 * motor.h
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#ifndef INC_MOTOR_CONTROL_MOTOR_H_
#define INC_MOTOR_CONTROL_MOTOR_H_

#include "stm32g4xx_hal.h"
#include "tim.h"
#include "user_interface/shell.h"

#define MOTOR_TIM_HANDLE &htim1
#define MOTOR_TIM_CHANNEL_U TIM_CHANNEL_1
#define MOTOR_TIM_CHANNEL_V TIM_CHANNEL_2

#define PWM_MAX_VAL 8500 // ARR + 1 (Period 8499)
#define PWM_MIN_VAL 0

/**
 * @brief Initializes the motor control module (PWM start, shell commands).
 */
void motor_init(void);

/**
 * @brief Sets the PWM duty cycle for the motor.
 * @param value Duty cycle value (0 to PWM_MAX_VAL).
 *              Typically, PWM_MAX_VAL/2 = Speed 0.
 */
void motor_set_PWM(int value);

/**
 * @brief Starts the PWM generation on U and V channels.
 */
void motor_start_pwm(void);

/**
 * @brief Stops the PWM generation on U and V channels.
 */
void motor_stop_pwm(void);

#endif /* INC_MOTOR_CONTROL_MOTOR_H_ */