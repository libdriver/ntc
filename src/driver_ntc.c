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
 * @file      driver_ntc.c
 * @brief     driver ntc source file
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

#include "driver_ntc.h"
#include <math.h>

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "General NTC"        /**< chip name */
#define MANUFACTURER_NAME         "General"            /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.8f                 /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                 /**< chip max supply voltage */
#define MAX_CURRENT               1.0f                 /**< chip max current */
#define TEMPERATURE_MIN           -50.0f               /**< chip min operating temperature */
#define TEMPERATURE_MAX           250.0f               /**< chip max operating temperature */
#define DRIVER_VERSION            1000                 /**< driver version */

/**
 * @brief     ntc beta formula
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] ohm input resistance
 * @return    converted temperature
 * @note      none
 */
static float a_ntc_beta_formula(ntc_handle_t *handle, float ohm)
{
    float b;
    float r0;
    float t0;
    float temp_k;
    
    b = handle->beta;                                          /* set b */
    r0 = handle->r25_ohm;                                      /* set r0 */
    t0 = 298.15f;                                              /* set t0 */
    temp_k = 1.0f / ((1.0f / t0) + logf(ohm / r0) / b);        /* calculate k */
    
    return temp_k - 273.15f;                                   /* get the final value */
}

/**
 * @brief     ntc steinhart hart
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] ohm input resistance
 * @return    converted temperature
 * @note      none
 */
static float a_ntc_steinhart_hart(ntc_handle_t *handle, float ohm)
{
    double a;
    double b;
    double c;
    double log_r;
    double log_r3;
    double temp_k;
    
    a = handle->a;                                      /* set a */
    b = handle->b;                                      /* set b */
    c = handle->c;                                      /* set c */
    log_r = log((double)ohm);                           /* calculate r */
    log_r3 = log_r * log_r * log_r;                     /* calculate r3 */
    temp_k = 1.0 / (a + b * log_r + c * log_r3);        /* calculate k */
    
    return (float)(temp_k - 273.15);                    /* get the final value */
}

/**
 * @brief     ntc table
 * @param[in] *handle pointer to a ntc handle structure
 * @param[in] ohm input resistance
 * @return    converted temperature
 * @note      none
 */
static float a_ntc_table(ntc_handle_t *handle, float ohm)
{
    int32_t low;
    int32_t high;
    int32_t mid;
    float temp;
    
    low = 0;                                                                                           /* set start */
    high = (int32_t)(handle->table_len - 1);                                                           /* set end */
    while (low <= high)                                                                                /* loop */
    {
        mid = (low + high) / 2;                                                                        /* set middle value */
        if (ohm > handle->table[mid].ohm)                                                              /* check range */
        {
            high = mid - 1;                                                                            /* middle - 1 */
        }
        else if (ohm < handle->table[mid].ohm)                                                         /* check range */
        {
            low = mid + 1;                                                                             /* middle + 1 */
        }
        else
        {
            return handle->table[mid].degrees_celsius;                                                 /* return the temp */
        }
    }
    
    temp = handle->table[high].degrees_celsius + 
          (handle->table[low].degrees_celsius - handle->table[high].degrees_celsius) * 
          (ohm - handle->table[high].ohm) / (handle->table[low].ohm - handle->table[high].ohm);        /* get the final value */
    
    return temp;                                                                                       /* return the temp */
}

/**
 * @brief      ntc convert
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[in]  ohm resistor in ohm
 * @param[out] *degrees_celsius pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 1 out of table
 *             - 2 table length is 0
 * @note       none
 */
static uint8_t a_ntc_convert(ntc_handle_t *handle, float ohm, float *degrees_celsius)
{
    if (handle->algorithm == (uint8_t)NTC_ALGORITHM_BETA_FORMULA)               /* beta formula */
    {
        *degrees_celsius = a_ntc_beta_formula(handle, ohm);                     /* convert */
        
        return 0;                                                               /* success return 0 */
    }
    else if (handle->algorithm == (uint8_t)NTC_ALGORITHM_STEINHART_HART)        /* steinhart hart */
    {
        *degrees_celsius = a_ntc_steinhart_hart(handle, ohm);                   /* convert */
        
        return 0;                                                               /* success return 0 */
    }
    else                                                                        /* lookup table */
    {
        if (handle->table_len > 0)                                              /* check table length */
        {
            if (ohm > handle->table[0].ohm)                                     /* check range */
            {
                handle->debug_print("ntc: out of table.\n");                    /* out of table */
                
                return 1;                                                       /* return error */
            }
            
            if (ohm < handle->table[handle->table_len - 1].ohm)                 /* check range */
            {
                handle->debug_print("ntc: out of table.\n");                    /* out of table */
                
                return 1;                                                       /* return error */
            }
            
            *degrees_celsius = a_ntc_table(handle, ohm);                        /* convert */
                
            return 0;                                                           /* success return 0 */
        }
        else
        {
            handle->debug_print("ntc: table length is 0.\n");                   /* table length is 0 */
            
            return 2;                                                           /* return error */
        }
    }
}

/**
 * @brief      ntc filter
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[in]  input_len input length
 * @param[out] *output pointer to an output buffer
 * @return     status code
 *             - 0 success
 *             - 1 filter param invalid
 * @note       none
 */
static uint8_t a_ntc_filter(ntc_handle_t *handle, uint16_t input_len, float *output)
{
    if (handle->filter == (uint8_t)NTC_FILTER_NONE)                                        /* no filter */
    {
        if (input_len != 1)                                                                /* check the length */
        {
            handle->debug_print("ntc: no filter length must be 1.\n");                     /* no filter length must be 1 */
            
            return 1;                                                                      /* return error */
        }
        *output = handle->buf_flt[0];                                                      /* set output */
        
        return 0;                                                                          /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_FIRST_ORDER_LAG)                        /* first order lag filter */
    {
        int32_t i;
        float sum;
        float sum2;
        
        if (input_len != 1)                                                                /* check the length */
        {
            handle->debug_print("ntc: first order lag filter length must be 1.\n");        /* first order lag filter length must be 1 */
            
            return 1;                                                                      /* return error */
        }
        
        if (handle->filter_pointer < handle->filter_len)                                   /* not full */
        {
            handle->cache_flt[handle->filter_pointer] = handle->buf_flt[0];                /* push one */
            handle->filter_pointer++;                                                      /* filter pointer++ */
        }
        else
        {
            for (i = 0; i < handle->filter_pointer - 1; i++)                               /* length - 1 */
            {
                handle->cache_flt[i] = handle->cache_flt[i + 1];                           /* exchange */
            }
            handle->cache_flt[handle->filter_pointer - 1] = handle->buf_flt[0];            /* push to the last */
        }
        
        sum = 0.0f;                                                                        /* init 0.0f */
        sum2 = 0.0f;                                                                       /* init 0.0f */
        for (i = 0; i < handle->filter_pointer; i++)                                       /* sum all */
        {
            sum += handle->cache_flt[i] * handle->param_flt[i];                            /* sum all */
            sum2 += handle->param_flt[i];                                                  /* sum all */
        }
        *output = sum / sum2;                                                              /* set the output */
        handle->cache_flt[handle->filter_pointer - 1] = *output;                           /* save */
        
        return 0;                                                                          /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_MEDIAN)                                 /* median filter */
    {
        int32_t i;
        int32_t j;
        float temp;
        
        if ((input_len % 2) == 0)                                                          /* check the length */
        {
            handle->debug_print("ntc: median filter length must be odd number.\n");        /* median filter length must be odd number */
            
            return 1;                                                                      /* return error */
        }
        if (input_len < 3)                                                                 /* check the length */
        {
            handle->debug_print("ntc: median filter length < 3.\n");                       /* median filter length < 3 */
            
            return 1;                                                                      /* return error */
        }
        
        for (j = 0; j < input_len - 1; j++)                                                /* 1 layer loop */
        {
            for (i = 0; i < input_len - 1 - j; i++)                                        /* 2 layer loop */
            {
                if (handle->buf_flt[i] > handle->buf_flt[i + 1])                           /* choose the bigger */
                {
                    temp = handle->buf_flt[i];                                             /* exchange */
                    handle->buf_flt[i] = handle->buf_flt[i + 1];                           /* exchange */
                    handle->buf_flt[i + 1] = temp;                                         /* exchange */
                }
            }
        }
        
        *output = handle->buf_flt[(input_len - 1) / 2];                                    /* set middle value */
        
        return 0;                                                                          /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_ANTI_SPIKE_AVERAGE)                     /* anti spike average filter */
    {
        int32_t i;
        int32_t j;
        float temp;
        float sum;
        
        if (input_len < 3)                                                                 /* check the length */
        {
            handle->debug_print("ntc: anti spike average filter < 3.\n");                  /* anti spike average filter < 3 */
            
            return 1;                                                                      /* return error */
        }
        
        for (j = 0; j < input_len - 1; j++)                                                /* 1 layer loop */
        {
            for (i = 0; i < input_len - 1 - j; i++)                                        /* 2 layer loop */
            {
                if (handle->buf_flt[i] > handle->buf_flt[i + 1])                           /* choose the bigger */
                {
                    temp = handle->buf_flt[i];                                             /* exchange */
                    handle->buf_flt[i] = handle->buf_flt[i + 1];                           /* exchange */
                    handle->buf_flt[i + 1] = temp;                                         /* exchange */
                }
            }
        }
        
        sum = 0.0f;                                                                        /* init 0.0f */
        for (i = 1; i < input_len - 1; i++)                                                /* remove the first and the last */
        {
            sum += handle->buf_flt[i];                                                     /* sum all */
        }
        *output = sum / (float)(input_len - 2);                                            /* set the output */
        
        return 0;                                                                          /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_MOVING_AVERAGE)                         /* moving average filter */
    {
        int32_t i;
        float sum;
        
        if (input_len != 1)                                                                /* check the length */
        {
            handle->debug_print("ntc: moving average filter length must be 1.\n");         /* moving average filter length must be 1 */
            
            return 1;                                                                      /* return error */
        }
        
        if (handle->filter_pointer < handle->filter_len)                                   /* not full */
        {
            handle->cache_flt[handle->filter_pointer] = handle->buf_flt[0];                /* push one */
            handle->filter_pointer++;                                                      /* filter pointer++ */
        }
        else
        {
            for (i = 0; i < handle->filter_pointer - 1; i++)                               /* length - 1 */
            {
                handle->cache_flt[i] = handle->cache_flt[i + 1];                           /* exchange */
            }
            handle->cache_flt[handle->filter_pointer - 1] = handle->buf_flt[0];            /* push to the last */
        }
        
        sum = 0.0f;                                                                        /* init 0.0f */
        for (i = 0; i < handle->filter_pointer; i++)                                       /* sum all */
        {
            sum += handle->cache_flt[i];                                                   /* sum all */
        }
        *output = sum / (float)(handle->filter_pointer);                                   /* set the output */
        
        return 0;                                                                          /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_WEIGHTED_MOVING_AVERAGE)                /* weighted moving average filter */
    {
        int32_t i;
        float sum;
        float sum2;
        
        if (input_len != 1)                                                                /* check the length */
        {
            handle->debug_print("ntc: moving average filter length must be 1.\n");         /* moving average filter length must be 1 */
            
            return 1;                                                                      /* return error */
        }
        
        if (handle->filter_pointer < handle->filter_len)                                   /* not full */
        {
            handle->cache_flt[handle->filter_pointer] = handle->buf_flt[0];                /* push one */
            handle->filter_pointer++;                                                      /* filter pointer++ */
        }
        else
        {
            for (i = 0; i < handle->filter_pointer - 1; i++)                               /* length - 1 */
            {
                handle->cache_flt[i] = handle->cache_flt[i + 1];                           /* exchange */
            }
            handle->cache_flt[handle->filter_pointer - 1] = handle->buf_flt[0];            /* push to the last */
        }
        
        sum = 0.0f;                                                                        /* init 0.0f */
        sum2 = 0.0f;                                                                       /* init 0.0f */
        for (i = 0; i < handle->filter_pointer; i++)                                       /* sum all */
        {
            sum += handle->cache_flt[i] * handle->param_flt[i];                            /* sum all */
            sum2 += handle->param_flt[i];                                                  /* sum all */
        }
        *output = sum / sum2;                                                              /* set the output */
        
        return 0;                                                                          /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_LIMITING)                               /* limiting filter */
    {
        int32_t i;
        float deg1;
        float deg2;
        
        if (input_len != 1)                                                                /* check the length */
        {
            handle->debug_print("ntc: filter param invalid.\n");                           /* filter param invalid */
            
            return 1;                                                                      /* return error */
        }
        
        if (handle->filter_pointer < handle->filter_len)                                   /* not full */
        {
            handle->cache_flt[handle->filter_pointer] = handle->buf_flt[0];                /* push one */
            handle->filter_pointer++;                                                      /* filter pointer++ */
        }
        else
        {
            for (i = 0; i < handle->filter_pointer - 1; i++)                               /* length - 1 */
            {
                handle->cache_flt[i] = handle->cache_flt[i + 1];                           /* exchange */
            }
            handle->cache_flt[handle->filter_pointer - 1] = handle->buf_flt[0];            /* push to the last */
        }
        
        if (a_ntc_convert(handle, handle->cache_flt[0], &deg1) != 0)                       /* convert degrees */
        {
            return 1;                                                                      /* return error */
        }
        if (a_ntc_convert(handle, handle->buf_flt[0], &deg2) != 0)                         /* convert degrees */
        {
            return 1;                                                                      /* return error */
        }
        if (fabsf(deg1 - deg2) > handle->param_flt[0])                                     /* set limit */
        {
            *output = handle->cache_flt[0];                                                /* set the last one */
            handle->cache_flt[1] = handle->cache_flt[0];                                   /* save the data */
        }
        else
        {
            *output = handle->buf_flt[0];                                                  /* set output */
        }
        
        return 0;                                                                          /* success return 0 */
    }
    else                                                                                   /* kalman filter */
    {
        if (input_len != 1)                                                                /* check the length */
        {
            handle->debug_print("ntc: filter param invalid.\n");                           /* filter param invalid */
            
            return 1;                                                                      /* return error */
        }
        
        handle->cache_flt[1] = handle->cache_flt[1] + handle->param_flt[0];                /* predict */
        handle->cache_flt[2] = handle->cache_flt[1] / 
                              (handle->cache_flt[1] + handle->param_flt[1]);               /* update */
        handle->cache_flt[0] = handle->cache_flt[0] + 
                               handle->cache_flt[2] * 
                              (handle->buf_flt[0] - handle->cache_flt[0]);                 /* correct */
        handle->cache_flt[1] = (1.0f - handle->cache_flt[2]) * handle->cache_flt[1];       /* correct */
        
        *output = handle->cache_flt[0];                                                    /* set output */
        
        return 0;                                                                          /* success return 0 */
    }
}

/**
 * @brief      ntc adc read
 * @param[in]  *handle pointer to a ntc handle structure
 * @param[in]  len adc read length
 * @param[out] *open_short_circuit pointer to an open short circuit buffer
 * @return     status code
 *             - 0 success
 *             - 2 open or short circuit
 *             - 3 invalid circuit type
 * @note       none
 */
static uint8_t a_ntc_adc(ntc_handle_t *handle, uint16_t len, uint8_t *open_short_circuit)
{
    uint8_t res;
    uint16_t i;
    
    res = handle->adc_read(handle->buf, len);                              /* adc read */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntc: adc read failed.\n");                    /* adc read failed */
        *open_short_circuit = 0;                                           /* init 0 */
        
        return 1;                                                          /* return error */
    }
    
    if (handle->circuit == (uint8_t)NTC_CIRCUIT_VCC_NTC_R_GND)             /* vcc -> ntc -> r_fixed -> gnd */
    {
        for (i = 0; i < len; i++)                                          /* convert all */
        {
            if (handle->buf[i] == 0)                                       /* check open circuit */
            {
                *open_short_circuit = 1;                                   /* set open circuit */
                handle->debug_print("ntc: open circuit.\n");               /* open circuit */
                
                return 2;                                                  /* return error */
            }
            else if (handle->buf[i] >= handle->vcc_counter)                /* check short circuit */
            {
                *open_short_circuit = 2;                                   /* set short circuit */
                handle->debug_print("ntc: short circuit.\n");              /* short circuit */
                
                return 2;                                                  /* return error */
            }
            else
            {
                handle->buf_flt[i] = handle->r_fixed_ohm * 
                                     (((float)handle->vcc_counter /
                                      (float)handle->buf[i]) - 1.0f);      /* convert */
            }
        }
        *open_short_circuit = 0;                                           /* init 0 */
        
        return 0;                                                          /* success return 0 */
    }
    else if (handle->circuit == (uint8_t)NTC_CIRCUIT_VCC_R_NTC_GND)        /* vcc -> r_fixed -> ntc -> gnd */
    {
        for (i = 0; i < len; i++)                                          /* convert all */
        {
            if (handle->buf[i] == 0)                                       /* check short circuit */
            {
                *open_short_circuit = 2;                                   /* set short circuit */
                handle->debug_print("ntc: short circuit.\n");              /* short circuit */
                
                return 2;                                                  /* return error */
            }
            else if (handle->buf[i] >= handle->vcc_counter)                /* check open circuit */
            {
                *open_short_circuit = 1;                                   /* set open circuit */
                handle->debug_print("ntc: open circuit.\n");               /* open circuit */
                
                return 2;                                                  /* return error */
            }
            else
            {
                handle->buf_flt[i] = handle->r_fixed_ohm / 
                                     (((float)handle->vcc_counter / 
                                      (float)handle->buf[i])- 1.0f);       /* convert */
            }
        }
        *open_short_circuit = 0;                                           /* init 0 */
        
        return 0;                                                          /* success return 0 */
    }
    else
    {
        handle->debug_print("ntc: invalid circuit type.\n");               /* adc invalid circuit type */
        *open_short_circuit = 0;                                           /* init 0 */
        
        return 3;                                                          /* return error */
    }
}

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
uint8_t ntc_set_circuit(ntc_handle_t *handle, ntc_circuit_t circuit)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    if (handle->debug_print == NULL)             /* check debug_print */
    {
        return 3;                                /* return error */
    }
    
    handle->circuit = (uint8_t)(circuit);        /* set circuit */
    
    return 0;                                    /* success return 0 */
}

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
uint8_t ntc_get_circuit(ntc_handle_t *handle, ntc_circuit_t *circuit)
{
    if (handle == NULL)                               /* check handle */
    {
        return 2;                                     /* return error */
    }
    if (handle->debug_print == NULL)                  /* check debug_print */
    {
        return 3;                                     /* return error */
    }
    
    *circuit = (ntc_circuit_t)handle->circuit;        /* set circuit */
    
    return 0;                                         /* success return 0 */
}

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
uint8_t ntc_set_circuit_fixed_resistor(ntc_handle_t *handle, float r_fixed_ohm)
{
    if (handle == NULL)                       /* check handle */
    {
        return 2;                             /* return error */
    }
    if (handle->debug_print == NULL)          /* check debug_print */
    {
        return 3;                             /* return error */
    }
    
    handle->r_fixed_ohm = r_fixed_ohm;        /* set resistor */
    
    return 0;                                 /* success return 0 */
}

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
uint8_t ntc_get_circuit_fixed_resistor(ntc_handle_t *handle, float *r_fixed_ohm)
{
    if (handle == NULL)                        /* check handle */
    {
        return 2;                              /* return error */
    }
    if (handle->debug_print == NULL)           /* check debug_print */
    {
        return 3;                              /* return error */
    }
    
    *r_fixed_ohm = handle->r_fixed_ohm;        /* set resistor */
    
    return 0;                                  /* success return 0 */
}

/**
 * @brief set algorithm
 */
uint8_t ntc_set_algorithm(ntc_handle_t *handle, ntc_algorithm_t algorithm)
{
    if (handle == NULL)                              /* check handle */
    {
        return 2;                                    /* return error */
    }
    if (handle->debug_print == NULL)                 /* check debug_print */
    {
        return 3;                                    /* return error */
    }
    
    handle->algorithm = (uint8_t)(algorithm);        /* set algorithm */
    
    return 0;                                        /* success return 0 */
}

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
uint8_t ntc_get_algorithm(ntc_handle_t *handle, ntc_algorithm_t *algorithm)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->debug_print == NULL)                        /* check debug_print */
    {
        return 3;                                           /* return error */
    }
    
    *algorithm = (ntc_algorithm_t)handle->algorithm;        /* set algorithm */
    
    return 0;                                               /* success return 0 */
}

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
uint8_t ntc_set_algorithm_beta_formula_beta_value(ntc_handle_t *handle, float beta)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->debug_print == NULL)        /* check debug_print */
    {
        return 3;                           /* return error */
    }
    
    handle->beta = beta;                    /* set beta formula beta value */
    
    return 0;                               /* success return 0 */
}

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
uint8_t ntc_get_algorithm_beta_formula_beta_value(ntc_handle_t *handle, float *beta)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->debug_print == NULL)        /* check debug_print */
    {
        return 3;                           /* return error */
    }
    
    *beta = handle->beta;                   /* set beta formula beta value */
    
    return 0;                               /* success return 0 */
}

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
uint8_t ntc_set_algorithm_beta_formula_r25_ohm(ntc_handle_t *handle, float r25_ohm)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->debug_print == NULL)        /* check debug_print */
    {
        return 3;                           /* return error */
    }
    
    handle->r25_ohm = r25_ohm;              /* set r25 ohm */
    
    return 0;                               /* success return 0 */
}

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
uint8_t ntc_get_algorithm_beta_formula_r25_ohm(ntc_handle_t *handle, float *r25_ohm)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->debug_print == NULL)        /* check debug_print */
    {
        return 3;                           /* return error */
    }
    
    *r25_ohm = handle->r25_ohm;             /* set r25 ohm */
    
    return 0;                               /* success return 0 */
}

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
uint8_t ntc_set_algorithm_steinhart_hart(ntc_handle_t *handle, double a, double b, double c)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->debug_print == NULL)        /* check debug_print */
    {
        return 3;                           /* return error */
    }
    
    handle->a = a;                          /* set a */
    handle->b = b;                          /* set b */
    handle->c = c;                          /* set c */
    
    return 0;                               /* success return 0 */
}

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
uint8_t ntc_get_algorithm_steinhart_hart(ntc_handle_t *handle, double *a, double *b, double *c)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->debug_print == NULL)        /* check debug_print */
    {
        return 3;                           /* return error */
    }
    
    *a = handle->a;                         /* set a */
    *b = handle->b;                         /* set b */
    *c = handle->c;                         /* set c */
    
    return 0;                               /* success return 0 */
}

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
uint8_t ntc_load_algorithm_lookup_table(ntc_handle_t *handle, const ntc_table_t *table, uint16_t table_len)
{
    uint16_t i;
    float degrees_celsius;
    float ohm;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->debug_print == NULL)                                                       /* check debug_print */
    {
        return 3;                                                                          /* return error */
    }
    if (table_len == 0)                                                                    /* check the table length */
    {
        handle->debug_print("ntc: table_len is 0.\n");                                     /* table_len is 0 */
        
        return 4;                                                                          /* return error */
    }
    
    ohm = table[0].ohm;                                                                    /* set ohm */
    degrees_celsius = table[0].degrees_celsius;                                            /* set degrees celsius */
    for (i = 1; i < table_len; i++)                                                        /* check table */
    {
        if (table[i].ohm > ohm)                                                            /* check table ohm */
        {
            handle->debug_print("ntc: table[%d] ohm is invalid.\n", i);                    /* table ohm is invalid */
            
            return 5;                                                                      /* return error */
        }
        if (table[i].degrees_celsius < degrees_celsius)                                    /* check degrees celsius */
        {
            handle->debug_print("ntc: table[%d] degrees celsius is invalid.\n", i);        /* table degrees celsius is invalid */
            
            return 6;                                                                      /* return error */
        }
        
        ohm = table[i].ohm;                                                                /* set ohm */
        degrees_celsius = table[i].degrees_celsius;                                        /* set degrees celsius */
    }
    handle->table = table;                                                                 /* set table pointer */
    handle->table_len = table_len;                                                         /* set table length */
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t ntc_set_filter(ntc_handle_t *handle, ntc_filter_t filter)
{
    if (handle == NULL)                        /* check handle */
    {
        return 2;                              /* return error */
    }
    if (handle->debug_print == NULL)           /* check debug_print */
    {
        return 3;                              /* return error */
    }
    
    handle->filter = (uint8_t)(filter);        /* set filter */
    
    return 0;                                  /* success return 0 */
}

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
uint8_t ntc_get_filter(ntc_handle_t *handle, ntc_filter_t *filter)
{
    if (handle == NULL)                            /* check handle */
    {
        return 2;                                  /* return error */
    }
    if (handle->debug_print == NULL)               /* check debug_print */
    {
        return 3;                                  /* return error */
    }
    
    *filter = (ntc_filter_t)handle->filter;        /* set filter */
    
    return 0;                                      /* success return 0 */
}

/**
 * @brief set filter first order lag
 */
uint8_t ntc_set_filter_first_order_lag(ntc_handle_t *handle, float alpha)
{
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->debug_print == NULL)                                      /* check debug_print */
    {
        return 3;                                                         /* return error */
    }
    if (NTC_FILTER_BUFFER_SIZE < 2)                                       /* check buffer length */
    {
        handle->debug_print("ntc: NTC_FILTER_BUFFER_SIZE < 2.\n");        /* NTC_FILTER_BUFFER_SIZE < 2 */
        
        return 4;                                                         /* return error */
    }
    if (alpha < 0.0f)                                                     /* check alpha */
    {
        handle->debug_print("ntc: alpha < 0.0f.\n");                      /* alpha < 0.0f */
        
        return 5;                                                         /* return error */
    }
    if (alpha > 1.0f)                                                     /* check alpha */
    {
        handle->debug_print("ntc: alpha > 1.0f.\n");                      /* alpha > 1.0f */
        
        return 6;                                                         /* return error */
    }
    
    handle->filter_len = 2;                                               /* set filter length 2 */
    handle->filter_pointer = 0;                                           /* init 0 */
    handle->param_flt[0] = 1.0f - alpha;                                  /* set param 0 */
    handle->param_flt[1] = alpha;                                         /* set param 1 */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief set filter median length
 */
uint8_t ntc_set_filter_median_length(ntc_handle_t *handle, uint16_t length)
{
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->debug_print == NULL)                                               /* check debug_print */
    {
        return 3;                                                                  /* return error */
    }
    if (NTC_FILTER_BUFFER_SIZE < length)                                           /* check buffer length */
    {
        handle->debug_print("ntc: NTC_FILTER_BUFFER_SIZE < %d.\n", length);        /* NTC_FILTER_BUFFER_SIZE < length */
        
        return 4;                                                                  /* return error */
    }
    if ((length % 2) == 0)                                                         /* check the length */
    {
        handle->debug_print("ntc: median filter length must be odd number.\n");    /* median filter length must be odd number */
        
        return 5;                                                                  /* return error */
    }
    if (length < 3)                                                                /* check the length */
    {
        handle->debug_print("ntc: length < 3.\n");                                 /* length < 3 */
        
        return 6;                                                                  /* return error */
    }
    
    handle->filter_len = length;                                                   /* set filter length */
    handle->filter_pointer = 0;                                                    /* init 0 */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief set filter anti spike average length
 */
uint8_t ntc_set_filter_anti_spike_average_length(ntc_handle_t *handle, uint16_t length)
{
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->debug_print == NULL)                                               /* check debug_print */
    {
        return 3;                                                                  /* return error */
    }
    if (NTC_FILTER_BUFFER_SIZE < length)                                           /* check buffer length */
    {
        handle->debug_print("ntc: NTC_FILTER_BUFFER_SIZE < %d.\n", length);        /* NTC_FILTER_BUFFER_SIZE < length */
        
        return 4;                                                                  /* return error */
    }
    if (length < 3)                                                                /* check the length */
    {
        handle->debug_print("ntc: length < 3.\n");                                 /* length < 3 */
        
        return 5;                                                                  /* return error */
    }
    
    handle->filter_len = length;                                                   /* set filter length */
    handle->filter_pointer = 0;                                                    /* init 0 */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief set filter moving average length
 */
uint8_t ntc_set_filter_moving_average_length(ntc_handle_t *handle, uint16_t length)
{
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->debug_print == NULL)                                               /* check debug_print */
    {
        return 3;                                                                  /* return error */
    }
    if (NTC_FILTER_BUFFER_SIZE < length)                                           /* check buffer length */
    {
        handle->debug_print("ntc: NTC_FILTER_BUFFER_SIZE < %d.\n", length);        /* NTC_FILTER_BUFFER_SIZE < length */
        
        return 4;                                                                  /* return error */
    }
    if (length == 0)                                                               /* check the length */
    {
        handle->debug_print("ntc: length is 0.\n");                                /* length is 0 */
        
        return 5;                                                                  /* return error */
    }
    
    handle->filter_len = length;                                                   /* set filter length */
    handle->filter_pointer = 0;                                                    /* init 0 */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief set filter weighted moving average length
 */
uint8_t ntc_set_filter_weighted_moving_average_length(ntc_handle_t *handle, float *weight, uint16_t length)
{
    uint16_t i;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->debug_print == NULL)                                               /* check debug_print */
    {
        return 3;                                                                  /* return error */
    }
    if (NTC_FILTER_BUFFER_SIZE < length)                                           /* check buffer length */
    {
        handle->debug_print("ntc: NTC_FILTER_BUFFER_SIZE < %d.\n", length);        /* NTC_FILTER_BUFFER_SIZE < length */
        
        return 4;                                                                  /* return error */
    }
    if (length == 0)                                                               /* check the length */
    {
        handle->debug_print("ntc: length is 0.\n");                                /* length is 0 */
        
        return 5;                                                                  /* return error */
    }
    
    for (i = 0; i < length; i++)                                                   /* save all */
    {
        handle->param_flt[i] = weight[i];                                          /* set weight */
    }
    handle->filter_len = length;                                                   /* set filter length */
    handle->filter_pointer = 0;                                                    /* init 0 */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief set filter limiting
 */
uint8_t ntc_set_filter_limiting(ntc_handle_t *handle, float degrees_celsius)
{
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->debug_print == NULL)                                      /* check debug_print */
    {
        return 3;                                                         /* return error */
    }
    if (NTC_FILTER_BUFFER_SIZE < 2)                                       /* check buffer length */
    {
        handle->debug_print("ntc: NTC_FILTER_BUFFER_SIZE < 2.\n");        /* NTC_FILTER_BUFFER_SIZE < 2 */
        
        return 4;                                                         /* return error */
    }
    
    handle->filter_len = 2;                                               /* set filter length 2 */
    handle->filter_pointer = 0;                                           /* init 0 */
    handle->param_flt[0] = degrees_celsius;                               /* set degrees celsius */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief set filter kalman
 */
uint8_t ntc_set_filter_kalman(ntc_handle_t *handle,
                              float q_process_noise_covariance,
                              float r_measurement_noise_covariance,
                              float p_estimation_error_covariance,
                              float x_estimated_value)
{
    uint8_t res;
    uint8_t open_short_circuit;
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->debug_print == NULL)                                      /* check debug_print */
    {
        return 3;                                                         /* return error */
    }
    if (NTC_FILTER_BUFFER_SIZE < 3)                                       /* check buffer length */
    {
        handle->debug_print("ntc: NTC_FILTER_BUFFER_SIZE < 3.\n");        /* NTC_FILTER_BUFFER_SIZE < 3 */
        
        return 4;                                                         /* return error */
    }
    if (isnan(x_estimated_value) != 0)                                    /* check isnan */
    {
        res = a_ntc_adc(handle, 1, &open_short_circuit);                  /* read adc */
        if (res != 0)                                                     /* check the result */
        {
            return 5;                                                     /* return error */
        }
        x_estimated_value = handle->buf_flt[0];                           /* set x estimated value */
    }
    
    handle->filter_len = 1;                                               /* set filter length 2 */
    handle->filter_pointer = 0;                                           /* init 0 */
    handle->param_flt[0] = q_process_noise_covariance;                    /* q, process noise covariance */
    handle->param_flt[1] = r_measurement_noise_covariance;                /* r, measurement noise covariance */
    handle->cache_flt[0] = x_estimated_value;                             /* x, estimated value */
    handle->cache_flt[1] = p_estimation_error_covariance;                 /* p, measurement noise covariance */
    handle->cache_flt[2] = 0.0f;                                          /* k, kalman gain */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief     reset the filter
 * @param[in] *handle pointer to a ntc handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntc_reset_filter(ntc_handle_t *handle)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->debug_print == NULL)        /* check debug_print */
    {
        return 3;                           /* return error */
    }
    
    handle->filter_pointer = 0;             /* init 0 */
    
    return 0;                               /* success return 0 */
}

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
uint8_t ntc_init(ntc_handle_t *handle)
{
    uint8_t res;
    uint16_t i;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->debug_print == NULL)                                          /* check debug_print */
    {
        return 3;                                                             /* return error */
    }
    if (handle->adc_init == NULL)                                             /* check adc_init */
    {
        handle->debug_print("ntc: adc_init is null.\n");                      /* adc_init is null */
        
        return 3;                                                             /* return error */
    }
    if (handle->adc_deinit == NULL)                                           /* check adc_deinit */
    {
        handle->debug_print("ntc: adc_deinit is null.\n");                    /* adc_deinit is null */
        
        return 3;                                                             /* return error */
    }
    if (handle->adc_read == NULL)                                             /* check adc_read */
    {
        handle->debug_print("ntc: adc_read is null.\n");                      /* adc_read is null */
        
        return 3;                                                             /* return error */
    }
    if (handle->delay_ms == NULL)                                             /* check delay_ms */
    {
        handle->debug_print("ntc: delay_ms is null.\n");                      /* delay_ms is null */
        
        return 3;                                                             /* return error */
    }
    
    res = handle->adc_init(&handle->vcc_counter);                             /* adc init */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("ntc: adc init failed.\n");                       /* adc init failed */
        
        return 1;                                                             /* return error */
    }
    if (handle->vcc_counter == 0)                                             /* check vcc counter */
    {
        handle->debug_print("ntc: vcc counter is invalid.\n");                /* vcc counter is invalid */
        (void)handle->adc_deinit();                                           /* adc deinit */
        
        return 4;                                                             /* return error */
    }
    handle->circuit = (uint8_t)NTC_CIRCUIT_VCC_R_NTC_GND;                     /* init vcc -> r_fixed -> ntc -> gnd */
    handle->r_fixed_ohm = 1.0f;                                               /* init 1.0f */
    handle->algorithm = (uint8_t)NTC_ALGORITHM_BETA_FORMULA;                  /* init beta formula */
    handle->beta = 1.0f;                                                      /* init 1.0f */
    handle->r25_ohm = 1.0f;                                                   /* init 1.0f */
    handle->a = 1.0;                                                          /* init 1.0 */
    handle->b = 1.0;                                                          /* init 1.0 */
    handle->c = 1.0;                                                          /* init 1.0 */
    handle->table = NULL;                                                     /* init null */
    handle->table_len = 0;                                                    /* init 0 */
    handle->filter = (uint8_t)NTC_FILTER_NONE;                                /* init 0 */
    for(i = 0; i < NTC_FILTER_BUFFER_SIZE; i++)                               /* init all */
    {
        handle->buf[i] = 0;                                                   /* init 0 */
        handle->buf_flt[i] = 0.0f;                                            /* init 0 */
        handle->cache_flt[i] = 1.0f;                                          /* init 1.0f */
        handle->param_flt[i] = 1.0f;                                          /* init 1.0f */
    }
    handle->filter_len = 0;                                                   /* init 0 */
    handle->filter_pointer = 0;                                               /* init 0 */
    handle->inited = 1;                                                       /* flag inited */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t ntc_deinit(ntc_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
    
    res = handle->adc_deinit();                                  /* adc deinit */
    if (res != 0)                                                /* check the result */
    {
        handle->debug_print("ntc: adc deinit failed.\n");        /* adc deinit failed */
        
        return 1;                                                /* return error */
    }
    handle->inited = 0;                                          /* flag close */
    
    return 0;                                                    /* success return 0 */
}

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
uint8_t ntc_read_temperature(ntc_handle_t *handle, float *ohm, float *degrees_celsius)
{
    uint8_t res;
    uint8_t open_short_circuit;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    if (handle->filter == (uint8_t)NTC_FILTER_NONE)                                /* no filter */
    {
        res = a_ntc_adc(handle, 1, &open_short_circuit);                           /* read adc */
        if (res != 0)                                                              /* check the result */
        {
            if (open_short_circuit != 0)                                           /* check open short circuit */
            {
                if (open_short_circuit == 1)                                       /* check type */
                {
                    return 4;                                                      /* return error */
                }
                else
                {
                    return 5;                                                      /* return error */
                }
            }
            else
            {
                return 1;                                                          /* return error */
            }
        }
        
        res = a_ntc_filter(handle, 1, ohm);                                        /* run the filter */
        if (res != 0)                                                              /* check the result */
        {
            return 6;                                                              /* return error */
        }
        
        res = a_ntc_convert(handle, *ohm, degrees_celsius);                        /* ntc convert */
        if (res != 0)                                                              /* check the result */
        {
            return 7;                                                              /* return error */
        }
        
        return 0;                                                                  /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_FIRST_ORDER_LAG)                /* first order lag filter */
    {
        res = a_ntc_adc(handle, 1, &open_short_circuit);                           /* read adc */
        if (res != 0)                                                              /* check the result */
        {
            if (open_short_circuit != 0)                                           /* check open short circuit */
            {
                if (open_short_circuit == 1)                                       /* check type */
                {
                    return 4;                                                      /* return error */
                }
                else
                {
                    return 5;                                                      /* return error */
                }
            }
            else
            {
                return 1;                                                          /* return error */
            }
        }
        
        res = a_ntc_filter(handle, 1, ohm);                                        /* run the filter */
        if (res != 0)                                                              /* check the result */
        {
            return 6;                                                              /* return error */
        }
        
        res = a_ntc_convert(handle, *ohm, degrees_celsius);                        /* ntc convert */
        if (res != 0)                                                              /* check the result */
        {
            return 7;                                                              /* return error */
        }
        
        return 0;                                                                  /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_MEDIAN)                         /* median filter */
    {
        res = a_ntc_adc(handle, handle->filter_len, &open_short_circuit);          /* read adc */
        if (res != 0)                                                              /* check the result */
        {
            if (open_short_circuit != 0)                                           /* check open short circuit */
            {
                if (open_short_circuit == 1)                                       /* check type */
                {
                    return 4;                                                      /* return error */
                }
                else
                {
                    return 5;                                                      /* return error */
                }
            }
            else
            {
                return 1;                                                          /* return error */
            }
        }
        
        res = a_ntc_filter(handle, handle->filter_len, ohm);                       /* run the filter */
        if (res != 0)                                                              /* check the result */
        {
            return 6;                                                              /* return error */
        }
        
        res = a_ntc_convert(handle, *ohm, degrees_celsius);                        /* ntc convert */
        if (res != 0)                                                              /* check the result */
        {
            return 7;                                                              /* return error */
        }
        
        return 0;                                                                  /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_ANTI_SPIKE_AVERAGE)             /* anti spike average filter */
    {
        res = a_ntc_adc(handle, handle->filter_len, &open_short_circuit);          /* read adc */
        if (res != 0)                                                              /* check the result */
        {
            if (open_short_circuit != 0)                                           /* check open short circuit */
            {
                if (open_short_circuit == 1)                                       /* check type */
                {
                    return 4;                                                      /* return error */
                }
                else
                {
                    return 5;                                                      /* return error */
                }
            }
            else
            {
                return 1;                                                          /* return error */
            }
        }
        
        res = a_ntc_filter(handle, handle->filter_len, ohm);                       /* run the filter */
        if (res != 0)                                                              /* check the result */
        {
            return 6;                                                              /* return error */
        }
        
        res = a_ntc_convert(handle, *ohm, degrees_celsius);                        /* ntc convert */
        if (res != 0)                                                              /* check the result */
        {
            return 7;                                                              /* return error */
        }
        
        return 0;                                                                  /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_MOVING_AVERAGE)                 /* moving average filter */
    {
        res = a_ntc_adc(handle, 1, &open_short_circuit);                           /* read adc */
        if (res != 0)                                                              /* check the result */
        {
            if (open_short_circuit != 0)                                           /* check open short circuit */
            {
                if (open_short_circuit == 1)                                       /* check type */
                {
                    return 4;                                                      /* return error */
                }
                else
                {
                    return 5;                                                      /* return error */
                }
            }
            else
            {
                return 1;                                                          /* return error */
            }
        }
        
        res = a_ntc_filter(handle, 1, ohm);                                        /* run the filter */
        if (res != 0)                                                              /* check the result */
        {
            return 6;                                                              /* return error */
        }
        
        res = a_ntc_convert(handle, *ohm, degrees_celsius);                        /* ntc convert */
        if (res != 0)                                                              /* check the result */
        {
            return 7;                                                              /* return error */
        }
        
        return 0;                                                                  /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_WEIGHTED_MOVING_AVERAGE)        /* weighted moving average filter */
    {
        res = a_ntc_adc(handle, 1, &open_short_circuit);                           /* read adc */
        if (res != 0)                                                              /* check the result */
        {
            if (open_short_circuit != 0)                                           /* check open short circuit */
            {
                if (open_short_circuit == 1)                                       /* check type */
                {
                    return 4;                                                      /* return error */
                }
                else
                {
                    return 5;                                                      /* return error */
                }
            }
            else
            {
                return 1;                                                          /* return error */
            }
        }
        
        res = a_ntc_filter(handle, 1, ohm);                                        /* run the filter */
        if (res != 0)                                                              /* check the result */
        {
            return 6;                                                              /* return error */
        }
        
        res = a_ntc_convert(handle, *ohm, degrees_celsius);                        /* ntc convert */
        if (res != 0)                                                              /* check the result */
        {
            return 7;                                                              /* return error */
        }
        
        return 0;                                                                  /* success return 0 */
    }
    else if (handle->filter == (uint8_t)NTC_FILTER_LIMITING)                       /* limiting filter */
    {
        res = a_ntc_adc(handle, 1, &open_short_circuit);                           /* read adc */
        if (res != 0)                                                              /* check the result */
        {
            if (open_short_circuit != 0)                                           /* check open short circuit */
            {
                if (open_short_circuit == 1)                                       /* check type */
                {
                    return 4;                                                      /* return error */
                }
                else
                {
                    return 5;                                                      /* return error */
                }
            }
            else
            {
                return 1;                                                          /* return error */
            }
        }
        
        res = a_ntc_filter(handle, 1, ohm);                                        /* run the filter */
        if (res != 0)                                                              /* check the result */
        {
            return 6;                                                              /* return error */
        }
        
        res = a_ntc_convert(handle, *ohm, degrees_celsius);                        /* ntc convert */
        if (res != 0)                                                              /* check the result */
        {
            return 7;                                                              /* return error */
        }
        
        return 0;                                                                  /* success return 0 */
    }
    else                                                                           /* kalman filter */
    {
        res = a_ntc_adc(handle, 1, &open_short_circuit);                           /* read adc */
        if (res != 0)                                                              /* check the result */
        {
            if (open_short_circuit != 0)                                           /* check open short circuit */
            {
                if (open_short_circuit == 1)                                       /* check type */
                {
                    return 4;                                                      /* return error */
                }
                else
                {
                    return 5;                                                      /* return error */
                }
            }
            else
            {
                return 1;                                                          /* return error */
            }
        }
        
        res = a_ntc_filter(handle, 1, ohm);                                        /* run the filter */
        if (res != 0)                                                              /* check the result */
        {
            return 6;                                                              /* return error */
        }
        
        res = a_ntc_convert(handle, *ohm, degrees_celsius);                        /* ntc convert */
        if (res != 0)                                                              /* check the result */
        {
            return 7;                                                              /* return error */
        }
        
        return 0;                                                                  /* success return 0 */
    }
}

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
uint8_t ntc_calculate_temperature(ntc_handle_t *handle, float ohm, float *degrees_celsius)
{
    uint8_t res;
    
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
    
    res = a_ntc_convert(handle, ohm, degrees_celsius);        /* ntc convert */
    if (res != 0)                                             /* check the result */
    {
        return 1;                                             /* return error */
    }
    
    return 0;                                                 /* success return 0 */
}

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
uint8_t ntc_calculate_temperature_with_filter(ntc_handle_t *handle, float *ohm, uint16_t len, float *degrees_celsius)
{
    uint8_t res;
    uint16_t i;
    
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    if (handle->inited != 1)                                   /* check handle initialization */
    {
        return 3;                                              /* return error */
    }
    
    for (i = 0; i < len; i++)                                  /* copy data */
    {
        handle->buf_flt[i] = ohm[i];                           /* copy data */
    }
    
    res = a_ntc_filter(handle, len, ohm);                      /* run the filter */
    if (res != 0)                                              /* check the result */
    {
        return 1;                                              /* return error */
    }
    
    res = a_ntc_convert(handle, *ohm, degrees_celsius);        /* ntc convert */
    if (res != 0)                                              /* check the result */
    {
        return 4;                                              /* return error */
    }
    
    return 0;                                                  /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a ntc info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ntc_info(ntc_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ntc_info_t));                            /* initialize ntc info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "ADC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
