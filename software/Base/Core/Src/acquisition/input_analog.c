/*
 * input_analog.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "acquisition/input_analog.h"
#include "main.h"
#include <stdio.h>

int cmd_imes(h_shell_t* h_shell, int argc, char** argv);

volatile uint16_t adc_buffer[1];

void input_analog_init(void)
{
    shell_add(&hshell1, "imes", cmd_imes, "Measure Phase U Current (A)");
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 1) != HAL_OK)
    {
    }
}

float input_analog_get_u_current(void)
{
    float voltage = (float)adc_buffer[0] * 3.3f / 4095.0f;

    return (voltage - OFFSET_VOLTAGE) / SENSITIVITY_V_A;
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        HAL_GPIO_TogglePin(USR_LED_GPIO_Port, USR_LED_Pin);
    }
}

int cmd_imes(h_shell_t* h_shell, int argc, char** argv)
{
    float current = input_analog_get_u_current();
    int size = snprintf(h_shell->print_buffer, SHELL_PRINT_BUFFER_SIZE, "I_U = %.3f A\r\n", current);
    h_shell->drv.transmit(h_shell->print_buffer, size);
    return 0;
}
