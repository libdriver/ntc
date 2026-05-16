[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTC

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntc/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

A thermistor is a type of sensor resistor, whose resistance value changes with temperature. Thermistors belong to the category of variable resistors and are widely used in various electronic components, such as surge current limiters, temperature sensors, resettable fuses, and self-regulating heaters. Unlike resistance thermometers, which use pure metals, the materials used in thermistors are typically ceramics or polymers.

LibDriver NTC is a full-featured driver for NTC, launched by LibDriver. It provides lookup table temperature conversion, B-value temperature conversion, Steinhart-Hart formula temperature conversion, NTC open-circuit detection, NTC short-circuit detection, first-order lag filtering, median filtering, anti-pulse interference average filtering, moving average filtering, weighted moving average filtering, amplitude limiting filtering, Kalman filtering and additional features. LibDriver is MISRA compliant.

LibDriver NTC Tool is a tool specifically developed for LibDriver NTC. Using this tool, you can quickly generate the header files required for the look-up table method based on Excel data. Additionally, this tool can calculate the Steinhart-Hart coefficients A, B, and C based on temperature-resistance data from three points.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Filter](#Filter)
    - [FIRST_ORDER_LAG_FILTER](#FIRST_ORDER_LAG_FILTER)
    - [MEDIAN](#MEDIAN)
    - [ANTI_SPIKE_AVERAGE](#ANTI_SPIKE_AVERAGE)
    - [MOVING_AVERAGE](#MOVING_AVERAGE)
    - [WEIGHTED_MOVING_AVERAGE](#WEIGHTED_MOVING_AVERAGE)
    - [LIMITING](#LIMITING)
    - [KALMAN](#KALMAN)
  - [LibDriver NTC Tool](#LibDriver-NTC-Tool)
    - [CLI](#CLI)
    - [GUI](#GUI)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver NTC source files.

/interface includes LibDriver NTC ADC platform independent template.

/test includes LibDriver NTC driver test code and this code can test the chip necessary function simply.

/example includes LibDriver NTC sample code.

/doc includes LibDriver NTC offline document.

/datasheet includes NTC datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

/utils includes the LibDriver NTC Tool.

### Install

Reference /interface ADC platform independent template and finish your platform ADC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_ntc_basic.h"
#include "driver_ntc_table.h"

uint32_t times = 3;
float r_fixed_ohm = 10000.0f;
float beta = 3950.0f;
float r25_ohm = 10000.0f;
float resistor_ohm[NTC_FILTER_BUFFER_SIZE];
uint16_t resistor_len = 5;
double steinhart_hart_a = 1.129148e-3;
double steinhart_hart_b = 2.34125e-4;
double steinhart_hart_c = 8.76741e-8;
ntc_filter_t filter = NTC_FILTER_NONE;
ntc_circuit_t circuit = NTC_CIRCUIT_VCC_R_NTC_GND;
ntc_algorithm_t algorithm = NTC_ALGORITHM_BETA_FORMULA;
uint8_t res;
uint32_t i;

/* run read test */
res = ntc_basic_init(circuit, r_fixed_ohm);
if (res != 0)
{
    return 1;
}

...
    
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

...
    
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

...
    
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

....
    
if (filter == NTC_FILTER_NONE)
{
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
    res = ntc_basic_set_filter_kalman(0.005f, 0.5f, 1.0f, nanf(""));
    if (res != 0)
    {
        (void)ntc_basic_deinit();

        return 1;
    }

    /* output */
    ntc_interface_debug_print("ntc: kalman q(process noise covariance) is %0.3f.\n", 0.005f);
    ntc_interface_debug_print("ntc: kalman r(measurement noise covariance) is %0.2f.\n", 0.5f);
    ntc_interface_debug_print("ntc: kalman p(estimation error covariance) is %0.2f.\n", 1.0f);
    ntc_interface_debug_print("ntc: kalman x(estimated value) is read the current adc.\n");
}

...
    
for (i = 0; i < times; i++)
{
    float degrees_celsius;

    /* read temperature */
    res = ntc_basic_read(&degrees_celsius);
    if (res != 0)
    {
        ntc_interface_debug_print("ntc: read temperature failed.\n");
        (void)ntc_basic_deinit();

        return 1;
    }

    /* output */
    ntc_interface_debug_print("ntc: %d/%d.\n", i + 1, times);
    ntc_interface_debug_print("ntc: temperature is %0.2fC.\n", degrees_celsius);

    /* delay 1000ms */
    ntc_interface_delay_ms(1000);
    
    ...
}

...

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

...
    
/* deinit */
(void)ntc_basic_deinit();

return 0;
```

### Filter

LibDriver NTC integrates several NTC filtering algorithms, including first-order lag filtering, median filtering, anti-pulse interference averaging filtering, moving average filtering, weighted moving average filtering, amplitude limiting filtering, and Kalman filtering. After appropriate filtering processing of the raw data, better output results can often be obtained.

#### FIRST_ORDER_LAG_FILTER

The first-order lag filter is a technique that utilizes software algorithms to simulate the effect of hardware \(RC\) low-pass filtering. By performing a weighted average of the current sampling value and the historical filtering results, it effectively filters out high-frequency noise and glitches, retains useful signals, and makes the data output smoother and more stable.

$$
y(n) = \alpha \cdot x(n) + (1 - \alpha) \cdot y(n - 1)
$$

#### MEDIAN

Median filtering involves sampling N times in succession, sorting the results, and selecting the median value. This effectively suppresses occasional pulse interference.

$$
y(n) = med \{ x(n-k), ..., x(n), ..., x(n+k) \}
$$

#### ANTI_SPIKE_AVERAGE

The pulse interference-resistant average filter is a digital filtering algorithm that combines the advantages of "median filtering" and "arithmetic average filtering". After removing the maximum and minimum values from the sampling queue, it calculates the average value. It can effectively eliminate accidental pulse-like burst interference, while smoothing periodic noise, ensuring the stability and accuracy of the sampled data.

$$
y = \frac{1}{n-2} \left ( \sum\limits_{i = 1}^{n} {{{x}_{i}} - ({{x}_{(1)}} + {{x}_{(n)}})} \right )
$$

#### MOVING_AVERAGE

The sliding average filter maintains a fixed-length first-in first-out (FIFO) queue, adding new samples each time and calculating the average value of the queue.

$$
y(n) = \frac{1}{N} \sum \limits_{i = 0}^{N-1}{x(n-i)}
$$

#### WEIGHTED_MOVING_AVERAGE

Based on the sliding average, the weighted sliding average filter assigns higher weights to samples that are closer to the current time.

$$
y(n) = \frac {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}} \cdot x(n-i)}} {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}}}}
$$

#### LIMITING

The limiting filter compares the difference between the current sampling value and the previous valid value. If the difference exceeds the set threshold, it is discarded.

$$
y(n) = \begin{cases} x(n) & |x(n) - y(n-1)| \le \Delta \\ y(n-1) & |x(n) - y(n-1)| > \Delta \end{cases}
$$

#### KALMAN

The Kalman filter is an efficient recursive filter that estimates the internal state of a dynamic system from a noisy sequence by combining system model predictions and sensor measurement data. It is widely used in the fields of sensor data fusion, navigation, and control. Its core lies in the iterative process of "prediction-update", which can minimize the mean square error of state estimation.

$$
\hat{x}_{k \vert{} k-1} = \hat{x}_{k-1 \vert{} k-1} \\
             P_{k \vert{} k-1} = P_{k-1 \vert{} k-1} + Q \\
             K_k = \frac{P_{k \vert{} k-1}}{P_{k \vert{} k-1} + R} \\
             \hat{x}_{k \vert{} k} = \hat{x}_{k \vert{} k-1} + K_k(z_k - \hat{x}_{k \vert{} k-1}) \\
             P_{k \vert{} k} = (1 - K_k)P_{k \vert{} k-1}
$$

### LibDriver NTC Tool

LibDriver NTC provides a tool for quickly generating the header files required for the lookup table method. This tool allows for the rapid conversion of Excel files into header files suitable for use with LibDriver NTC. Additionally, the tool can calculate Steinhart-Hart coefficients. By selecting any three non-coincident points from the Excel data, the tool can compute the Steinhart-Hart coefficients A, B, and C. The tool offers both command-line and graphical interface modes of operation.

The LibDriver NTC tool is located in the /utils directory. ntc_utils.py is the LibDriver NTC Tool program, ntc_table_template.xlsx is an Excel data template that allows for the import of NTC temperature-resistance data for different models, and requirements.txt is a file describing the dependencies of the LibDriver NTC Tool. The resource folder contains the resources for the LibDriver NTC Tool. During the tool's operation, a config.json configuration file is generated to record user settings. The generated header file, named driver_ntc_table.h by default, is consistently created in the /utils directory.

1. Install dependencies

    ```shell
    # go to the LibDriver NTC Tool dir
    cd utils
    
    # install requirements
    pip install -r requirements.txt
    ```
    
2. help command
   
   ```shell
   python ntc_utils.py --help
   
   Usage: ntc_utils.py [OPTIONS]
   
   ┌─ Options ──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
   │ --input-file    -i       TEXT                         Input Excel file name [default: ntc_table_template.xlsx]                                                                                                 │
   │ --output-file   -o       TEXT                         Output C header file name [default: driver_ntc_table.h]                                                                                                  │
   │ --command       -c       [header|steinhart-hart|gui]  Command type, only header, steinhart-hart or gui [default: gui]                                                                                          │
   │ --temperature0  -t0      FLOAT                        Steinhart-Hart temperature point 0 [default: 0.0]                                                                                                        │
   │ --temperature1  -t1      FLOAT                        Steinhart-Hart temperature point 1 [default: 25.0]                                                                                                       │
   │ --temperature2  -t2      FLOAT                        Steinhart-Hart temperature point 2 [default: 80.0]                                                                                                       │
   │ --help                                                Show this message and exit.                                                                                                                              │
   └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
   ```

#### CLI

1. Generate the C header file for the lookup table method

   ```shell
   python ntc_utils.py -c header -i ntc_table_template.xlsx -o driver_ntc_table.h

   c header generator
   input excel file: ntc_table_template.xlsx
   output c header file: driver_ntc_table.h
   success
   ```

   -c header generates a lookup table method C header file command

   -i Excel file path, do not modify the temperature_c, resistance_kohm, and ntc table in the Excel template

   -o Output the name of the C header file, which is fixed in the /utils directory

2. Calculate the Steinhart-Hart coefficient

   ```shell
   python ntc_utils.py -c steinhart-hart -i ntc_table_template.xlsx  -t0 0 -t1 50 -t2 100
   
   Steinhart-Hart A B C calculator
   input excel file: ntc_table_template.xlsx
   temperature point 0: 0.0℃
   temperature point 1: 50.0℃
   temperature point 2: 100.0℃
   steinhart-hart A is 1.12919441e-03
   steinhart-hart B is 2.34746649e-04
   steinhart-hart C is 7.99758254e-08
   success
   ```

   -c steinhart-hart command for calculating Steinhart-Hart coefficients

   -i Excel file path, do not modify the temperature_c, resistance_kohm, and ntc table in the Excel template

   -t0 Steinhart Hart coefficient calculation temperature point 0, three points should not be the same and must be included in the Excel file containing temperature points

   -t1 Steinhart Hart coefficient calculation temperature point 1, three points should not be the same and must be included in the Excel file containing temperature points

   -t2 Steinhart Hart coefficient calculation temperature point 2, three points should not be the same and must be included in the Excel file containing temperature points

#### GUI

Start the program

```shell
python ntc_utils.py -c gui
```

-c gui Command to start the program

The startup interface is as follows. On the top right corner is a language switching list, where you can choose English，简体中文，繁體中文，日本語， Deutsch and 한국어.

<div align=center>
<img src="/doc/image/en/main.png"/>
</div>

Click "Import Excel" and the program will import the Excel data and plot the resistance-temperature characteristic curve. Set the name of the exported header file, click "Generate C Header File", and you can generate a lookup table C header file and open the directory where the file is stored.

<div align=center>
<img src="/doc/image/en/table.png"/>
</div>

Set the Steinhart-Hart coefficients for calculating temperature points T0, T1, and T2. Click "Calculate Steinhart-Hart" to compute the Steinhart-Hart coefficients based on these three points and display the results. Simultaneously, plot the resistance-temperature characteristic curve based on these parameters for comparing the differences with the original data.

<div align=center>
<img src="/doc/image/en/sh.png"/>
</div>

### Document

Online documents: [https://www.libdriver.com/docs/ntc/index.html](https://www.libdriver.com/docs/ntc/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.