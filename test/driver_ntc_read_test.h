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
 * @file      driver_ntc_read_test.h
 * @brief     driver ntc read test header file
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

#ifndef DRIVER_NTC_READ_TEST_H
#define DRIVER_NTC_READ_TEST_H

#include "driver_ntc_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ntc_test_driver ntc test driver function
 * @brief    ntc test driver modules
 * @ingroup  ntc_driver
 * @{
 */

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
                                   float beta, float r25_ohm, ntc_filter_t filter, uint32_t times);

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
                                     double a, double b, double c, ntc_filter_t filter, uint32_t times);

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
                                   ntc_filter_t filter, uint32_t times);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif