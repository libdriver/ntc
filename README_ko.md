[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTC

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntc/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

서미스터는 온도에 따라 저항값이 변하는 센서 저항기의 일종입니다. 서미스터는 가변 저항기의 범주에 속하며 서지 전류 제한기, 온도 센서, 재설정 가능 퓨즈, 자율 조절 히터 등 다양한 전자 부품에 널리 사용됩니다. 순수 금속을 사용하는 저항 온도계와 달리 서미스터는 일반적으로 세라믹이나 고분자 소재를 사용합니다.

LibDriver NTC는 LibDriver에서 출시한 NTC용 완벽한 기능을 갖춘 드라이버입니다. 이 드라이버는 룩업 테이블 온도 변환, B값 온도 변환, 스테인하트-하트 공식 온도 변환, NTC 개방 회로 감지, NTC 단락 회로 감지, 1차 지연 필터링, 중앙값 필터링, 펄스 간섭 방지 평균 필터링, 이동 평균 필터링, 가중 이동 평균 필터링, 진폭 제한 필터링, 칼만 필터링 및 기타 추가 기능을 제공합니다. LibDriver는 MISRA 규정을 준수합니다.

LibDriver NTC Tool은 LibDriver NTC를 위해 특별히 개발된 도구입니다. 이 도구를 사용하면 Excel 데이터를 기반으로 조회 테이블 방식에 필요한 헤더 파일을 신속하게 생성할 수 있습니다. 또한, 이 도구는 세 지점의 온도-저항 데이터를 기반으로 Steinhart-Hart 계수 A, B, C를 계산할 수 있습니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
  - [필터](#필터)
    - [1차지연필터](#1차지연필터)
    - [중앙값](#중앙값)
    - [스파이크방지평균](#스파이크방지평균)
    - [이동평균](#이동평균)
    - [가중이동평균](#가중이동평균)
    - [제한](#제한)
    - [칼만함수](#칼만함수)
  - [LibDriver NTC Tool](#LibDriver-NTC-Tool)
- [CLI](#CLI)
- [GUI](#GUI)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver NTC의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver NTC용 플랫폼 독립적인 ADC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver NTC드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver NTC프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver NTC오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 NTC데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

/utils에는 LibDriver NTC 도구가 포함되어 있습니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인ADC버스 템플릿을 참조하여 지정된 플랫폼에 대한ADC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 필터

LibDriver NTC는 1차 지연 필터링, 중앙값 필터링, 펄스 간섭 방지 평균 필터링, 이동 평균 필터링, 가중 이동 평균 필터링, 진폭 제한 필터링 및 칼만 필터링을 포함한 여러 NTC 필터링 알고리즘을 통합합니다. 원시 데이터에 적절한 필터링 처리를 적용하면 더 나은 출력 결과를 얻을 수 있습니다.

#### 1차지연필터

1차 지연 필터는 하드웨어 (RC) 저역 통과 필터링의 효과를 소프트웨어 알고리즘을 이용하여 모방하는 기술입니다. 현재 샘플링 값과 과거 필터링 결과의 가중 평균을 수행함으로써 고주파 노이즈와 글리치를 효과적으로 제거하고 유용한 신호를 유지하여 데이터 출력을 더욱 부드럽고 안정적으로 만듭니다.

$$
y(n) = \alpha \cdot x(n) + (1 - \alpha) \cdot y(n - 1)
$$

#### 중앙값

중앙값 필터링은 N번 연속으로 샘플링하고, 결과를 정렬한 후 중앙값을 선택하는 방식입니다. 이 방법을 통해 간헐적인 펄스 간섭을 효과적으로 억제할 수 있습니다.

$$
y(n) = med \{ x(n-k), ..., x(n), ..., x(n+k) \}
$$

#### 스파이크방지평균

펄스 간섭 저항 평균 필터는 "중앙값 필터링"과 "산술 평균 필터링"의 장점을 결합한 디지털 필터링 알고리즘입니다. 샘플링 대기열에서 최댓값과 최솟값을 제거한 후 평균값을 계산합니다. 이를 통해 펄스 형태의 버스트 간섭을 효과적으로 제거하고 주기적인 잡음을 평활화하여 샘플링 데이터의 안정성과 정확성을 보장합니다.

$$
y = \frac{1}{n-2} \left ( \sum\limits_{i = 1}^{n} {{{x}_{i}} - ({{x}_{(1)}} + {{x}_{(n)}})} \right )
$$

#### 이동평균

슬라이딩 평균 필터는 고정된 길이의 선입선출(FIFO) 큐를 유지하며, 매번 새로운 샘플을 추가하고 큐의 평균값을 계산합니다.

$$
y(n) = \frac{1}{N} \sum \limits_{i = 0}^{N-1}{x(n-i)}
$$

#### 가중이동평균

가중 이동 평균 필터는 이동 평균을 기반으로 하며, 현재 시간에 가까운 샘플일수록 더 높은 가중치를 부여합니다.

$$
y(n) = \frac {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}} \cdot x(n-i)}} {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}}}}
$$

#### 제한

제한 필터는 현재 샘플링 값과 이전 유효 값의 차이를 비교합니다. 차이가 설정된 임계값을 초과하면 해당 값은 폐기됩니다.

$$
\begin{gathered}
y(n) = \begin{cases} x(n) & |x(n) - y(n-1)| \le \Delta \\ y(n-1) & |x(n) - y(n-1)| > \Delta \end{cases}
\end{gathered}
$$

#### 칼만함수

칼만 필터는 시스템 모델 예측과 센서 측정 데이터를 결합하여 잡음이 섞인 시퀀스로부터 동적 시스템의 내부 상태를 효율적으로 추정하는 재귀 필터입니다. 센서 데이터 융합, 내비게이션, 제어 분야에서 널리 사용됩니다. 칼만 필터의 핵심은 "예측-업데이트"의 반복 과정을 통해 상태 추정의 평균 제곱 오차를 최소화하는 데 있습니다.

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

LibDriver NTC는 조회 테이블 방식에 필요한 헤더 파일을 신속하게 생성하는 도구를 제공합니다. 이 도구를 사용하면 Excel 파일을 LibDriver NTC에서 사용하기 적합한 헤더 파일로 빠르게 변환할 수 있습니다. 또한, 이 도구는 스테인하트-하트 계수를 계산할 수 있습니다. Excel 데이터에서 서로 겹치지 않는 세 점을 선택하면 스테인하트-하트 계수 A, B, C를 계산할 수 있습니다. 이 도구는 명령줄 모드와 그래픽 사용자 인터페이스 모드 모두를 제공합니다.

LibDriver NTC 도구는 /utils 디렉터리에 있습니다. ntc_utils.py는 LibDriver NTC 도구 프로그램이고, ntc_table_template.xlsx는 다양한 모델의 NTC 온도-저항 데이터를 가져올 수 있는 Excel 데이터 템플릿이며, requirements.txt는 LibDriver NTC 도구의 종속성을 설명하는 파일입니다. resource 폴더에는 LibDriver NTC 도구에 필요한 리소스가 포함되어 있습니다. 도구 작동 중에는 사용자 설정을 기록하기 위해 config.json 구성 파일이 생성됩니다. 생성된 헤더 파일은 기본적으로 driver_ntc_table.h라는 이름으로 /utils 디렉터리에 항상 생성됩니다.

1. 필수 구성 요소를 설치합니다.

    ```shell
    # go to the LibDriver NTC Tool dir
    cd utils
    
    # install requirements
    pip install -r requirements.txt
    ```
    
2. 도움말 명령
   
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

1. 조회 테이블 메서드에 대한 C 헤더 파일을 생성합니다.

   ```shell
   python ntc_utils.py -c header -i ntc_table_template.xlsx -o driver_ntc_table.h

   c header generator
   input excel file: ntc_table_template.xlsx
   output c header file: driver_ntc_table.h
   success
   ```

   -c header 조회 테이블 방식을 생성하는 C 헤더 파일 명령

   -i 엑셀 파일 경로를 지정하고, 엑셀 템플릿의 temperature_c, resistance_kohm, ntc 테이블을 수정하지 마십시오.

   -o /utils 디렉터리에 고정된 C 헤더 파일의 이름을 출력합니다.

2. 스테인하트-하트 계수를 계산하세요

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

   -c steinhart-hart 스테인하트-하트 계수를 계산하는 명령어

   -i 엑셀 파일 경로를 지정하고, 엑셀 템플릿의 temperature_c, resistance_kohm, ntc 테이블을 수정하지 마십시오.

   -t0 스테인하르트 하트 계수 계산 시 온도 지점 0을 포함한 세 지점은 서로 달라야 하며, 온도 지점이 포함된 엑셀 파일에 반드시 포함되어야 합니다.

   -t1 스테인하르트 하트 계수 계산 시 온도 지점 1, 세 지점은 서로 달라야 하며 온도 지점 정보가 포함된 엑셀 파일에 반드시 포함되어야 합니다.

   -t2 스테인하르트 하트 계수 계산 시 온도 지점 2, 세 지점은 서로 달라야 하며 온도 지점 정보가 포함된 엑셀 파일에 반드시 포함되어야 합니다.

#### GUI

프로그램을 시작하세요

```shell
python ntc_utils.py -c gui
```

-c gui 프로그램을 시작하는 명령어

시작 화면은 다음과 같습니다. 오른쪽 상단에는 언어 전환 목록이 있으며, 여기에서 영어, 중국어 간체, 중국어 번체, 일본어, 독일어 및 한국어를 선택할 수 있습니다.

<div align=center>
<img src="/doc/image/ko/main.png" width="750" height="561"/>
</div>

"Excel 가져오기"를 클릭하면 프로그램이 Excel 데이터를 가져와 저항-온도 특성 곡선을 그립니다. 내보낼 헤더 파일의 이름을 설정하고 "C 헤더 파일 생성"을 클릭하면 조회 테이블 C 헤더 파일을 생성하고 파일이 저장된 디렉토리를 열 수 있습니다.

<div align=center>
<img src="/doc/image/ko/table.png" width="750" height="561"/>
</div>

온도 지점 T0, T1, T2를 계산하기 위한 스테인하트-하트 계수를 설정합니다. "스테인하트-하트 계산"을 클릭하여 이 세 지점을 기반으로 스테인하트-하트 계수를 계산하고 결과를 표시합니다. 동시에 이러한 매개변수를 기반으로 저항-온도 특성 곡선을 그려 원본 데이터와의 차이를 비교합니다.

<div align=center>
<img src="/doc/image/ko/sh.png" width="750" height="561"/>
</div>

### 문서

온라인 문서: [https://www.libdriver.com/docs/ntc/index.html](https://www.libdriver.com/docs/ntc/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.