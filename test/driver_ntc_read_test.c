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
 * @file      driver_ntc_read_test.c
 * @brief     driver ntc read test source file
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

#include "driver_ntc_read_test.h"

static ntc_handle_t gs_handle;        /**< ntc handle */

/**
 * @brief     read beta formula test
 * @param[in] circuit ntc circuit
 * @param[in] r_fixed_ohm fixed resistor in ohm
 * @param[in] beta beta value
 * @param[in] r25_ohm r25 ohm
 * @param[in] filter ntc filter
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ntc_read_beta_formula_test(ntc_circuit_t circuit, float r_fixed_ohm,
                                   float beta, float r25_ohm, ntc_filter_t filter, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    ntc_info_t info;
    
    /* link interface function */
    DRIVER_NTC_LINK_INIT(&gs_handle, ntc_handle_t);
    DRIVER_NTC_LINK_ADC_INIT(&gs_handle, ntc_interface_adc_init);
    DRIVER_NTC_LINK_ADC_DEINIT(&gs_handle, ntc_interface_adc_deinit);
    DRIVER_NTC_LINK_ADC_READ(&gs_handle, ntc_interface_adc_read);
    DRIVER_NTC_LINK_DELAY_MS(&gs_handle, ntc_interface_delay_ms);
    DRIVER_NTC_LINK_DEBUG_PRINT(&gs_handle, ntc_interface_debug_print);
    
    /* get information */
    res = ntc_info(&info);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        ntc_interface_debug_print("ntc: chip is %s.\n", info.chip_name);
        ntc_interface_debug_print("ntc: manufacturer is %s.\n", info.manufacturer_name);
        ntc_interface_debug_print("ntc: interface is %s.\n", info.interface);
        ntc_interface_debug_print("ntc: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ntc_interface_debug_print("ntc: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ntc_interface_debug_print("ntc: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ntc_interface_debug_print("ntc: max current is %0.2fmA.\n", info.max_current_ma);
        ntc_interface_debug_print("ntc: max temperature is %0.1fC.\n", info.temperature_max);
        ntc_interface_debug_print("ntc: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* init */
    res = ntc_init(&gs_handle);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: init failed.\n");
       
        return 1;
    }
    
    /* start read test */
    ntc_interface_debug_print("ntc: start read test.\n");
    
    /* output */
    if (circuit == NTC_CIRCUIT_VCC_NTC_R_GND)
    {
        ntc_interface_debug_print("ntc: circuit is vcc -> ntc -> r_fixed -> gnd.\n");
    }
    else
    {
        ntc_interface_debug_print("ntc: circuit is vcc -> r_fixed -> ntc -> gnd.\n");
    }
    ntc_interface_debug_print("ntc: circuit fixed resistor is %0.2f ohm.\n", r_fixed_ohm);
    ntc_interface_debug_print("ntc: beta formula beta value is %0.2f.\n", beta);
    ntc_interface_debug_print("ntc: beta formula r25 resistor is %0.2f ohm.\n", r25_ohm);
    if (filter == NTC_FILTER_NONE)
    {
        ntc_interface_debug_print("ntc: filter is none.\n");
    }
    else if (filter == NTC_FILTER_FIRST_ORDER_LAG)
    {
        ntc_interface_debug_print("ntc: filter is first order lag.\n");
    }
    else if (filter == NTC_FILTER_MEDIAN)
    {
        ntc_interface_debug_print("ntc: filter is median.\n");
    }
    else if (filter == NTC_FILTER_ANTI_SPIKE_AVERAGE)
    {
        ntc_interface_debug_print("ntc: filter is anti spike average.\n");
    }
    else if (filter == NTC_FILTER_MOVING_AVERAGE)
    {
        ntc_interface_debug_print("ntc: filter is moving average.\n");
    }
    else if (filter == NTC_FILTER_WEIGHTED_MOVING_AVERAGE)
    {
        ntc_interface_debug_print("ntc: filter is weighted moving average.\n");
    }
    else if (filter == NTC_FILTER_LIMITING)
    {
        ntc_interface_debug_print("ntc: filter is limiting.\n");
    }
    else
    {
        ntc_interface_debug_print("ntc: filter is kalman.\n");
    }
    
    /* set circuit */
    res = ntc_set_circuit(&gs_handle, circuit);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set circuit failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set circuit fixed resistor */
    res = ntc_set_circuit_fixed_resistor(&gs_handle, r_fixed_ohm);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set circuit fixed resistor failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set algorithm beta formula */
    res = ntc_set_algorithm(&gs_handle, NTC_ALGORITHM_BETA_FORMULA);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set algorithm failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set algorithm beta formula beta value */
    res = ntc_set_algorithm_beta_formula_beta_value(&gs_handle, beta);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set algorithm beta formula beta value failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set algorithm beta formula r25 ohm */
    res = ntc_set_algorithm_beta_formula_r25_ohm(&gs_handle, r25_ohm);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set algorithm beta formula r25 ohm failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    if (filter == NTC_FILTER_NONE)
    {
        /* set filter none */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_NONE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
    }
    else if (filter == NTC_FILTER_FIRST_ORDER_LAG)
    {
        /* set filter first order lag */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_FIRST_ORDER_LAG);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter first order lag 0.8 */
        res = ntc_set_filter_first_order_lag(&gs_handle, 0.8f);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter first order lag failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: first order lag filter alpha is %.02f.\n", 0.8f);
    }
    else if (filter == NTC_FILTER_MEDIAN)
    {
        /* set filter median */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_MEDIAN);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter median length 5 */
        res = ntc_set_filter_median_length(&gs_handle, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter median length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: median filter length is %d.\n", 5);
    }
    else if (filter == NTC_FILTER_ANTI_SPIKE_AVERAGE)
    {
        /* set filter anti spike average */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_ANTI_SPIKE_AVERAGE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter anti spike average length 5 */
        res = ntc_set_filter_anti_spike_average_length(&gs_handle, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter anti spike average length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: anti spike average filter length is %d.\n", 5);
    }
    else if (filter == NTC_FILTER_MOVING_AVERAGE)
    {
        /* set filter moving average */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_MOVING_AVERAGE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter moving average length 5 */
        res = ntc_set_filter_moving_average_length(&gs_handle, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter moving average length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: moving average filter length is %d.\n", 5);
    }
    else if (filter == NTC_FILTER_WEIGHTED_MOVING_AVERAGE)
    {
        float weight[] = {1.0, 2.0f, 3.0f, 4.0f, 5.0f};
        
        /* set filter weighted moving average */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_WEIGHTED_MOVING_AVERAGE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter weighted moving average length 5 */
        res = ntc_set_filter_weighted_moving_average_length(&gs_handle, weight, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter weighted moving average length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: weighted moving average filter is %0.2f %0.2f %0.2f %0.2f %0.2f.\n",
                                  weight[0], weight[1], weight[2], weight[3], weight[4]);
    }
    else if (filter == NTC_FILTER_LIMITING)
    {
        /* set filter limiting */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_LIMITING);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter limiting 10.0C */
        res = ntc_set_filter_limiting(&gs_handle, 10.0f);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter limiting failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: limiting filter is %0.2fC.\n", 10.0f);
    }
    else
    {
        /* set filter kalman */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_KALMAN);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter kalman */
        res = ntc_set_filter_kalman(&gs_handle, 0.005f, 0.5f, 1.0f, nanf(""));
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter kalman failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: kalman q(process noise covariance) is %0.3f.\n", 0.005f);
        ntc_interface_debug_print("ntc: kalman r(measurement noise covariance) is %0.2f.\n", 0.5f);
        ntc_interface_debug_print("ntc: kalman p(estimation error covariance) is %0.2f.\n", 1.0f);
        ntc_interface_debug_print("ntc: kalman x(estimated value) is read the current adc.\n");
    }
    
    for (i = 0; i < times; i++)
    {
        float ohm;
        float degrees_celsius;
        
        /* read temperature */
        res = ntc_read_temperature(&gs_handle, &ohm, &degrees_celsius);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: read temperature failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: resistance is %0.2fohm.\n", ohm);
        ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
        
        /* delay 1000ms */
        ntc_interface_delay_ms(1000);
    }
    
    /* finish read test */
    ntc_interface_debug_print("ntc: finish read test.\n");
    (void)ntc_deinit(&gs_handle);
    
    return 0;
}

/**
 * @brief     read steinhart hart test
 * @param[in] circuit ntc circuit
 * @param[in] r_fixed_ohm fixed resistor in ohm
 * @param[in] a param a
 * @param[in] b param b
 * @param[in] c param c
 * @param[in] filter ntc filter
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ntc_read_steinhart_hart_test(ntc_circuit_t circuit, float r_fixed_ohm,
                                     double a, double b, double c, ntc_filter_t filter, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    ntc_info_t info;
    
    /* link interface function */
    DRIVER_NTC_LINK_INIT(&gs_handle, ntc_handle_t);
    DRIVER_NTC_LINK_ADC_INIT(&gs_handle, ntc_interface_adc_init);
    DRIVER_NTC_LINK_ADC_DEINIT(&gs_handle, ntc_interface_adc_deinit);
    DRIVER_NTC_LINK_ADC_READ(&gs_handle, ntc_interface_adc_read);
    DRIVER_NTC_LINK_DELAY_MS(&gs_handle, ntc_interface_delay_ms);
    DRIVER_NTC_LINK_DEBUG_PRINT(&gs_handle, ntc_interface_debug_print);
    
    /* get information */
    res = ntc_info(&info);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        ntc_interface_debug_print("ntc: chip is %s.\n", info.chip_name);
        ntc_interface_debug_print("ntc: manufacturer is %s.\n", info.manufacturer_name);
        ntc_interface_debug_print("ntc: interface is %s.\n", info.interface);
        ntc_interface_debug_print("ntc: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ntc_interface_debug_print("ntc: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ntc_interface_debug_print("ntc: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ntc_interface_debug_print("ntc: max current is %0.2fmA.\n", info.max_current_ma);
        ntc_interface_debug_print("ntc: max temperature is %0.1fC.\n", info.temperature_max);
        ntc_interface_debug_print("ntc: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* init */
    res = ntc_init(&gs_handle);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: init failed.\n");
       
        return 1;
    }
    
    /* start read test */
    ntc_interface_debug_print("ntc: start read test.\n");
    
    /* output */
    if (circuit == NTC_CIRCUIT_VCC_NTC_R_GND)
    {
        ntc_interface_debug_print("ntc: circuit is vcc -> ntc -> r_fixed -> gnd.\n");
    }
    else
    {
        ntc_interface_debug_print("ntc: circuit is vcc -> r_fixed -> ntc -> gnd.\n");
    }
    ntc_interface_debug_print("ntc: circuit fixed resistor is %0.2f ohm.\n", r_fixed_ohm);
    ntc_interface_debug_print("ntc: steinhart hart A is %e.\n", a);
    ntc_interface_debug_print("ntc: steinhart hart B is %e.\n", b);
    ntc_interface_debug_print("ntc: steinhart hart C is %e.\n", c);
    if (filter == NTC_FILTER_NONE)
    {
        ntc_interface_debug_print("ntc: filter is none.\n");
    }
    else if (filter == NTC_FILTER_FIRST_ORDER_LAG)
    {
        ntc_interface_debug_print("ntc: filter is first order lag.\n");
    }
    else if (filter == NTC_FILTER_MEDIAN)
    {
        ntc_interface_debug_print("ntc: filter is median.\n");
    }
    else if (filter == NTC_FILTER_ANTI_SPIKE_AVERAGE)
    {
        ntc_interface_debug_print("ntc: filter is anti spike average.\n");
    }
    else if (filter == NTC_FILTER_MOVING_AVERAGE)
    {
        ntc_interface_debug_print("ntc: filter is moving average.\n");
    }
    else if (filter == NTC_FILTER_WEIGHTED_MOVING_AVERAGE)
    {
        ntc_interface_debug_print("ntc: filter is weighted moving average.\n");
    }
    else if (filter == NTC_FILTER_LIMITING)
    {
        ntc_interface_debug_print("ntc: filter is limiting.\n");
    }
    else
    {
        ntc_interface_debug_print("ntc: filter is kalman.\n");
    }
    
    /* set circuit */
    res = ntc_set_circuit(&gs_handle, circuit);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set circuit failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set circuit fixed resistor */
    res = ntc_set_circuit_fixed_resistor(&gs_handle, r_fixed_ohm);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set circuit fixed resistor failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set algorithm steinhart hart */
    res = ntc_set_algorithm(&gs_handle, NTC_ALGORITHM_STEINHART_HART);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set algorithm failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set algorithm steinhart hart */
    res = ntc_set_algorithm_steinhart_hart(&gs_handle, a, b, c);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set algorithm steinhart hart failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    if (filter == NTC_FILTER_NONE)
    {
        /* set filter none */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_NONE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
    }
    else if (filter == NTC_FILTER_FIRST_ORDER_LAG)
    {
        /* set filter first order lag */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_FIRST_ORDER_LAG);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter first order lag 0.8 */
        res = ntc_set_filter_first_order_lag(&gs_handle, 0.8f);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter first order lag failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: first order lag filter alpha is %.02f.\n", 0.8f);
    }
    else if (filter == NTC_FILTER_MEDIAN)
    {
        /* set filter median */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_MEDIAN);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter median length 5 */
        res = ntc_set_filter_median_length(&gs_handle, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter median length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: median filter length is %d.\n", 5);
    }
    else if (filter == NTC_FILTER_ANTI_SPIKE_AVERAGE)
    {
        /* set filter anti spike average */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_ANTI_SPIKE_AVERAGE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter anti spike average length 5 */
        res = ntc_set_filter_anti_spike_average_length(&gs_handle, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter anti spike average length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: anti spike average filter length is %d.\n", 5);
    }
    else if (filter == NTC_FILTER_MOVING_AVERAGE)
    {
        /* set filter moving average */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_MOVING_AVERAGE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter moving average length 5 */
        res = ntc_set_filter_moving_average_length(&gs_handle, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter moving average length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: moving average filter length is %d.\n", 5);
    }
    else if (filter == NTC_FILTER_WEIGHTED_MOVING_AVERAGE)
    {
        float weight[] = {1.0, 2.0f, 3.0f, 4.0f, 5.0f};
        
        /* set filter weighted moving average */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_WEIGHTED_MOVING_AVERAGE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter weighted moving average length 5 */
        res = ntc_set_filter_weighted_moving_average_length(&gs_handle, weight, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter weighted moving average length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: weighted moving average filter is %0.2f %0.2f %0.2f %0.2f %0.2f.\n",
                                  weight[0], weight[1], weight[2], weight[3], weight[4]);
    }
    else if (filter == NTC_FILTER_LIMITING)
    {
        /* set filter limiting */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_LIMITING);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter limiting 10.0C */
        res = ntc_set_filter_limiting(&gs_handle, 10.0f);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter limiting failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: limiting filter is %0.2fC.\n", 10.0f);
    }
    else
    {
        /* set filter kalman */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_KALMAN);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter kalman */
        res = ntc_set_filter_kalman(&gs_handle, 0.005f, 0.5f, 1.0f, nanf(""));
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter kalman failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: kalman q(process noise covariance) is %0.3f.\n", 0.005f);
        ntc_interface_debug_print("ntc: kalman r(measurement noise covariance) is %0.2f.\n", 0.5f);
        ntc_interface_debug_print("ntc: kalman p(estimation error covariance) is %0.2f.\n", 1.0f);
        ntc_interface_debug_print("ntc: kalman x(estimated value) is read the current adc.\n");
    }
    
    for (i = 0; i < times; i++)
    {
        float ohm;
        float degrees_celsius;
        
        /* read temperature */
        res = ntc_read_temperature(&gs_handle, &ohm, &degrees_celsius);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: read temperature failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: resistance is %0.2fohm.\n", ohm);
        ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
        
        /* delay 1000ms */
        ntc_interface_delay_ms(1000);
    }
    
    /* finish read test */
    ntc_interface_debug_print("ntc: finish read test.\n");
    (void)ntc_deinit(&gs_handle);
    
    return 0;
}

/**
 * @brief     read lookup table test
 * @param[in] circuit ntc circuit
 * @param[in] r_fixed_ohm fixed resistor in ohm
 * @param[in] *table pointer to a ntc table buffer
 * @param[in] table_len table length
 * @param[in] filter ntc filter
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ntc_read_lookup_table_test(ntc_circuit_t circuit, float r_fixed_ohm,
                                   const ntc_table_t *table, uint16_t table_len,
                                   ntc_filter_t filter, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    ntc_info_t info;
    
    /* link interface function */
    DRIVER_NTC_LINK_INIT(&gs_handle, ntc_handle_t);
    DRIVER_NTC_LINK_ADC_INIT(&gs_handle, ntc_interface_adc_init);
    DRIVER_NTC_LINK_ADC_DEINIT(&gs_handle, ntc_interface_adc_deinit);
    DRIVER_NTC_LINK_ADC_READ(&gs_handle, ntc_interface_adc_read);
    DRIVER_NTC_LINK_DELAY_MS(&gs_handle, ntc_interface_delay_ms);
    DRIVER_NTC_LINK_DEBUG_PRINT(&gs_handle, ntc_interface_debug_print);
    
    /* get information */
    res = ntc_info(&info);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        ntc_interface_debug_print("ntc: chip is %s.\n", info.chip_name);
        ntc_interface_debug_print("ntc: manufacturer is %s.\n", info.manufacturer_name);
        ntc_interface_debug_print("ntc: interface is %s.\n", info.interface);
        ntc_interface_debug_print("ntc: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ntc_interface_debug_print("ntc: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ntc_interface_debug_print("ntc: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ntc_interface_debug_print("ntc: max current is %0.2fmA.\n", info.max_current_ma);
        ntc_interface_debug_print("ntc: max temperature is %0.1fC.\n", info.temperature_max);
        ntc_interface_debug_print("ntc: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* init */
    res = ntc_init(&gs_handle);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: init failed.\n");
       
        return 1;
    }
    
    /* start read test */
    ntc_interface_debug_print("ntc: start read test.\n");
    
    /* output */
    if (circuit == NTC_CIRCUIT_VCC_NTC_R_GND)
    {
        ntc_interface_debug_print("ntc: circuit is vcc -> ntc -> r_fixed -> gnd.\n");
    }
    else
    {
        ntc_interface_debug_print("ntc: circuit is vcc -> r_fixed -> ntc -> gnd.\n");
    }
    ntc_interface_debug_print("ntc: circuit fixed resistor is %0.2f ohm.\n", r_fixed_ohm);
    ntc_interface_debug_print("ntc: table length is %d.\n", table_len);
    if (filter == NTC_FILTER_NONE)
    {
        ntc_interface_debug_print("ntc: filter is none.\n");
    }
    else if (filter == NTC_FILTER_FIRST_ORDER_LAG)
    {
        ntc_interface_debug_print("ntc: filter is first order lag.\n");
    }
    else if (filter == NTC_FILTER_MEDIAN)
    {
        ntc_interface_debug_print("ntc: filter is median.\n");
    }
    else if (filter == NTC_FILTER_ANTI_SPIKE_AVERAGE)
    {
        ntc_interface_debug_print("ntc: filter is anti spike average.\n");
    }
    else if (filter == NTC_FILTER_MOVING_AVERAGE)
    {
        ntc_interface_debug_print("ntc: filter is moving average.\n");
    }
    else if (filter == NTC_FILTER_WEIGHTED_MOVING_AVERAGE)
    {
        ntc_interface_debug_print("ntc: filter is weighted moving average.\n");
    }
    else if (filter == NTC_FILTER_LIMITING)
    {
        ntc_interface_debug_print("ntc: filter is limiting.\n");
    }
    else
    {
        ntc_interface_debug_print("ntc: filter is kalman.\n");
    }
    
    /* set circuit */
    res = ntc_set_circuit(&gs_handle, circuit);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set circuit failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set circuit fixed resistor */
    res = ntc_set_circuit_fixed_resistor(&gs_handle, r_fixed_ohm);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set circuit fixed resistor failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set algorithm lookup table */
    res = ntc_set_algorithm(&gs_handle, NTC_ALGORITHM_LOOKUP_TABLE);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set algorithm failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    /* load algorithm lookup table */
    res = ntc_load_algorithm_lookup_table(&gs_handle, table, table_len);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: set algorithm lookup table failed.\n");
        (void)ntc_deinit(&gs_handle);
        
        return 1;
    }
    
    if (filter == NTC_FILTER_NONE)
    {
        /* set filter none */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_NONE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
    }
    else if (filter == NTC_FILTER_FIRST_ORDER_LAG)
    {
        /* set filter first order lag */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_FIRST_ORDER_LAG);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter first order lag 0.8 */
        res = ntc_set_filter_first_order_lag(&gs_handle, 0.8f);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter first order lag failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: first order lag filter alpha is %.02f.\n", 0.8f);
    }
    else if (filter == NTC_FILTER_MEDIAN)
    {
        /* set filter median */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_MEDIAN);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter median length 5 */
        res = ntc_set_filter_median_length(&gs_handle, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter median length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: median filter length is %d.\n", 5);
    }
    else if (filter == NTC_FILTER_ANTI_SPIKE_AVERAGE)
    {
        /* set filter anti spike average */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_ANTI_SPIKE_AVERAGE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter anti spike average length 5 */
        res = ntc_set_filter_anti_spike_average_length(&gs_handle, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter anti spike average length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: anti spike average filter length is %d.\n", 5);
    }
    else if (filter == NTC_FILTER_MOVING_AVERAGE)
    {
        /* set filter moving average */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_MOVING_AVERAGE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter moving average length 5 */
        res = ntc_set_filter_moving_average_length(&gs_handle, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter moving average length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: moving average filter length is %d.\n", 5);
    }
    else if (filter == NTC_FILTER_WEIGHTED_MOVING_AVERAGE)
    {
        float weight[] = {1.0, 2.0f, 3.0f, 4.0f, 5.0f};
        
        /* set filter weighted moving average */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_WEIGHTED_MOVING_AVERAGE);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter weighted moving average length 5 */
        res = ntc_set_filter_weighted_moving_average_length(&gs_handle, weight, 5);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter weighted moving average length failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: weighted moving average filter is %0.2f %0.2f %0.2f %0.2f %0.2f.\n",
                                  weight[0], weight[1], weight[2], weight[3], weight[4]);
    }
    else if (filter == NTC_FILTER_LIMITING)
    {
        /* set filter limiting */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_LIMITING);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter limiting 10.0C */
        res = ntc_set_filter_limiting(&gs_handle, 10.0f);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter limiting failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: limiting filter is %0.2fC.\n", 10.0f);
    }
    else
    {
        /* set filter kalman */
        res = ntc_set_filter(&gs_handle, NTC_FILTER_KALMAN);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* set filter kalman */
        res = ntc_set_filter_kalman(&gs_handle, 0.005f, 0.5f, 1.0f, nanf(""));
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: set filter kalman failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: kalman q(process noise covariance) is %0.3f.\n", 0.005f);
        ntc_interface_debug_print("ntc: kalman r(measurement noise covariance) is %0.2f.\n", 0.5f);
        ntc_interface_debug_print("ntc: kalman p(estimation error covariance) is %0.2f.\n", 1.0f);
        ntc_interface_debug_print("ntc: kalman x(estimated value) is read the current adc.\n");
    }
    
    for (i = 0; i < times; i++)
    {
        float ohm;
        float degrees_celsius;
        
        /* read temperature */
        res = ntc_read_temperature(&gs_handle, &ohm, &degrees_celsius);
        if (res != 0)
        {
            ntc_interface_debug_print("ntc: read temperature failed.\n");
            (void)ntc_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ntc_interface_debug_print("ntc: resistance is %0.2fohm.\n", ohm);
        ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
        
        /* delay 1000ms */
        ntc_interface_delay_ms(1000);
    }
    
    /* finish read test */
    ntc_interface_debug_print("ntc: finish read test.\n");
    (void)ntc_deinit(&gs_handle);
    
    return 0;
}
