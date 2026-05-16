[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTC

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntc/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

サーミスタは、温度によって抵抗値が変化するセンサー抵抗器の一種です。サーミスタは可変抵抗器に分類され、サージ電流リミッター、温度センサー、リセット可能なヒューズ、自己制御ヒーターなど、さまざまな電子部品に広く使用されています。純金属を使用する抵抗温度計とは異なり、サーミスタに使用される材料は通常、セラミックまたはポリマーです。

LibDriver NTCは、LibDriver社が提供するNTC用のフル機能ドライバです。ルックアップテーブルによる温度変換、B値による温度変換、スタインハート・ハート式による温度変換、NTC開回路検出、NTC短絡検出、一次遅延フィルタリング、メディアンフィルタリング、パルス干渉抑制平均フィルタリング、移動平均フィルタリング、加重移動平均フィルタリング、振幅制限フィルタリング、カルマンフィルタなどの機能を備えています。LibDriverはMISRA規格に準拠しています。

LibDriver NTCツールは、LibDriver NTC専用に開発されたツールです。このツールを使用すると、Excelデータに基づいてルックアップテーブル方式に必要なヘッダーファイルを迅速に生成できます。さらに、このツールは3点の温度抵抗データに基づいて、スタインハート・ハート係数A、B、Cを計算することも可能です。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [フィルター](#フィルター)
    - [一次遅延フィルター](#一次遅延フィルター)

    - [中央値](#中央値)

    - [スパイク防止平均](#スパイク防止平均)

    - [移動平均](#移動平均)

    - [加重移動平均](#加重移動平均)

    - [制限値](#制限値)

    - [カルマンフィルター](#カルマンフィルター)
    
- [LibDriver NTC Tool](#LibDriver NTC Tool)
  - [CLI](#CLI)
    
  - [GUI](#GUI)
- [ドキュメント](#ドキュメント)
- [貢献](#貢献)
- [著作権](#著作権)
- [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver NTCのソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver NTC用のプラットフォームに依存しないADCバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver NTCドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver NTCプログラミング例が含まれています。

/ docディレクトリには、LibDriver NTCオフラインドキュメントが含まれています。

/ datasheetディレクトリには、NTCデータシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

/utils には LibDriver NTC ツールが含まれています。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないADCバステンプレートを参照して、指定したプラットフォームのADCバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### フィルター

LibDriver NTCは、一次遅延フィルタ、メディアンフィルタ、パルス干渉抑制平均化フィルタ、移動平均フィルタ、加重移動平均フィルタ、振幅制限フィルタ、カルマンフィルタなど、複数のNTCフィルタアルゴリズムを統合しています。生データに適切なフィルタ処理を施すことで、多くの場合、より優れた出力結果が得られます。

#### 一次遅延フィルター

一次遅延フィルタは、ソフトウェアアルゴリズムを用いてハードウェアのRCローパスフィルタの効果をシミュレートする手法です。現在のサンプリング値と過去のフィルタリング結果の加重平均を行うことで、高周波ノイズやグリッチを効果的に除去し、有用な信号を保持し、より滑らかで安定したデータ出力を実現します。
$$
y(n) = \alpha \cdot x(n) + (1 - \alpha) \cdot y(n - 1)
$$

#### 中央値

メディアンフィルタは、N回連続してサンプリングを行い、結果をソートして中央値を選択する処理です。これにより、偶発的なパルス干渉を効果的に抑制できます。
$$
y(n) = med \{ x(n-k), ..., x(n), ..., x(n+k) \}
$$

#### スパイク防止平均

パルス干渉耐性平均フィルタは、「メディアンフィルタ」と「算術平均フィルタ」の利点を組み合わせたデジタルフィルタアルゴリズムです。サンプリングキューから最大値と最小値を除外した後、平均値を計算します。偶発的なパルス状のバースト干渉を効果的に除去し、周期的なノイズを平滑化することで、サンプリングデータの安定性と精度を確保します。
$$
y = \frac{1}{n-2} \left ( \sum\limits_{i = 1}^{n} {{{x}_{i}} - ({{x}_{(1)}} + {{x}_{(n)}})} \right )
$$

#### 移動平均

移動平均フィルタは、固定長の先入れ先出し（FIFO）キューを保持し、毎回新しいサンプルを追加してキューの平均値を計算します。
$$
y(n) = \frac{1}{N} \sum \limits_{i = 0}^{N-1}{x(n-i)}
$$

#### 加重移動平均

加重移動平均フィルタは、移動平均に基づいて、現在時刻に近いサンプルに高い重みを割り当てます。
$$
y(n) = \frac {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}} \cdot x(n-i)}} {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}}}}
$$

#### 制限値

リミッティングフィルタは、現在のサンプリング値と前の有効な値との差を比較します。差が設定された閾値を超えた場合、その値は破棄されます。
$$
y(n) = \begin{cases} x(n) & |x(n) - y(n-1)| \le \Delta \\ y(n-1) & |x(n) - y(n-1)| > \Delta \end{cases}
$$

#### カルマンフィルター

カルマンフィルタは、システムモデル予測とセンサ計測データを組み合わせることで、ノイズを含む時系列データから動的システムの内部状態を推定する効率的な再帰フィルタです。センサデータ融合、ナビゲーション、制御などの分野で広く用いられています。その核心は、「予測更新」の反復処理にあり、これにより状態推定の平均二乗誤差を最小化できます。
$$
\hat{x}_{k \vert{} k-1} = \hat{x}_{k-1 \vert{} k-1} \\
             P_{k \vert{} k-1} = P_{k-1 \vert{} k-1} + Q \\
             K_k = \frac{P_{k \vert{} k-1}}{P_{k \vert{} k-1} + R} \\
             \hat{x}_{k \vert{} k} = \hat{x}_{k \vert{} k-1} + K_k(z_k - \hat{x}_{k \vert{} k-1}) \\
             P_{k \vert{} k} = (1 - K_k)P_{k \vert{} k-1}
$$

### LibDriver NTC

LibDriver NTCは、ルックアップテーブル方式に必要なヘッダーファイルを迅速に生成するためのツールを提供します。このツールを使用すると、ExcelファイルをLibDriver NTCで使用できるヘッダーファイルに素早く変換できます。さらに、このツールはスタインハート・ハート係数を計算できます。Excelデータから任意の3つの非一致点を選択することで、スタインハート・ハート係数A、B、Cを計算できます。このツールは、コマンドラインモードとグラフィカルインターフェースモードの両方に対応しています。

LibDriver NTCツールは/utilsディレクトリにあります。ntc_utils.pyはLibDriver NTCツールプログラム、ntc_table_template.xlsxは様々なモデルのNTC温度抵抗データをインポートするためのExcelデータテンプレート、requirements.txtはLibDriver NTCツールの依存関係を記述したファイルです。resourceフォルダには、LibDriver NTCツールのリソースが格納されています。ツール実行中に、ユーザー設定を記録するためのconfig.json設定ファイルが生成されます。生成されるヘッダーファイル（デフォルトではdriver_ntc_table.hという名前）は、常に/utilsディレクトリに作成されます。

1. 依存関係のインストール

    ```shell
    # go to the LibDriver NTC Tool dir
    cd utils
    
    # install requirements
    pip install -r requirements.txt
    ```

2. ヘルプコマンド

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

1. ルックアップテーブルメソッド用のCヘッダーファイルを生成する

   ```shell
   python ntc_utils.py -c header -i ntc_table_template.xlsx -o driver_ntc_table.h

   c header generator
   input excel file: ntc_table_template.xlsx
   output c header file: driver_ntc_table.h
   success
   ```

   -c header ルックアップテーブルメソッドを生成するCヘッダーファイルコマンド

   -i Excelファイルのパス, Excelテンプレート内のtemperature_c、resistance_kohm、ntcテーブルは変更しないでください

   -o /utils ディレクトリに固定されている C ヘッダーファイルの名前を出力します

2. スタインハート・ハート係数を計算する

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

   -c steinhart-hart スタインハート・ハート係数を計算するためのコマンド

   -i Excelファイルのパス, Excelテンプレート内のtemperature_c、resistance_kohm、ntcテーブルは変更しないでください

   -t0 スタインハート係数の計算温度点0、3つの点は同じであってはならず、温度点を含むExcelファイルに含まれている必要があります

   -t1 スタインハート係数の計算温度点1、3つの点は同じであってはならず、温度点を含むExcelファイルに含まれている必要があります

   -t2 スタインハート係数の計算温度点2、3つの点は同じであってはならず、温度点を含むExcelファイルに含まれている必要があります

#### GUI

プログラムを開始する

```shell
python ntc_utils.py -c gui
```

-c gui プログラムを開始するコマンド

起動画面は以下のとおりです。右上隅に言語切り替えリストがあり、English，简体中文，繁體中文，日本語， Deutsch and 한국어.

<div align=center>
<img src="/doc/image/ja/main.png"/>
</div>

「Excelをインポート」をクリックすると、プログラムがExcelデータをインポートし、抵抗-温度特性曲線をプロットします。エクスポートするヘッダーファイルの名前を設定し、「Cヘッダーファイルを生成」をクリックすると、ルックアップテーブルCヘッダーファイルを生成し、ファイルが保存されているディレクトリを開くことができます。

<div align=center>
<img src="/doc/image/ja/table.png"/>
</div>

温度点T0、T1、T2を計算するためのスタインハート・ハート係数を設定します。「スタインハート・ハート係数を計算」をクリックすると、これら3点に基づいてスタインハート・ハート係数が計算され、結果が表示されます。同時に、これらのパラメータに基づいて抵抗-温度特性曲線をプロットし、元のデータとの差異を比較します。

<div align=center>
<img src="/doc/image/ja/sh.png"/>
</div>

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/ntc/index.html](https://www.libdriver.com/docs/ntc/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。