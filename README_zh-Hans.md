[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTC

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntc/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

热敏电阻是一种传感器电阻，电阻值随着温度的变化而改变。热敏电阻属可变电阻的一类，广泛应用于各种电子元件中，例如涌浪电流限制器、温度传感器、可复式保险丝、及自动调节的加热器等。不同于电阻温度计使用纯金属，在热敏电阻器中使用的材料通常是陶瓷或聚合物。

LibDriver NTC是LibDriver推出的NTC全功能驱动，该驱动提供查表法温度转换、B值温度转换、Steinhart Hart公式温度转换、NTC采集开路检测、NTC采集短路检测，一阶滞后滤波、中值滤波、防脉冲干扰平均滤波、滑动平均滤波、加权滑动平均滤波、限幅滤波和卡尔曼滤波等功能并且它符合MISRA标准。

LibDriver NTC Tool是一个专为LibDriver NTC开发的工具，使用该工具可以快速根据Excel数据生成查表法所需的头文件，该工具还可以根据三点的温度-电阻数据，计算Steinhart Hart系数A，B和C。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [滤波](#滤波)
    - [一阶滞后滤波](#一阶滞后滤波)
    - [中值滤波](#中值滤波)
    - [防脉冲干扰平均滤波](#防脉冲干扰平均滤波)
    - [滑动平均滤波](#滑动平均滤波)
    - [加权滑动平均滤波](#加权滑动平均滤波)
    - [限幅滤波](#限幅滤波)
    - [卡尔曼滤波](#卡尔曼滤波)
  - [LibDriver NTC Tool](#LibDriver-NTC-Tool)
    - [命令行](#命令行)
    - [图形界面](#图形界面)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver NTC的源文件。

/interface目录包含了LibDriver NTC与平台无关的ADC总线模板。

/test目录包含了LibDriver NTC驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver NTC编程范例。

/doc目录包含了LibDriver NTC离线文档。

/datasheet目录包含了NTC数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

/utils目录包含了LibDriver NTC Tool。

### 安装

参考/interface目录下与平台无关的ADC总线模板，完成指定平台的ADC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 滤波

LibDriver NTC集成了一阶滞后滤波、中值滤波、防脉冲干扰平均滤波、滑动平均滤波、加权滑动平均滤波、限幅滤波和卡尔曼滤波几种常用的NTC滤波算法，原始数据经合适的滤波处理后往往能获得更好的输出结果。

#### 一阶滞后滤波

一阶滞后滤波器是一种利用软件算法模拟硬件 \(RC\) 低通滤波效果的技术。它通过将当前采样值与历史滤波结果进行加权平均，有效滤除高频噪声和毛刺，保留有用信号，使数据输出更加平滑稳定。

$$
y(n) = \alpha \cdot x(n) + (1 - \alpha) \cdot y(n - 1)
$$

#### 中值滤波

中值滤波连续采样 N次并排序，取中间值，能有效抑制偶然的脉冲干扰。

$$
y(n) = med \{ x(n-k), ..., x(n), ..., x(n+k) \}
$$

#### 防脉冲干扰平均滤波

防脉冲干扰平均滤波器是一种融合了“中位值滤波”与“算术平均滤波”优点的数字滤波算法。它去掉采样队列中的最大值和最小值后，再计算平均值，它能有效剔除偶然的脉冲性突发干扰，同时平滑周期性噪声，保证采样数据的稳定与精准。

$$
y = \frac{1}{n-2} \left ( \sum\limits_{i = 1}^{n} {{{x}_{i}} - ({{x}_{(1)}} + {{x}_{(n)}})} \right )
$$

#### 滑动平均滤波

滑动平均滤波维护一个固定长度的先进先出队列，每次加入新样本并计算队列平均值。

$$
y(n) = \frac{1}{N} \sum \limits_{i = 0}^{N-1}{x(n-i)}
$$

#### 加权滑动平均滤波

加权滑动平均滤波器在滑动平均的基础上，给越接近当前时刻的样本赋予越高的权重。

$$
y(n) = \frac {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}} \cdot x(n-i)}} {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}}}}
$$

#### 限幅滤波

限幅滤波比较当前采样值与上一次有效值的差值，若超过设定的门限则丢弃。

$$
y(n) = \begin{cases} 
x(n) & |x(n) - y(n-1)| \le \Delta \\ 
y(n-1) & |x(n) - y(n-1)| > \Delta 
\end{cases}
$$

#### 卡尔曼滤波

卡尔曼滤波器是一种高效的递归滤波器，通过结合系统模型预测和传感器测量数据，从带噪声的序列中估计动态系统的内部状态。它在传感器数据融合、导航和控制领域应用广泛，其核心在于“预测-更新”的迭代过程，能够最小化状态估计的均方误差。

$$
\begin{gathered}
\hat{x}_{k \vert{} k-1} = \hat{x}_{k-1 \vert{} k-1} \\
             P_{k \vert{} k-1} = P_{k-1 \vert{} k-1} + Q \\
             K_k = \frac{P_{k \vert{} k-1}}{P_{k \vert{} k-1} + R} \\
             \hat{x}_{k \vert{} k} = \hat{x}_{k \vert{} k-1} + K_k(z_k - \hat{x}_{k \vert{} k-1}) \\
             P_{k \vert{} k} = (1 - K_k)P_{k \vert{} k-1}
\end{gathered}
$$

### LibDriver NTC Tool

LibDriver NTC提供了一个快速生成查表法所需头文件的工具，使用该工具可以快速的将Excel文件转换为适合于LibDriver NTC使用的头文件。该工具还可以计算Steinhart Hart系数，选择Excel数据中任意三个不重合的点，计算Steinhart Hart的A，B，C系数，该工具提供命令行和图形界面两种操作模式。

LibDriver NTC工具位于/utils目录，ntc_utils.py为LibDriver NTC工具程序，ntc_table_template.xlsx为Excel数据模板，可以根据此模板导入不同型号的NTC温度-电阻数据。requirements.txt为LibDriver NTC工具依赖项描述文件，resource为LibDriver NTC工具资源文件夹。该工具在运行过程中会产生config.json配置文件，该文件用于记录用户配置。产生的头文件默认为driver_ntc_table.h，该头文件固定产生于/utils目录。

1. 安装依赖

    ```shell
    # go to the LibDriver NTC Tool dir
    cd utils
    
    # install requirements
    pip install -r requirements.txt
    ```
    
2. 帮助命令
   
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

#### 命令行

1. 生成查表法C头文件

   ```shell
   python ntc_utils.py -c header -i ntc_table_template.xlsx -o driver_ntc_table.h
   
   c header generator
   input excel file: ntc_table_template.xlsx
   output c header file: driver_ntc_table.h
   success
   ```

   -c header 生成查表法C头文件命令

   -i Excel文件路径，Excel模板的temperature_c，resistance_kohm和ntc table勿修改

   -o 输出C头文件名字，固定为/utils目录

2. 计算Steinhart Hart系数

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

   -c steinhart-hart 计算Steinhart Hart系数命令

   -i Excel文件路径，Excel模板的temperature_c，resistance_kohm和ntc table勿修改

   -t0 Steinhart Hart系数计算温度点0，三个点不要相同且必须是Excel文件包含温度点

   -t1 Steinhart Hart系数计算温度点1，三个点不要相同且必须是Excel文件包含温度点

   -t2 Steinhart Hart系数计算温度点2，三个点不要相同且必须是Excel文件包含温度点

#### 图形界面

启动图形界面

```shell
python ntc_utils.py -c gui
```

-c gui 启动图形界面命令

启动界面如下，右上角为语言切换列表，可以选择English，简体中文，繁體中文，日本語， Deutsch和한국어。

<div align=center>
<img src="/doc/image/zh-Hans/main.png" width="750" height="561"/>
</div>

点击导入Excel，程序会导入Excel数据，并绘制电阻-温度特性图。设置导出头文件名字，点击生成C头文件，可以生成查表法C头文件并打开该文件存储目录。

<div align=center>
<img src="/doc/image/zh-Hans/table.png" width="750" height="561"/>
</div>

设置Steinhart Hart系数计算温度点T0，T1和T2，点击计算Steinhart-Hart系数，可计算基于三点的Steinhart Hart系数并输出显示，同时绘制基于该参数的电阻与温度特性图，用于比较与原始数据差异。

<div align=center>
<img src="/doc/image/zh-Hans/sh.png" width="750" height="561"/>
</div>

### 文档

在线文档: [https://www.libdriver.com/docs/ntc/index.html](https://www.libdriver.com/docs/ntc/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。