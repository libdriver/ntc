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
 * @file      driver_ntc_basic.c
 * @brief     driver ntc basic source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2026-05-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2026/05/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ntc_basic.h"

static ntc_handle_t gs_handle;        /**< ntc handle */

/**
 * @brief     basic example init
 * @param[in] circuit ntc circuit
 * @param[in] r_fixed_ohm fixed resistor in ohm
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ntc_basic_init(ntc_circuit_t circuit, float r_fixed_ohm)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_NTC_LINK_INIT(&gs_handle, ntc_handle_t);
    DRIVER_NTC_LINK_ADC_INIT(&gs_handle, ntc_interface_adc_init);
    DRIVER_NTC_LINK_ADC_DEINIT(&gs_handle, ntc_interface_adc_deinit);
    DRIVER_NTC_LINK_ADC_READ(&gs_handle, ntc_interface_adc_read);
    DRIVER_NTC_LINK_DELAY_MS(&gs_handle, ntc_interface_delay_ms);
    DRIVER_NTC_LINK_DEBUG_PRINT(&gs_handle, ntc_interface_debug_print);
    
    /* init */
    res = ntc_init(&gs_handle);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: init failed.\n");
       
        return 1;
    }
    
    /* set circuit */
    res = ntc_set_circuit(&gs_handle, circuit);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set circuit.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set circuit fixed resistor */
    res = ntc_set_circuit_fixed_resistor(&gs_handle, r_fixed_ohm);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set circuit fixed resistor.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ntc_basic_deinit(void)
{
    if (ntc_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set algorithm beta formula
 * @param[in] beta beta value
 * @param[in] r25_ohm r25 ohm
 * @return    status code
 *            - 0 success
 *            - 1 set algorithm failed
 * @note      none
 */
uint8_t ntc_basic_set_algorithm_beta_formula(float beta, float r25_ohm)
{
    uint8_t res;
    
    /* set algorithm beta formula */
    res = ntc_set_algorithm(&gs_handle, NTC_ALGORITHM_BETA_FORMULA);
    if (res != 0)
    {
        return 1;
    }
    
    /* set algorithm beta formula beta value */
    res = ntc_set_algorithm_beta_formula_beta_value(&gs_handle, beta);
    if (res != 0)
    {
        return 1;
    }
    
    /* set algorithm beta formula r25 ohm */
    res = ntc_set_algorithm_beta_formula_r25_ohm(&gs_handle, r25_ohm);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set algorithm steinhart hart
 * @param[in] a param a
 * @param[in] b param b
 * @param[in] c param c
 * @return    status code
 *            - 0 success
 *            - 1 set algorithm failed
 * @note      none
 */
uint8_t ntc_basic_set_algorithm_steinhart_hart(double a, double b, double c)
{
    uint8_t res;
    
    /* set algorithm steinhart hart */
    res = ntc_set_algorithm(&gs_handle, NTC_ALGORITHM_STEINHART_HART);
    if (res != 0)
    {
        return 1;
    }
    
    /* set algorithm steinhart hart */
    res = ntc_set_algorithm_steinhart_hart(&gs_handle, a, b, c);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set algorithm lookup table
 * @param[in] *table pointer to a ntc table buffer
 * @param[in] table_len table length
 * @return    status code
 *            - 0 success
 *            - 1 set algorithm failed
 * @note      none
 */
uint8_t ntc_basic_set_algorithm_lookup_table(const ntc_table_t *table, uint16_t table_len)
{
    uint8_t res;
    
    /* set algorithm steinhart hart */
    res = ntc_set_algorithm(&gs_handle, NTC_ALGORITHM_LOOKUP_TABLE);
    if (res != 0)
    {
        return 1;
    }
    
    /* set algorithm lookup table */
    res = ntc_load_algorithm_lookup_table(&gs_handle, table, table_len);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example set filter none
 * @return status code
 *         - 0 success
 *         - 1 set filter failed
 * @note   none
 */
uint8_t ntc_basic_set_filter_none(void)
{
    uint8_t res;
    
    /* set filter */
    res = ntc_set_filter(&gs_handle, NTC_FILTER_NONE);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set filter first order lag
 * @param[in] alpha input alpha
 * @return    status code
 *            - 0 success
 *            - 1 set filter failed
 * @note      none
 */
uint8_t ntc_basic_set_filter_first_order_lag(float alpha)
{
    uint8_t res;
    
    /* set filter */
    res = ntc_set_filter(&gs_handle, NTC_FILTER_FIRST_ORDER_LAG);
    if (res != 0)
    {
        return 1;
    }
    
    /* filter first order lag */
    res = ntc_set_filter_first_order_lag(&gs_handle, alpha);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set filter median
 * @param[in] length median length
 * @return    status code
 *            - 0 success
 *            - 1 set filter failed
 * @note      none
 */
uint8_t ntc_basic_set_filter_median(uint16_t length)
{
    uint8_t res;
    
    /* set filter */
    res = ntc_set_filter(&gs_handle, NTC_FILTER_MEDIAN);
    if (res != 0)
    {
        return 1;
    }
    
    /* set median filter length */
    res = ntc_set_filter_median_length(&gs_handle, length);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set filter anti spike average
 * @param[in] length anti spike average length
 * @return    status code
 *            - 0 success
 *            - 1 set filter failed
 * @note      none
 */
uint8_t ntc_basic_set_filter_anti_spike_average(uint16_t length)
{
    uint8_t res;
    
    /* set filter */
    res = ntc_set_filter(&gs_handle, NTC_FILTER_ANTI_SPIKE_AVERAGE);
    if (res != 0)
    {
        return 1;
    }
    
    /* set anti spike average length */
    res = ntc_set_filter_anti_spike_average_length(&gs_handle, length);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set filter moving average
 * @param[in] length moving average length
 * @return    status code
 *            - 0 success
 *            - 1 set filter failed
 * @note      none
 */
uint8_t ntc_basic_set_filter_moving_average(uint16_t length)
{
    uint8_t res;
    
    /* set filter */
    res = ntc_set_filter(&gs_handle, NTC_FILTER_MOVING_AVERAGE);
    if (res != 0)
    {
        return 1;
    }
    
    /* set moving average length */
    res = ntc_set_filter_moving_average_length(&gs_handle, length);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set filter weighted moving average
 * @param[in] *weight pointer to a weight buffer
 * @param[in] length weighted moving average length
 * @return    status code
 *            - 0 success
 *            - 1 set filter failed
 * @note      weight[0](older), ..., weight[n](newer)
 */
uint8_t ntc_basic_set_filter_weighted_moving_average(float *weight, uint16_t length)
{
    uint8_t res;
    
    /* set filter */
    res = ntc_set_filter(&gs_handle, NTC_FILTER_WEIGHTED_MOVING_AVERAGE);
    if (res != 0)
    {
        return 1;
    }
    
    /* set weighted moving average length */
    res = ntc_set_filter_weighted_moving_average_length(&gs_handle, weight, length);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set filter limiting
 * @param[in] degrees_celsius degrees celsius
 * @return    status code
 *            - 0 success
 *            - 1 set filter failed
 * @note      none
 */
uint8_t ntc_basic_set_filter_limiting(float degrees_celsius)
{
    uint8_t res;
    
    /* set filter */
    res = ntc_set_filter(&gs_handle, NTC_FILTER_LIMITING);
    if (res != 0)
    {
        return 1;
    }
    
    /* set limiting */
    res = ntc_set_filter_limiting(&gs_handle, degrees_celsius);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set filter kalman
 * @param[in] q_process_noise_covariance process noise covariance
 * @param[in] r_measurement_noise_covariance measurement noise covariance
 * @param[in] p_estimation_error_covariance estimation error covariance
 * @param[in] x_estimated_value x estimated value
 * @return    status code
 *            - 0 success
 *            - 1 set filter failed
 * @note      none
 */
uint8_t ntc_basic_set_filter_kalman(float q_process_noise_covariance,
                                    float r_measurement_noise_covariance,
                                    float p_estimation_error_covariance,
                                    float x_estimated_value)
{
    uint8_t res;
    
    /* set filter */
    res = ntc_set_filter(&gs_handle, NTC_FILTER_KALMAN);
    if (res != 0)
    {
        return 1;
    }
    
    /* set kalman */
    res = ntc_set_filter_kalman(&gs_handle,
                                q_process_noise_covariance,
                                r_measurement_noise_covariance,
                                p_estimation_error_covariance,
                                x_estimated_value);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example reset filter
 * @return status code
 *         - 0 success
 *         - 1 reset filter failed
 * @note   none
 */
uint8_t ntc_basic_reset_filter(void)
{
    uint8_t res;
    
    /* reset filter */
    res = ntc_reset_filter(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *degrees_celsius pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ntc_basic_read(float *degrees_celsius)
{
    uint8_t res;
    float ohm;
    
    /* read temperature */
    res = ntc_read_temperature(&gs_handle, &ohm, degrees_celsius);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic calculate temperature
 * @param[in]  ohm resistor in ohm
 * @param[out] *degrees_celsius pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 1 calculate failed
 * @note       none
 */
uint8_t ntc_basic_calculate_temperature(float ohm, float *degrees_celsius)
{
    uint8_t res;
    
    /* calculate temperature */
    res = ntc_calculate_temperature(&gs_handle, ohm, degrees_celsius);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic calculate temperature with filter
 * @param[in]  *ohm pointer to a resistor in ohm
 * @param[in]  len resistor length
 * @param[out] *degrees_celsius pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 1 calculate failed
 * @note       none
 */
uint8_t ntc_basic_calculate_temperature_with_filter(float *ohm, uint16_t len, float *degrees_celsius)
{
    uint8_t res;
    
    /* calculate temperature with filter */
    res = ntc_calculate_temperature_with_filter(&gs_handle, ohm, len, degrees_celsius);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
