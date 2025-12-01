/*
 * motor.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "motor_control/motor.h"
#include <stdlib.h> // For atoi

// Private function prototypes for shell commands
int cmd_motor_start(h_shell_t* h_shell, int argc, char** argv);
int cmd_motor_stop(h_shell_t* h_shell, int argc, char** argv);
int cmd_motor_speed(h_shell_t* h_shell, int argc, char** argv);

void motor_init(void)
{
	// Ensure PWMs are stopped initially
	motor_stop_pwm();
	motor_set_PWM(PWM_MAX_VAL / 2);

	// Register shell commands
	shell_add(&hshell1, "start", cmd_motor_start, "Start Motor PWM generation");
	shell_add(&hshell1, "stop", cmd_motor_stop, "Stop Motor PWM generation");
	shell_add(&hshell1, "speed", cmd_motor_speed, "Set Motor speed (speed <val>)");
}

void motor_start_pwm(void)
{
	// Start PWM on Channel U (CH1) and Complementary (CH1N)
	HAL_TIM_PWM_Start(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_U);
	HAL_TIMEx_PWMN_Start(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_U);

	// Start PWM on Channel V (CH2) and Complementary (CH2N)
	HAL_TIM_PWM_Start(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_V);
	HAL_TIMEx_PWMN_Start(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_V);
}

void motor_stop_pwm(void)
{
	// Stop PWM on Channel U
	HAL_TIM_PWM_Stop(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_U);
	HAL_TIMEx_PWMN_Stop(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_U);

	// Stop PWM on Channel V
	HAL_TIM_PWM_Stop(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_V);
	HAL_TIMEx_PWMN_Stop(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_V);
}

void motor_set_PWM(int value)
{
	// Clamp value
	if (value < PWM_MIN_VAL) value = PWM_MIN_VAL;
	if (value > PWM_MAX_VAL) value = PWM_MAX_VAL;
	
	__HAL_TIM_SET_COMPARE(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_U, value);
	__HAL_TIM_SET_COMPARE(MOTOR_TIM_HANDLE, MOTOR_TIM_CHANNEL_V, value);
}

/* --- Shell Commands --- */

int cmd_motor_start(h_shell_t* h_shell, int argc, char** argv)
{
	motor_start_pwm();
	int size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "Power ON\r\n");
	h_shell->drv.transmit(h_shell->print_buffer, size);
	return 0;
}

int cmd_motor_stop(h_shell_t* h_shell, int argc, char** argv)
{
	motor_stop_pwm();
	int size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "Power OFF\r\n");
	h_shell->drv.transmit(h_shell->print_buffer, size);
	return 0;
}

int cmd_motor_speed(h_shell_t* h_shell, int argc, char** argv)
{
	if (argc < 2)
	{
		int size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "Usage: speed <value>\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		return -1;
	}

	int speed_val = atoi(argv[1]);
	int size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "DEBUG: Parsed speed_val: %d\r\n", speed_val);
	h_shell->drv.transmit(h_shell->print_buffer, size);

	motor_set_PWM(speed_val);

	size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "DEBUG: Speed set to %d (CCR1: %lu)\r\n", speed_val, htim1.Instance->CCR1);
	h_shell->drv.transmit(h_shell->print_buffer, size);
	return 0;
}
