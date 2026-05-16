/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      adc.c
 * @brief     adc source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-11-11
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/11/11  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "adc.h"

/**
 * @brief adc var definition
 */
ADC_HandleTypeDef g_adc_handle;              /**< adc handle */

/**
 * @brief      adc init
 * @param[out] *vcc_counter pointer to a vcc counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 init failed
 * @note       adc pin is PA0
 */
uint8_t adc_init(uint32_t *vcc_counter)
{
    ADC_ChannelConfTypeDef adc_channel;
    
    g_adc_handle.Instance = ADC1;
    g_adc_handle.Init.ClockPrescaler=ADC_CLOCKPRESCALER_PCLK_DIV4;
    g_adc_handle.Init.Resolution = ADC_RESOLUTION_12B;
    g_adc_handle.Init.DataAlign =ADC_DATAALIGN_RIGHT;
    g_adc_handle.Init.ScanConvMode = DISABLE;
    g_adc_handle.Init.ContinuousConvMode= DISABLE;
    g_adc_handle.Init.NbrOfConversion= 1;
    g_adc_handle.Init.DiscontinuousConvMode= DISABLE;
    g_adc_handle.Init.NbrOfDiscConversion= 0;
    g_adc_handle.Init.ExternalTrigConv=ADC_SOFTWARE_START;
    g_adc_handle.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;
    g_adc_handle.Init.DMAContinuousRequests= DISABLE;
    
    /* adc init */
    if (HAL_ADC_Init(&g_adc_handle) != HAL_OK)
    {
        return 1;
    }
    
    adc_channel.Channel = ADC_CHANNEL_0;
    adc_channel.Rank = 1;
    adc_channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    
    /* config the adc channel */
    if (HAL_ADC_ConfigChannel(&g_adc_handle, &adc_channel) != HAL_OK)
    {
        (void)HAL_ADC_DeInit(&g_adc_handle);
        
        return 1;
    }
    
    /* set 4095 */
    *vcc_counter = 4095;
    
    return 0;
}

/**
 * @brief  adc deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t adc_deinit(void)
{
    /* adc deinit */
    if (HAL_ADC_DeInit(&g_adc_handle) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      adc read
 * @param[out] *counter pointer to a counter buffer
 * @param[in]  len adc length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t adc_read(uint32_t *counter, uint16_t len)
{
    uint16_t i;
    
    for (i = 0; i < len; i++)
    {
        /* adc start */
        if (HAL_ADC_Start(&g_adc_handle) != HAL_OK)
        {
            return 1;
        }
        
        /* adc poll */
        if (HAL_ADC_PollForConversion(&g_adc_handle, 10) != HAL_OK)
        {
            return 1;
        }
        
        /* get the adc value */
        counter[i] = HAL_ADC_GetValue(&g_adc_handle);
    }
    
    return 0;
}
