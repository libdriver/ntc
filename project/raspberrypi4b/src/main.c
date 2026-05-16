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
 * @file      main.c
 * @brief     main source file
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
#include "driver_ntc_read_test.h"
#include "driver_ntc_table.h"
#include <getopt.h>
#include <stdlib.h>

/**
 * @brief     ntc full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ntc(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    char short_options[] = "hipe:t:";
    struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"algorithm", required_argument, NULL, 1},
        {"beta", required_argument, NULL, 2},
        {"circuit", required_argument, NULL, 3},
        {"fixed-resistor", required_argument, NULL, 4},
        {"filter", required_argument, NULL, 5},
        {"r25-resistor", required_argument, NULL, 6},
        {"resistor", required_argument, NULL, 7},
        {"times", required_argument, NULL, 8},
        {"steinhart-hart-a", required_argument, NULL, 9},
        {"steinhart-hart-b", required_argument, NULL, 10},
        {"steinhart-hart-c", required_argument, NULL, 11},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    float r_fixed_ohm = 10000.0f;
    float beta = 3950.0f;
    float r25_ohm = 10000.0f;
    float resistor_ohm[NTC_FILTER_BUFFER_SIZE];
    uint16_t resistor_len = 0;
    double steinhart_hart_a = 1.129148e-3;
    double steinhart_hart_b = 2.34125e-4;
    double steinhart_hart_c = 8.76741e-8;
    ntc_filter_t filter = NTC_FILTER_NONE;
    ntc_circuit_t circuit = NTC_CIRCUIT_VCC_R_NTC_GND;
    ntc_algorithm_t algorithm = NTC_ALGORITHM_BETA_FORMULA;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* algorithm */
            case 1 :
            {
                if (strcmp(optarg, "BETA-FORMULA") == 0)
                {
                    algorithm = NTC_ALGORITHM_BETA_FORMULA;
                }
                else if (strcmp(optarg, "STEINHART-HART") == 0)
                {
                    algorithm = NTC_ALGORITHM_STEINHART_HART;
                }
                else if (strcmp(optarg, "LOOKUP-TABLE-B3950-10K") == 0)
                {
                    algorithm = NTC_ALGORITHM_LOOKUP_TABLE;
                }
                else
                {
                    return 5;
                }

                break;
            }
            
            /* beta */
            case 2 :
            {
                beta = (float)atof(optarg);

                break;
            }
            
            /* circuit */
            case 3 :
            {
                if (strcmp(optarg, "VCC_NTC_R_GND") == 0)
                {
                    circuit = NTC_CIRCUIT_VCC_NTC_R_GND;
                }
                else if (strcmp(optarg, "VCC_R_NTC_GND") == 0)
                {
                    circuit = NTC_CIRCUIT_VCC_R_NTC_GND;
                }
                else
                {
                    return 5;
                }

                break;
            }
            
            /* fixed resistor */
            case 4 :
            {
                r_fixed_ohm = (float)atof(optarg);
                
                break;
            }
            
            /* filter */
            case 5 :
            {
                if (strcmp(optarg, "NONE") == 0)
                {
                    filter = NTC_FILTER_NONE;
                }
                else if (strcmp(optarg, "FIRST_ORDER_LAG") == 0)
                {
                    filter = NTC_FILTER_FIRST_ORDER_LAG;
                }
                else if (strcmp(optarg, "MEDIAN") == 0)
                {
                    filter = NTC_FILTER_MEDIAN;
                }
                else if (strcmp(optarg, "ANTI_SPIKE_AVERAGE") == 0)
                {
                    filter = NTC_FILTER_ANTI_SPIKE_AVERAGE;
                }
                else if (strcmp(optarg, "MOVING_AVERAGE") == 0)
                {
                    filter = NTC_FILTER_MOVING_AVERAGE;
                }
                else if (strcmp(optarg, "WEIGHTED_MOVING_AVERAGE") == 0)
                {
                    filter = NTC_FILTER_WEIGHTED_MOVING_AVERAGE;
                }
                else if (strcmp(optarg, "LIMITING") == 0)
                {
                    filter = NTC_FILTER_LIMITING;
                }
                else if (strcmp(optarg, "KALMAN") == 0)
                {
                    filter = NTC_FILTER_KALMAN;
                }
                else
                {
                    return 5;
                }

                break;
            }
            
            /* r25-resistor */
            case 6 :
            {
                r25_ohm = (float)atof(optarg);

                break;
            }
            
            /* resistor */
            case 7 :
            {
                char *token = strtok(optarg, ",");
                
                while (token != NULL)
                {
                    resistor_ohm[resistor_len++] = (float)atof(token);
                    if (resistor_len > NTC_FILTER_BUFFER_SIZE)
                    {
                        return 1;
                    }
                    token = strtok(NULL, ",");
                }
                
                break;
            }
            
            /* running times */
            case 8 :
            {
                /* set the times */
                times = atol(optarg);

                break;
            }
            
            /* steinhart hart a */
            case 9 :
            {
                /* set the times */
                steinhart_hart_a = atof(optarg);

                break;
            }
            
            /* steinhart hart b */
            case 10 :
            {
                /* set the times */
                steinhart_hart_b = atof(optarg);

                break;
            }
            
            /* steinhart hart c */
            case 11 :
            {
                /* set the times */
                steinhart_hart_c = atof(optarg);

                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("e_calc", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float degrees_celsius;
        
        /* run read test */
        res = ntc_basic_init(circuit, r_fixed_ohm);
        if (res != 0)
        {
            return 1;
        }
        
        if (algorithm == NTC_ALGORITHM_BETA_FORMULA)
        {
            /* set algorithm beta formula */
            res = ntc_basic_set_algorithm_beta_formula(beta, r25_ohm);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: algorithm is beta formula.\n");
            ntc_interface_debug_print("ntc: beta formula beta value is %0.2f.\n", beta);
            ntc_interface_debug_print("ntc: beta formula r25 resistor is %0.2f ohm.\n", r25_ohm);
        }
        else if (algorithm == NTC_ALGORITHM_STEINHART_HART)
        {
            /* set algorithm steinhart hart */
            res = ntc_basic_set_algorithm_steinhart_hart(steinhart_hart_a, steinhart_hart_b, steinhart_hart_c);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: algorithm is steinhart hart.\n");
            ntc_interface_debug_print("ntc: steinhart hart A is %e.\n", steinhart_hart_a);
            ntc_interface_debug_print("ntc: steinhart hart B is %e.\n", steinhart_hart_b);
            ntc_interface_debug_print("ntc: steinhart hart C is %e.\n", steinhart_hart_c);
        }
        else
        {
            /* set algorithm lookup table */
            res = ntc_basic_set_algorithm_lookup_table(gsc_ntc_table, DRIVER_NTC_TABLE_SIZE);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: algorithm is lookup table.\n");
            ntc_interface_debug_print("ntc: table length is %d.\n", DRIVER_NTC_TABLE_SIZE);
        }
        
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
        
        if (filter == NTC_FILTER_NONE)
        {
            /* set filter none */
            res = ntc_basic_set_filter_none();
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
        }
        else if (filter == NTC_FILTER_FIRST_ORDER_LAG)
        {
            res = ntc_basic_set_filter_first_order_lag(0.8f);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: first order lag filter alpha is %.02f.\n", 0.8f);
        }
        else if (filter == NTC_FILTER_MEDIAN)
        {
            res = ntc_basic_set_filter_median(5);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: median filter length is %d.\n", 5);
        }
        else if (filter == NTC_FILTER_ANTI_SPIKE_AVERAGE)
        {
            res = ntc_basic_set_filter_anti_spike_average(5);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: anti spike average filter length is %d.\n", 5);
        }
        else if (filter == NTC_FILTER_MOVING_AVERAGE)
        {
            res = ntc_basic_set_filter_moving_average(5);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: moving average filter length is %d.\n", 5);
        }
        else if (filter == NTC_FILTER_WEIGHTED_MOVING_AVERAGE)
        {
            float weight[] = {1.0, 2.0f, 3.0f, 4.0f, 5.0f};
            
            res = ntc_basic_set_filter_weighted_moving_average(weight, 5);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: weighted moving average filter is %0.2f %0.2f %0.2f %0.2f %0.2f.\n",
                                      weight[0], weight[1], weight[2], weight[3], weight[4]);
        }
        else if (filter == NTC_FILTER_LIMITING)
        {
            res = ntc_basic_set_filter_limiting(10.0f);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: limiting filter is %0.2fC.\n", 10.0f);
        }
        else
        {
            res = ntc_basic_set_filter_kalman(0.005f, 0.5f, 1.0f, 0.0f);
            if (res != 0)
            {
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: kalman q(process noise covariance) is %0.3f.\n", 0.005f);
            ntc_interface_debug_print("ntc: kalman r(measurement noise covariance) is %0.2f.\n", 0.5f);
            ntc_interface_debug_print("ntc: kalman p(estimation error covariance) is %0.2f.\n", 1.0f);
            ntc_interface_debug_print("ntc: kalman x(estimated value) is %0.2f.\n", 0.0f);
        }
        
        /* output */
        ntc_interface_debug_print("ntc: input resistor is ");
        for (i = 0; i < resistor_len; i++)
        {
            ntc_interface_debug_print("%0.2fohm ", resistor_ohm[i]);
        }
        ntc_interface_debug_print("\n");
        
        if (filter == NTC_FILTER_NONE)
        {
            for (i = 0; i < resistor_len; i++)
            {
                /* read temperature */
                res = ntc_basic_calculate_temperature_with_filter(&resistor_ohm[i], 1, &degrees_celsius);
                if (res != 0)
                {
                    ntc_interface_debug_print("ntc: calculate temperature with filter failed.\n");
                    (void)ntc_basic_deinit();
                    
                    return 1;
                }
                
                /* output */
                ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
            }
        }
        else if (filter == NTC_FILTER_FIRST_ORDER_LAG)
        {
            for (i = 0; i < resistor_len; i++)
            {
                /* read temperature */
                res = ntc_basic_calculate_temperature_with_filter(&resistor_ohm[i], 1, &degrees_celsius);
                if (res != 0)
                {
                    ntc_interface_debug_print("ntc: calculate temperature with filter failed.\n");
                    (void)ntc_basic_deinit();
                    
                    return 1;
                }
                
                /* output */
                ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
            }
        }
        else if (filter == NTC_FILTER_MEDIAN)
        {
            /* read temperature */
            res = ntc_basic_calculate_temperature_with_filter(resistor_ohm, resistor_len, &degrees_celsius);
            if (res != 0)
            {
                ntc_interface_debug_print("ntc: calculate temperature with filter failed.\n");
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
        }
        else if (filter == NTC_FILTER_ANTI_SPIKE_AVERAGE)
        {
            /* read temperature */
            res = ntc_basic_calculate_temperature_with_filter(resistor_ohm, resistor_len, &degrees_celsius);
            if (res != 0)
            {
                ntc_interface_debug_print("ntc: calculate temperature with filter failed.\n");
                (void)ntc_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
        }
        else if (filter == NTC_FILTER_MOVING_AVERAGE)
        {
            for (i = 0; i < resistor_len; i++)
            {
                /* read temperature */
                res = ntc_basic_calculate_temperature_with_filter(&resistor_ohm[i], 1, &degrees_celsius);
                if (res != 0)
                {
                    ntc_interface_debug_print("ntc: calculate temperature with filter failed.\n");
                    (void)ntc_basic_deinit();
                    
                    return 1;
                }
                
                /* output */
                ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
            }
        }
        else if (filter == NTC_FILTER_WEIGHTED_MOVING_AVERAGE)
        {
            for (i = 0; i < resistor_len; i++)
            {
                /* read temperature */
                res = ntc_basic_calculate_temperature_with_filter(&resistor_ohm[i], 1, &degrees_celsius);
                if (res != 0)
                {
                    ntc_interface_debug_print("ntc: calculate temperature with filter failed.\n");
                    (void)ntc_basic_deinit();
                    
                    return 1;
                }
                
                /* output */
                ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
            }
        }
        else if (filter == NTC_FILTER_LIMITING)
        {
            for (i = 0; i < resistor_len; i++)
            {
                /* read temperature */
                res = ntc_basic_calculate_temperature_with_filter(&resistor_ohm[i], 1, &degrees_celsius);
                if (res != 0)
                {
                    ntc_interface_debug_print("ntc: calculate temperature with filter failed.\n");
                    (void)ntc_basic_deinit();
                    
                    return 1;
                }
                
                /* output */
                ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
            }
        }
        else
        {
            for (i = 0; i < resistor_len; i++)
            {
                /* read temperature */
                res = ntc_basic_calculate_temperature_with_filter(&resistor_ohm[i], 1, &degrees_celsius);
                if (res != 0)
                {
                    ntc_interface_debug_print("ntc: calculate temperature with filter failed.\n");
                    (void)ntc_basic_deinit();
                    
                    return 1;
                }
                
                /* output */
                ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);
            }
        }
        
        /* deinit */
        (void)ntc_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ntc_interface_debug_print("Usage:\n");
        ntc_interface_debug_print("  ntc (-i | --information)\n");
        ntc_interface_debug_print("  ntc (-h | --help)\n");
        ntc_interface_debug_print("  ntc (-p | --port)\n");
        ntc_interface_debug_print("  ntc (-e calc | --example=calc) [--algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>]\n");
        ntc_interface_debug_print("      [--resistor=<number0,...>]\n");
        ntc_interface_debug_print("      [--beta=<value>] [--r25-resistor=<ohm>]\n");
        ntc_interface_debug_print("      [--steinhart-hart-a=<value>] [--steinhart-hart-b=<value>] [--steinhart-hart-c=<value>]\n");
        ntc_interface_debug_print("      [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>]\n");
        ntc_interface_debug_print("\n");
        ntc_interface_debug_print("Options:\n");
        ntc_interface_debug_print("      --algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>\n");
        ntc_interface_debug_print("                                 Set the algorithm.([default: beta-formula])\n");
        ntc_interface_debug_print("      --beta=<value>             Set the beta value.([default: 3950.0f])\n");
        ntc_interface_debug_print("  -e <calc>, --example=<calc>\n");
        ntc_interface_debug_print("                                 Run the driver example.\n");
        ntc_interface_debug_print("      --filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE |\n");
        ntc_interface_debug_print("                WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>\n");
        ntc_interface_debug_print("                                 Set the filter.([default: NONE])\n");
        ntc_interface_debug_print("  -h, --help                     Show the help.\n");
        ntc_interface_debug_print("  -i, --information              Show the chip information.\n");
        ntc_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        ntc_interface_debug_print("      --r25-resistor=<ohm>       Set the R25 resistor.([default: 10000.0f])\n");
        ntc_interface_debug_print("      --resistor=<number0,...>   Set the resistor values.\n");
        ntc_interface_debug_print("      --steinhart-hart-a=<value> Set the Steinhart-Hart A coefficient.([default: 1.129148e-3])\n");
        ntc_interface_debug_print("      --steinhart-hart-b=<value> Set the Steinhart-Hart B coefficient.([default: 2.34125e-4])\n");
        ntc_interface_debug_print("      --steinhart-hart-c=<value> Set the Steinhart-Hart C coefficient.([default: 8.76741e-8])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ntc_info_t info;

        /* print ntc info */
        ntc_info(&info);
        ntc_interface_debug_print("ntc: chip is %s.\n", info.chip_name);
        ntc_interface_debug_print("ntc: manufacturer is %s.\n", info.manufacturer_name);
        ntc_interface_debug_print("ntc: interface is %s.\n", info.interface);
        ntc_interface_debug_print("ntc: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ntc_interface_debug_print("ntc: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ntc_interface_debug_print("ntc: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ntc_interface_debug_print("ntc: max current is %0.2fmA.\n", info.max_current_ma);
        ntc_interface_debug_print("ntc: max temperature is %0.1fC.\n", info.temperature_max);
        ntc_interface_debug_print("ntc: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ntc_interface_debug_print("ntc: ADC connected to none.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;
    
    res = ntc(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        ntc_interface_debug_print("ntc: run failed.\n");
    }
    else if (res == 5)
    {
        ntc_interface_debug_print("ntc: param is invalid.\n");
    }
    else
    {
        ntc_interface_debug_print("ntc: unknown status code.\n");
    }

    return 0;
}
