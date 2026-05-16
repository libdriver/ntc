[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTC

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntc/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

熱敏電阻是一種傳感器電阻，電阻值隨著溫度的變化而改變。熱敏電阻屬可變電阻的一類，廣泛應用於各種電子元件中，例如湧浪電流限制器、溫度傳感器、可復式保險絲、及自動調節的加熱器等。不同於電阻溫度計使用純金屬，在熱敏電阻器中使用的材料通常是陶瓷或聚合物。

LibDriver NTC是LibDriver推出的NTC全功能驅動，該驅動提供查表法溫度轉換、B值溫度轉換、Steinhart Hart公式溫度轉換、NTC採集開路檢測、NTC採集短路檢測，一階滯後濾波、中值濾波、防脈衝干擾平均濾波、滑動平均濾波、加權滑動平均濾波、限幅濾波和卡爾曼濾波等功能並且它符合MISRA標準。

LibDriver NTC Tool是一個專為LibDriver NTC開發的工具，使用該工具可以快速根據Excel數據生成查表法所需的頭文件，該工具還可以根據三點的溫度-電阻數據，計算Steinhart Hart係數A，B和C。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [濾波](#濾波)
    - [一階滯後濾波](#一階滯後濾波)
    - [中值濾波](#中值濾波)
    - [防脈衝干擾平均濾波](#防脈衝干擾平均濾波)
    - [滑動平均濾波](#滑動平均濾波)
    - [加權滑動平均濾波](#加權滑動平均濾波)
    - [限幅濾波](#限幅濾波)
    - [卡爾曼濾波](#卡爾曼濾波)
  - [LibDriver NTC Tool](#LibDriver_NTC_Tool)
    - [命令行](#命令行)
    - [圖形界面](#圖形界面) 
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver NTC的源文件。

/interface目錄包含了LibDriver NTC與平台無關的ADC總線模板。

/test目錄包含了LibDriver NTC驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver NTC編程範例。

/doc目錄包含了LibDriver NTC離線文檔。

/datasheet目錄包含了NTC數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

/utils目錄包含了LibDriver NTC Tool。

### 安裝

參考/interface目錄下與平台無關的ADC總線模板，完成指定平台的ADC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 濾波

LibDriver NTC整合了一階滯後濾波、中值濾波、防脈衝干擾平均濾波、滑動平均濾波、加權滑動平均濾波、限幅濾波和卡爾曼濾波幾種常用的NTC濾波算法，原始數據經合適的濾波處理後往往能獲得更好的輸出結果。

#### 一階滯後濾波

一階滯後濾波器是一種利用軟件算法模擬硬件 \(RC\) 低通濾波效果的技術。它通過將當前採樣值與歷史濾波結果進行加權平均，有效濾除高頻雜訊和毛刺，保留有用訊號，使數據輸出更加平滑穩定。
$$
y(n) = \alpha \cdot x(n) + (1 - \alpha) \cdot y(n - 1)
$$

#### 中值濾波

中值濾波連續採樣N次並排序，取中間值，能有效抑制偶然的脈衝干擾。
$$
y(n) = med \{ x(n-k), ..., x(n), ..., x(n+k) \}
$$

#### 防脈衝干擾平均濾波

防脈衝干擾平均濾波器是一種融合了「中位值濾波」與「算術平均濾波」優點的數字濾波算法。它去掉採樣隊列中的最大值和最小值後，再計算平均值，它能有效剔除偶然的脈衝性突發干擾，同時平滑週期性雜訊，保證採樣數據的穩定與精準。
$$
y = \frac{1}{n-2} \left ( \sum\limits_{i = 1}^{n} {{{x}_{i}} - ({{x}_{(1)}} + {{x}_{(n)}})} \right )
$$

#### 滑動平均濾波

滑動平均濾波維護一個固定長度的先進先出佇列，每次加入新樣本並計算佇列平均值。
$$
y(n) = \frac{1}{N} \sum \limits_{i = 0}^{N-1}{x(n-i)}
$$

#### 加權滑動平均濾波

加權滑動平均濾波器在滑動平均的基礎上，給越接近當前時刻的樣本賦予越高的權重。
$$
y(n) = \frac {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}} \cdot x(n-i)}} {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}}}}
$$

#### 限幅濾波

限幅濾波比較當前採樣值與上一次有效值的差值，若超過設定的門限則捨棄。
$$
y(n) = \begin{cases} x(n) & |x(n) - y(n-1)| \le \Delta \\ y(n-1) & |x(n) - y(n-1)| > \Delta \end{cases}
$$

#### 卡爾曼濾波

卡爾曼濾波器是一種高效的遞迴濾波器，通過結合系統模型預測和感測器測量數據，從帶雜訊的序列中估計動態系統的內部狀態。它在感測器數據融合、導航和控制領域應用廣泛，其核心在於「預測-更新」的迭代過程，能夠最小化狀態估計的均方誤差。
$$
\hat{x}_{k \vert{} k-1} = \hat{x}_{k-1 \vert{} k-1} \\
             P_{k \vert{} k-1} = P_{k-1 \vert{} k-1} + Q \\
             K_k = \frac{P_{k \vert{} k-1}}{P_{k \vert{} k-1} + R} \\
             \hat{x}_{k \vert{} k} = \hat{x}_{k \vert{} k-1} + K_k(z_k - \hat{x}_{k \vert{} k-1}) \\
             P_{k \vert{} k} = (1 - K_k)P_{k \vert{} k-1}
$$

### LibDriver NTC Tool

LibDriver NTC提供了一個快速生成查表法所需頭文件的工具，使用該工具可以快速地將Excel文件轉換為適合於LibDriver NTC使用的頭文件。該工具還可以計算Steinhart Hart係數，選擇Excel數據中任意三個不重合的點，計算Steinhart Hart的A，B，C係數，該工具提供命令行和圖形界面兩種操作模式。

LibDriver NTC工具位於/utils目錄，ntc_utils.py為LibDriver NTC工具程式，ntc_table_template.xlsx為Excel數據模板，可以根據此模板導入不同型號的NTC溫度-電阻數據。requirements.txt為LibDriver NTC工具依賴項描述文件，resource為LibDriver NTC工具資源文件夾。該工具在運行過程中會產生config.json配置文件，該文件用於記錄用戶配置。產生的頭文件默認為driver_ntc_table.h，該頭文件固定產生於/utils目錄。

1. 安裝依賴

    ```shell
    # go to the LibDriver NTC Tool dir
    cd utils
    
    # install requirements
    pip install -r requirements.txt
    ```
    
2. 幫助命令
   
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

1. 生成查表法C標頭檔

   ```shell
   python ntc_utils.py -c header -i ntc_table_template.xlsx -o driver_ntc_table.h
   
   c header generator
   input excel file: ntc_table_template.xlsx
   output c header file: driver_ntc_table.h
   success
   ```

   -c header 生成查表法C頭文件命令

   -i Excel檔案路徑，Excel模板中的temperature_c、resistance_kohm和ntc table請勿修改

   -o 輸出C頭文件名稱，固定為/utils目錄

2. 計算Steinhart Hart係數

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

   -c steinhart-hart 計算Steinhart Hart係數命令

   -i Excel檔案路徑，Excel模板中的temperature_c、resistance_kohm和ntc table請勿修改

   -t0 Steinhart Hart係數計算溫度點0，三個點不要相同且必须是Excel檔案包含的溫度點

   -t1 Steinhart Hart係數計算溫度點1，三個點不要相同且必须是Excel檔案包含的溫度點

   -t2 Steinhart Hart係數計算溫度點2，三個點不要相同且必须是Excel文件包含的溫度點

#### 圖形界面

啟動圖形界面

```shell
python ntc_utils.py -c gui
```

-c gui 啟動圖形界面命令

啟動界面如下，右上角為語言切換列表，可以選擇English，简体中文，繁體中文，日本語， Deutsch和한국어。

<div align=center>
<img src="/doc/image/zh-Hant/main.png"/>
</div>

點擊導入Excel，程式會導入Excel數據，並繪製電阻-溫度特性圖。設置導出頭文件名字，點擊生成C頭文件，可以生成查表法C頭文件並打開該文件存儲目錄。

<div align=center>
<img src="/doc/image/zh-Hant/table.png"/>
</div>

設定Steinhart Hart係數計算溫度點T0，T1和T2，點擊計算Steinhart-Hart係數，可計算基於三點的Steinhart Hart係數並輸出顯示，同時繪製基於該參數的電阻與溫度特性圖，用於比較與原始數據的差異。

<div align=center>
<img src="/doc/image/zh-Hant/sh.png"/>
</div>

### 文檔

在線文檔: [https://www.libdriver.com/docs/ntc/index.html](https://www.libdriver.com/docs/ntc/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。