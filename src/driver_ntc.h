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
 * @file      driver_ntc.h
 * @brief     driver ntc header file
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

#ifndef DRIVER_NTC_H
#define DRIVER_NTC_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ntc_driver ntc driver function
 * @brief    ntc driver modules
 * @{
 */

/**
 * @addtogroup ntc_basic_driver
 * @{
 */

/**
 * @brief ntc filter buffer size definition
 */
#ifndef NTC_FILTER_BUFFER_SIZE
    #define NTC_FILTER_BUFFER_SIZE        32        /**< 32 */
#endif

/**
 * @brief ntc circuit enumeration definition
 */
typedef enum
{
    NTC_CIRCUIT_VCC_NTC_R_GND = 0x00,        /**< vcc -> ntc -> r_fixed -> gnd */
    NTC_CIRCUIT_VCC_R_NTC_GND = 0x01,        /**< vcc -> r_fixed -> ntc -> gnd */
} ntc_circuit_t;

/**
 * @brief ntc algorithm enumeration definition
 */
typedef enum
{
    NTC_ALGORITHM_BETA_FORMULA   = 0x00,        /**< beta formula */
    NTC_ALGORITHM_STEINHART_HART = 0x01,        /**< stein-hart hart */
    NTC_ALGORITHM_LOOKUP_TABLE   = 0x02,        /**< lookup table */
} ntc_algorithm_t;

/**
 * @brief ntc filter enumeration definition
 */
typedef enum
{
    NTC_FILTER_NONE                    = 0x00,        /**< no filter */
    NTC_FILTER_FIRST_ORDER_LAG         = 0x01,        /**< first order lag filter */
    NTC_FILTER_MEDIAN                  = 0x02,        /**< median filter */
    NTC_FILTER_ANTI_SPIKE_AVERAGE      = 0x03,        /**< anti spike average filter */
    NTC_FILTER_MOVING_AVERAGE          = 0x04,        /**< moving average filter */
    NTC_FILTER_WEIGHTED_MOVING_AVERAGE = 0x05,        /**< weighted moving average filter */
    NTC_FILTER_LIMITING                = 0x06,        /**< limiting filter */
    NTC_FILTER_KALMAN                  = 0x07,        /**< kalman filter */
} ntc_filter_t;

/**
 * @brief ntc table structure definition
 */
typedef struct ntc_table_s
{
    float degrees_celsius;        /**< degrees celsius */
    float ohm;                    /**< ohm */
} ntc_table_t;

/**
 * @brief ntc handle structure definition
 */
typedef struct ntc_handle_s
{
    uint8_t (*adc_init)(uint32_t *vcc_counter);                  /**< pointer to an adc_init function address */
    uint8_t (*adc_deinit)(void);                                 /**< pointer to an adc_deinit function address */
    uint8_t (*adc_read)(uint32_t *counter, uint16_t len);        /**< pointer to an adc_read function address */
    void (*delay_ms)(uint32_t ms);                               /**< pointer to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);             /**< pointer to a debug_print function address */
    uint8_t inited;                                              /**< inited flag */
    uint8_t circuit;                                             /**< circuit */
    uint32_t vcc_counter;                                        /**< vcc counter */
    float r_fixed_ohm;                                           /**< r fixed in ohm */
    uint8_t algorithm;                                           /**< algorithm */
    float beta;                                                  /**< beta formula beta value of ntc */
    float r25_ohm;                                               /**< beta formula 25C resistor value of ntc */
    double a;                                                    /**< stein-hart hart a */
    double b;                                                    /**< stein-hart hart b */
    double c;                                                    /**< stein-hart hart c */
    const ntc_table_t *table;                                    /**< pointer to a ntc table */
    uint16_t table_len;                                          /**< table length */
    uint8_t filter;                                              /**< filter */
    uint32_t buf[NTC_FILTER_BUFFER_SIZE];                        /**< buffer */
    float buf_flt[NTC_FILTER_BUFFER_SIZE];                       /**< buffer float */
    float cache_flt[NTC_FILTER_BUFFER_SIZE];                     /**< cache float */
    float param_flt[NTC_FILTER_BUFFER_SIZE];                     /**< param float */
    uint16_t filter_len;                                         /**< filter buffer length */
    uint16_t filter_pointer;                                     /**< filter pointer */
} ntc_handle_t;

/**
 * @brief ntc information structure definition
 */
typedef struct ntc_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ntc_info_t;

/**
 * @}
 */

/**
 * @defgroup ntc_link_driver ntc link driver function
 * @brief    ntc link driver modules
 * @ingroup  ntc_driver
 * @{
 */

/**
 * @brief     initialize ntc_handle_t structure
 * @param[in] HANDLE pointer to a ntc handle structure
 * @param[in] STRUCTURE ntc_handle_t
 * @note      none
 */
#define DRIVER_NTC_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link adc_init function
 * @param[in] HANDLE pointer to a ntc handle structure
 * @param[in] FUC pointer to an adc_init function address
 * @note      none
 */
#define DRIVER_NTC_LINK_ADC_INIT(HANDLE, FUC)          (HANDLE)->adc_init = FUC

/**
 * @brief     link adc_deinit function
 * @param[in] HANDLE pointer to a ntc handle structure
 * @param[in] FUC pointer to an adc_deinit function address
 * @note      none
 */
#define DRIVER_NTC_LINK_ADC_DEINIT(HANDLE, FUC)        (HANDLE)->adc_deinit = FUC

/**
 * @brief     link adc_read function
 * @param[in] HANDLE pointer to a ntc handle structure
 * @param[in] FUC pointer to an adc_read function address
 * @note      none
 */
#define DRIVER_NTC_LINK_ADC_READ(HANDLE, FUC)          (HANDLE)->adc_read = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a ntc handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_NTC_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a ntc handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_NTC_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup ntc_basic_driver ntc basic driver function
 * @brief    ntc basic driver modules
 * @ingroup  ntc_driver
 * @{
 */


/**
 * @brief      get chip's information
 * @param[out] *info pointer to a ntc info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ntc_info(ntc_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a ntc handle structure
 * @return    status code
 *            - 0 success
 *            - 1 adc initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 vcc counter is invalid
 * @note      none
 */
uint8_t ntc_init(ntc_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a ntc handle structure
 * @return    status code
 *            - 0 success
 *            - 1 adc deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntc_deinit(ntc_handle_t *handle);

/**
 * @brief      read temperature
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[out] *ohm pointer to a resistor in ohm
 * @param[out] *degrees_celsius pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 1 read adc failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 open circuit
 *             - 5 short circuit
 *             - 6 filter error
 *             - 7 temperature convert failed
 * @note       none
 */
uint8_t ntc_read_temperature(ntc_handle_t *handle, float *ohm, float *degrees_celsius);

/**
 * @brief      calculate temperature
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[in]  ohm resistor in ohm
 * @param[out] *degrees_celsius pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 1 convert failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       this function does not perform any filtering processing, 
 *             does not rely on any adc read values, 
 *             and directly performs temperature conversion based on the resistance value
 */
uint8_t ntc_calculate_temperature(ntc_handle_t *handle, float ohm, float *degrees_celsius);

/**
 * @brief      calculate temperature with filter
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[in]  *ohm pointer to a resistor in ohm
 * @param[in]  len resistor length
 * @param[out] *degrees_celsius pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 1 filter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 convert failed
 * @note       this function can perform filtering processing, 
 *             does not rely on any adc read values
 */
uint8_t ntc_calculate_temperature_with_filter(ntc_handle_t *handle, float *ohm, uint16_t len, float *degrees_celsius);

/**
 * @brief     set algorithm
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] algorithm ntc algorithm type
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      \f$
 *            \begin{align}
 *            & \text{beta formula} \\ 
 *            & t = \frac{1} {\frac {\ln (\frac{r} {{{r}_{25}}})} {b} + \frac{1} {{{t}_{25}}}} - 273.15 \\ 
 *            & \text{steinhart-hart} \\ 
 *            & \text{t = } \frac{1} {a + b \ln (r) + c{{(\ln (r))}^{3}}} \\ 
 *            \end{align}
 *            \f$
 */
uint8_t ntc_set_algorithm(ntc_handle_t *handle, ntc_algorithm_t algorithm);

/**
 * @brief      get algorithm
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[out] *algorithm pointer to a ntc algorithm type buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntc_get_algorithm(ntc_handle_t *handle, ntc_algorithm_t *algorithm);

/**
 * @brief     set algorithm beta formula beta value
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] beta beta value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntc_set_algorithm_beta_formula_beta_value(ntc_handle_t *handle, float beta);

/**
 * @brief      get algorithm beta formula beta value
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[out] *beta pointer to a beta value buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntc_get_algorithm_beta_formula_beta_value(ntc_handle_t *handle, float *beta);

/**
 * @brief     set algorithm beta formula r25 ohm
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] r25_ohm r25 ohm
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntc_set_algorithm_beta_formula_r25_ohm(ntc_handle_t *handle, float r25_ohm);

/**
 * @brief      get algorithm beta formula r25 ohm
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[out] *r25_ohm pointer to a r25 ohm buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntc_get_algorithm_beta_formula_r25_ohm(ntc_handle_t *handle, float *r25_ohm);

/**
 * @brief     set algorithm steinhart hart
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] a param a
 * @param[in] b param b
 * @param[in] c param c
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntc_set_algorithm_steinhart_hart(ntc_handle_t *handle, double a, double b, double c);

/**
 * @brief      get algorithm steinhart hart
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[out] *a pointer to a param a buffer
 * @param[out] *b pointer to a param b buffer
 * @param[out] *c pointer to a param c buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntc_get_algorithm_steinhart_hart(ntc_handle_t *handle, double *a, double *b, double *c);

/**
 * @brief     load algorithm lookup table
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] *table pointer to a ntc table buffer
 * @param[in] table_len table length
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 table_len is 0
 *            - 5 table ohm is invalid
 *            - 6 table degrees celsius is invalid
 * @note      none
 */
uint8_t ntc_load_algorithm_lookup_table(ntc_handle_t *handle, const ntc_table_t *table, uint16_t table_len);

/**
 * @brief     set circuit
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] circuit ntc circuit
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntc_set_circuit(ntc_handle_t *handle, ntc_circuit_t circuit);

/**
 * @brief      get circuit
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[out] *circuit pointer to a ntc circuit buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntc_get_circuit(ntc_handle_t *handle, ntc_circuit_t *circuit);

/**
 * @brief     set circuit fixed resistor
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] r_fixed_ohm fixed resistor in ohm
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntc_set_circuit_fixed_resistor(ntc_handle_t *handle, float r_fixed_ohm);

/**
 * @brief      get circuit fixed resistor
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[out] *r_fixed_ohm pointer to a fixed resistor buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntc_get_circuit_fixed_resistor(ntc_handle_t *handle, float *r_fixed_ohm);

/**
 * @}
 */

/**
 * @defgroup ntc_filter_driver ntc filter driver function
 * @brief    ntc filter driver modules
 * @ingroup  ntc_driver
 * @{
 */

/**
 * @brief     set filter
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] filter ntc filter
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntc_set_filter(ntc_handle_t *handle, ntc_filter_t filter);

/**
 * @brief      get filter
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[out] *filter pointer to a ntc filter buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntc_get_filter(ntc_handle_t *handle, ntc_filter_t *filter);

/**
 * @brief     reset the filter
 * @param[in] *handle pointer to a ntc handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntc_reset_filter(ntc_handle_t *handle);

/**
 * @brief     set filter first order lag
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] alpha input alpha
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 NTC_FILTER_BUFFER_SIZE < 2
 *            - 5 alpha < 0.0f
 *            - 6 alpha > 1.0f
 * @note      \f$
 *            y(n) = \alpha \cdot x(n) + (1 - \alpha) \cdot y(n - 1)
 *            \f$
 */
uint8_t ntc_set_filter_first_order_lag(ntc_handle_t *handle, float alpha);

/**
 * @brief     set filter median length
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] length median length
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 NTC_FILTER_BUFFER_SIZE < length
 *            - 5 median filter length must be odd number
 *            - 6 length < 3
 * @note      \f$
 *            y(n) = med \{ x(n-k), ..., x(n), ..., x(n+k) \}
 *            \f$
 */
uint8_t ntc_set_filter_median_length(ntc_handle_t *handle, uint16_t length);

/**
 * @brief     set filter anti spike average length
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] length anti spike average length
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 NTC_FILTER_BUFFER_SIZE < length
 *            - 5 length < 3
 * @note      \f$
 *            y = \frac{1}{n-2} \left ( \sum\limits_{i = 1}^{n} {{{x}_{i}} - ({{x}_{(1)}} + {{x}_{(n)}})} \right )
 *            \f$
 */
uint8_t ntc_set_filter_anti_spike_average_length(ntc_handle_t *handle, uint16_t length);

/**
 * @brief     set filter moving average length
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] length moving average length
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 NTC_FILTER_BUFFER_SIZE < length
 *            - 5 length is 0
 * @note      \f$
 *            y(n) = \frac{1}{N} \sum \limits_{i = 0}^{N-1}{x(n-i)}
 *            \f$
 */
uint8_t ntc_set_filter_moving_average_length(ntc_handle_t *handle, uint16_t length);

/**
 * @brief     set filter weighted moving average length
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] *weight pointer to a weight buffer
 * @param[in] length weighted moving average length
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 NTC_FILTER_BUFFER_SIZE < length
 *            - 5 length is 0
 * @note      \f$
 *            y(n) = \frac {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}} \cdot x(n-i)}} {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}}}}
 *            \f$
 */
uint8_t ntc_set_filter_weighted_moving_average_length(ntc_handle_t *handle, float *weight, uint16_t length);

/**
 * @brief     set filter limiting
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] degrees_celsius degrees celsius
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 NTC_FILTER_BUFFER_SIZE < 2
 * @note      \f$
 *            y(n) = \begin{cases} x(n) & |x(n) - y(n-1)| \le \Delta \\ y(n-1) & |x(n) - y(n-1)| > \Delta \end{cases}
 *            \f$
 */
uint8_t ntc_set_filter_limiting(ntc_handle_t *handle, float degrees_celsius);

/**
 * @brief     set filter kalman
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] q_process_noise_covariance process noise covariance
 * @param[in] r_measurement_noise_covariance measurement noise covariance
 * @param[in] p_estimation_error_covariance estimation error covariance
 * @param[in] x_estimated_value x estimated value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 NTC_FILTER_BUFFER_SIZE < 3
 *            - 5 adc read failed
 * @note      \f$
 *            \hat{x}_{k \vert{} k-1} = \hat{x}_{k-1 \vert{} k-1} \\
 *            P_{k \vert{} k-1} = P_{k-1 \vert{} k-1} + Q \\
 *            K_k = \frac{P_{k \vert{} k-1}}{P_{k \vert{} k-1} + R} \\
 *            \hat{x}_{k \vert{} k} = \hat{x}_{k \vert{} k-1} + K_k(z_k - \hat{x}_{k \vert{} k-1}) \\
 *            P_{k \vert{} k} = (1 - K_k)P_{k \vert{} k-1}
 *            \f$
 */
uint8_t ntc_set_filter_kalman(ntc_handle_t *handle,
                              float q_process_noise_covariance,
                              float r_measurement_noise_covariance,
                              float p_estimation_error_covariance,
                              float x_estimated_value);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
