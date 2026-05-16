### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

ADC Pin: ADC PA0.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. NTC

#### 3.1 Command Instruction

1. Show ntc chip and driver information.

    ```shell
    ntc (-i | --information)  
    ```

2. Show ntc help.

    ```shell
    ntc (-h | --help)        
    ```

3. Show ntc pin connections of the current board.

    ```shell
    ntc (-p | --port)              
    ```

4. Run ntc beta formula read test, ohm means resistor in ohm, value means beta formula beta value, num means test times.

    ```shell
    ntc (-t read-beta-formula | --test=read-beta-formula) [--circuit=<VCC_NTC_R_GND | VCC_R_NTC_GND>] [--fixed-resistor=<ohm>] [--beta=<value>] [--r25-resistor=<ohm>] [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>] [--times=<num>]
    ```

5. Run ntc steinhart hart read test,  ohm means resistor in ohm, value means steinhart-hart value, num means read times.

    ```shell
    ntc (-t read-steinhart-hart | --test=read-steinhart-hart) [--circuit=<VCC_NTC_R_GND | VCC_R_NTC_GND>] [--fixed-resistor=<ohm>] [--steinhart-hart-a=<value>] [--steinhart-hart-b=<value>] [--steinhart-hart-c=<value>] [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>] [--times=<num>] 
    ```
    
6. Run ntc lookup table b3950 10k read test,  ohm means resistor in ohm, num means read times.

    ```shell
    ntc (-t read-lookup-table-b3950-10k | --test=read-lookup-table-b3950-10k) [--circuit=<VCC_NTC_R_GND | VCC_R_NTC_GND>] [--fixed-resistor=<ohm>] [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>] [--times=<num>]
    ```

7. Run ntc read function,  ohm means resistor in ohm, value means beta formula beta value or steinhart-hart value, num means read times.

    ```shell
    ntc (-e read | --example=read) [--algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>] [--circuit=<VCC_NTC_R_GND | VCC_R_NTC_GND>] [--fixed-resistor=<ohm>] [--beta=<value>] [--r25-resistor=<ohm>]  [--steinhart-hart-a=<value>] [--steinhart-hart-b=<value>] [--steinhart-hart-c=<value>] [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>] [--times=<num>]
    ```

8. Run ntc calculate function,  number0 means input array, ohm means resistor in ohm, value means beta formula beta value or steinhart-hart value.

    ```shell
    ntc (-e calc | --example=calc) [--algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>] [--resistor=<number0,...>] [--beta=<value>] [--r25-resistor=<ohm>] [--steinhart-hart-a=<value>] [--steinhart-hart-b=<value>] [--steinhart-hart-c=<value>] [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>]
    ```

#### 3.2 Command Example

```shell
ntc -i

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
```

```shell
ntc -p

ntc: ADC connected to GPIOA PIN0.
```

```shell
ntc -t read-beta-formula --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=NONE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is none.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.80C.
ntc: resistance is 7758.02ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.80C.
ntc: finish read test.
```

```shell
ntc -t read-beta-formula --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=FIRST_ORDER_LAG --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is first order lag.
ntc: first order lag filter alpha is 0.80.
ntc: resistance is 7781.15ohm.
ntc: temperature is 30.75C.
ntc: resistance is 7774.98ohm.
ntc: temperature is 30.77C.
ntc: resistance is 7773.75ohm.
ntc: temperature is 30.78C.
ntc: finish read test.
```

```shell
ntc -t read-beta-formula --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=MEDIAN --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is median.
ntc: median filter length is 5.
ntc: resistance is 7758.02ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.80C.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.80C.
ntc: finish read test.
```

```shell
ntc -t read-beta-formula --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=ANTI_SPIKE_AVERAGE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: resistance is 7770.87ohm.
ntc: temperature is 30.79C.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.80C.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.80C.
ntc: finish read test.
```

```shell
ntc -t read-beta-formula --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=MOVING_AVERAGE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is moving average.
ntc: moving average filter length is 5.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.78C.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.80C.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.80C.
ntc: finish read test.
```

```shell
ntc -t read-beta-formula --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=WEIGHTED_MOVING_AVERAGE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is weighted moving average.
ntc: weighted moving average filter is 1.00 2.00 3.00 4.00 5.00.
ntc: resistance is 7758.02ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7758.02ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7761.88ohm.
ntc: temperature is 30.81C.
ntc: finish read test.
```

```shell
ntc -t read-beta-formula --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=LIMITING --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is limiting.
ntc: limiting filter is 10.00C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.78C.
ntc: resistance is 7758.02ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.78C.
ntc: finish read test.
```

```shell
ntc -t read-beta-formula --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=KALMAN --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is kalman.
ntc: kalman q(process noise covariance) is 0.005.
ntc: kalman r(measurement noise covariance) is 0.50.
ntc: kalman p(estimation error covariance) is 1.00.
ntc: kalman x(estimated value) is read the current adc.
ntc: resistance is 7770.88ohm.
ntc: temperature is 30.79C.
ntc: resistance is 7771.91ohm.
ntc: temperature is 30.78C.
ntc: resistance is 7774.62ohm.
ntc: temperature is 30.77C.
ntc: finish read test.
```

```shell
ntc -t read-steinhart-hart --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=NONE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is none.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.84C.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.86C.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.86C.
ntc: finish read test.
```

```shell
ntc -t read-steinhart-hart --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=FIRST_ORDER_LAG --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is first order lag.
ntc: first order lag filter alpha is 0.80.
ntc: resistance is 7788.88ohm.
ntc: temperature is 30.79C.
ntc: resistance is 7782.70ohm.
ntc: temperature is 30.81C.
ntc: resistance is 7787.64ohm.
ntc: temperature is 30.80C.
ntc: finish read test.
```

```shell
ntc -t read-steinhart-hart --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=MEDIAN --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is median.
ntc: median filter length is 5.
ntc: resistance is 7796.61ohm.
ntc: temperature is 30.77C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.84C.
ntc: resistance is 7796.61ohm.
ntc: temperature is 30.77C.
ntc: finish read test.
```

```shell
ntc -t read-steinhart-hart --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=ANTI_SPIKE_AVERAGE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: resistance is 7778.58ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7786.31ohm.
ntc: temperature is 30.80C.
ntc: resistance is 7778.58ohm.
ntc: temperature is 30.82C.
ntc: finish read test
```

```shell
ntc -t read-steinhart-hart --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=MOVING_AVERAGE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is moving average.
ntc: moving average filter length is 5.
ntc: resistance is 7796.61ohm.
ntc: temperature is 30.77C.
ntc: resistance is 7777.32ohm.
ntc: temperature is 30.83C.
ntc: resistance is 7778.60ohm.
ntc: temperature is 30.82C.
ntc: finish read test.
```

```shell
ntc -t read-steinhart-hart --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=WEIGHTED_MOVING_AVERAGE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is weighted moving average.
ntc: weighted moving average filter is 1.00 2.00 3.00 4.00 5.00.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.84C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.84C.
ntc: resistance is 7781.16ohm.
ntc: temperature is 30.82C.
ntc: finish read test.
```

```shell
ntc -t read-steinhart-hart --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=LIMITING --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is limiting.
ntc: limiting filter is 10.00C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.84C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.84C.
ntc: resistance is 7788.88ohm.
ntc: temperature is 30.79C.
ntc: finish read test.
```

```shell
ntc -t read-steinhart-hart --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=KALMAN --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is kalman.
ntc: kalman q(process noise covariance) is 0.005.
ntc: kalman r(measurement noise covariance) is 0.50.
ntc: kalman p(estimation error covariance) is 1.00.
ntc: kalman x(estimated value) is read the current adc.
ntc: resistance is 7783.75ohm.
ntc: temperature is 30.81C.
ntc: resistance is 7785.82ohm.
ntc: temperature is 30.80C.
ntc: resistance is 7782.20ohm.
ntc: temperature is 30.81C.
ntc: finish read test.
```

```shell
ntc -t read-lookup-table-b3950-10k --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --filter=NONE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: table length is 301.
ntc: filter is none.
ntc: resistance is 7765.73ohm.
ntc: temperature is 30.84C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7788.88ohm.
ntc: temperature is 30.77C.
ntc: finish read test.
```

```shell
ntc -t read-lookup-table-b3950-10k --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --filter=FIRST_ORDER_LAG --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: table length is 301.
ntc: filter is first order lag.
ntc: first order lag filter alpha is 0.80.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7785.79ohm.
ntc: temperature is 30.78C.
ntc: resistance is 7782.08ohm.
ntc: temperature is 30.79C.
ntc: finish read test.
```

```shell
ntc -t read-lookup-table-b3950-10k --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --filter=MEDIAN --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: table length is 301.
ntc: filter is median.
ntc: median filter length is 5.
ntc: resistance is 7781.15ohm.
ntc: temperature is 30.79C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.82C.
ntc: finish read test.
```

```shell
ntc -t read-lookup-table-b3950-10k --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --filter=ANTI_SPIKE_AVERAGE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: table length is 301.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7770.87ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7776.01ohm.
ntc: temperature is 30.81C.
ntc: finish read test.
```

```shell
ntc -t read-lookup-table-b3950-10k --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --filter=MOVING_AVERAGE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: table length is 301.
ntc: filter is moving average.
ntc: moving average filter length is 5.
ntc: resistance is 7781.15ohm.
ntc: temperature is 30.79C.
ntc: resistance is 7781.15ohm.
ntc: temperature is 30.79C.
ntc: resistance is 7778.58ohm.
ntc: temperature is 30.80C.
ntc: finish read test.
```

```shell
ntc -t read-lookup-table-b3950-10k --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --filter=WEIGHTED_MOVING_AVERAGE --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: table length is 301.
ntc: filter is weighted moving average.
ntc: weighted moving average filter is 1.00 2.00 3.00 4.00 5.00.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7783.73ohm.
ntc: temperature is 30.79C.
ntc: resistance is 7774.73ohm.
ntc: temperature is 30.81C.
ntc: finish read test.
```

```shell
ntc -t read-lookup-table-b3950-10k --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --filter=LIMITING --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: table length is 301.
ntc: filter is limiting.
ntc: limiting filter is 10.00C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.82C.
ntc: resistance is 7758.02ohm.
ntc: temperature is 30.86C.
ntc: resistance is 7773.44ohm.
ntc: temperature is 30.82C.
ntc: finish read test.
```

```shell
ntc -t read-lookup-table-b3950-10k --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --filter=KALMAN --times=3

ntc: chip is General NTC.
ntc: manufacturer is General.
ntc: interface is ADC.
ntc: driver version is 1.0.
ntc: min supply voltage is 1.8V.
ntc: max supply voltage is 5.5V.
ntc: max current is 1.00mA.
ntc: max temperature is 250.0C.
ntc: min temperature is -50.0C.
ntc: start read test.
ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: table length is 301.
ntc: filter is kalman.
ntc: kalman q(process noise covariance) is 0.005.
ntc: kalman r(measurement noise covariance) is 0.50.
ntc: kalman p(estimation error covariance) is 1.00.
ntc: kalman x(estimated value) is read the current adc.
ntc: resistance is 7768.29ohm.
ntc: temperature is 30.83C.
ntc: resistance is 7770.37ohm.
ntc: temperature is 30.83C.
ntc: resistance is 7773.53ohm.
ntc: temperature is 30.82C.
ntc: finish read test.
```

```shell
ntc -e read --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=NONE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is none.
ntc: 1/3.
ntc: temperature is 31.17C.
ntc: 2/3.
ntc: temperature is 31.20C.
ntc: 3/3.
ntc: temperature is 31.20C.
```

```shell
ntc -e read --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=FIRST_ORDER_LAG --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is first order lag.
ntc: first order lag filter alpha is 0.80.
ntc: 1/3.
ntc: temperature is 31.22C.
ntc: 2/3.
ntc: temperature is 31.20C.
ntc: 3/3.
ntc: temperature is 31.22C.
```

```shell
ntc -e read --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=MEDIAN --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is median.
ntc: median filter length is 5.
ntc: 1/3.
ntc: temperature is 31.41C.
ntc: 2/3.
ntc: temperature is 31.43C.
ntc: 3/3.
ntc: temperature is 31.43C.
```

```shell
ntc -e read --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=ANTI_SPIKE_AVERAGE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: 1/3.
ntc: temperature is 31.41C.
ntc: 2/3.
ntc: temperature is 31.41C.
ntc: 3/3.
ntc: temperature is 31.46C.
```

```shell
ntc -e read --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=MOVING_AVERAGE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is moving average.
ntc: moving average filter length is 5.
ntc: 1/3.
ntc: temperature is 31.43C.
ntc: 2/3.
ntc: temperature is 31.44C.
ntc: 3/3.
ntc: temperature is 31.43C.
```

```shell
ntc -e read --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=WEIGHTED_MOVING_AVERAGE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is weighted moving average.
ntc: weighted moving average filter is 1.00 2.00 3.00 4.00 5.00.
ntc: 1/3.
ntc: temperature is 31.45C.
ntc: 2/3.
ntc: temperature is 31.45C.
ntc: 3/3.
ntc: temperature is 31.44C.
```

```shell
ntc -e read --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=LIMITING --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is limiting.
ntc: limiting filter is 10.00C.
ntc: 1/3.
ntc: temperature is 31.45C.
ntc: 2/3.
ntc: temperature is 31.50C.
ntc: 3/3.
ntc: temperature is 31.45C.
```

```shell
ntc -e read --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --filter=KALMAN --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is kalman.
ntc: kalman q(process noise covariance) is 0.005.
ntc: kalman r(measurement noise covariance) is 0.50.
ntc: kalman p(estimation error covariance) is 1.00.
ntc: kalman x(estimated value) is read the current adc.
ntc: 1/3.
ntc: temperature is 31.59C.
ntc: 2/3.
ntc: temperature is 31.59C.
ntc: 3/3.
ntc: temperature is 31.59C.
```

```shell
ntc -e read --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=NONE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is none.
ntc: 1/3.
ntc: temperature is 31.33C.
ntc: 2/3.
ntc: temperature is 31.36C.
ntc: 3/3.
ntc: temperature is 31.38C.
```

```shell
ntc -e read --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=FIRST_ORDER_LAG --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is first order lag.
ntc: first order lag filter alpha is 0.80.
ntc: 1/3.
ntc: temperature is 31.31C.
ntc: 2/3.
ntc: temperature is 31.31C.
ntc: 3/3.
ntc: temperature is 31.29C.
```

```shell
ntc -e read --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=MEDIAN --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is median.
ntc: median filter length is 5.
ntc: 1/3.
ntc: temperature is 31.26C.
ntc: 2/3.
ntc: temperature is 31.24C.
ntc: 3/3.
ntc: temperature is 31.24C.
```

```shell
ntc -e read --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=ANTI_SPIKE_AVERAGE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: 1/3.
ntc: temperature is 31.21C.
ntc: 2/3.
ntc: temperature is 31.22C.
ntc: 3/3.
ntc: temperature is 31.22C.
```

```shell
ntc -e read --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=MOVING_AVERAGE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is moving average.
ntc: moving average filter length is 5.
ntc: 1/3.
ntc: temperature is 31.14C.
ntc: 2/3.
ntc: temperature is 31.16C.
ntc: 3/3.
ntc: temperature is 31.15C.
```

```shell
ntc -e read --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=WEIGHTED_MOVING_AVERAGE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is weighted moving average.
ntc: weighted moving average filter is 1.00 2.00 3.00 4.00 5.00.
ntc: 1/3.
ntc: temperature is 31.10C.
ntc: 2/3.
ntc: temperature is 31.14C.
ntc: 3/3.
ntc: temperature is 31.13C.
```

```shell
ntc -e read --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=LIMITING --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is limiting.
ntc: limiting filter is 10.00C.
ntc: 1/3.
ntc: temperature is 31.05C.
ntc: 2/3.
ntc: temperature is 31.10C.
ntc: 3/3.
ntc: temperature is 31.07C.
```

```shell
ntc -e read --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --filter=KALMAN --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is kalman.
ntc: kalman q(process noise covariance) is 0.005.
ntc: kalman r(measurement noise covariance) is 0.50.
ntc: kalman p(estimation error covariance) is 1.00.
ntc: kalman x(estimated value) is read the current adc.
ntc: 1/3.
ntc: temperature is 31.06C.
ntc: 2/3.
ntc: temperature is 31.05C.
ntc: 3/3.
ntc: temperature is 31.05C.
```


```shell
ntc -e read --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --filter=NONE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is none.
ntc: 1/3.
ntc: temperature is 31.07C.
ntc: 2/3.
ntc: temperature is 31.07C.
ntc: 3/3.
ntc: temperature is 31.10C.
```

```shell
ntc -e read --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --filter=FIRST_ORDER_LAG --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is first order lag.
ntc: first order lag filter alpha is 0.80.
ntc: 1/3.
ntc: temperature is 31.05C.
ntc: 2/3.
ntc: temperature is 31.05C.
ntc: 3/3.
ntc: temperature is 31.05C.
```

```shell
ntc -e read --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --filter=MEDIAN --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is median.
ntc: median filter length is 5.
ntc: 1/3.
ntc: temperature is 31.07C.
ntc: 2/3.
ntc: temperature is 31.07C.
ntc: 3/3.
ntc: temperature is 31.07C.
```

```shell
ntc -e read --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --filter=ANTI_SPIKE_AVERAGE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: 1/3.
ntc: temperature is 31.05C.
ntc: 2/3.
ntc: temperature is 31.09C.
ntc: 3/3.
ntc: temperature is 31.06C.
```

```shell
ntc -e read --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --filter=MOVING_AVERAGE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is moving average.
ntc: moving average filter length is 5.
ntc: 1/3.
ntc: temperature is 31.05C.
ntc: 2/3.
ntc: temperature is 31.06C.
ntc: 3/3.
ntc: temperature is 31.04C.
```

```shell
ntc -e read --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --filter=WEIGHTED_MOVING_AVERAGE --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is weighted moving average.
ntc: weighted moving average filter is 1.00 2.00 3.00 4.00 5.00.
ntc: 1/3.
ntc: temperature is 31.07C.
ntc: 2/3.
ntc: temperature is 31.09C.
ntc: 3/3.
ntc: temperature is 31.08C.
```

```shell
ntc -e read --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --filter=LIMITING --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is limiting.
ntc: limiting filter is 10.00C.
ntc: 1/3.
ntc: temperature is 31.10C.
ntc: 2/3.
ntc: temperature is 31.07C.
ntc: 3/3.
ntc: temperature is 31.05C.
```

```shell
ntc -e read --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --filter=KALMAN --times=3

ntc: circuit is vcc -> r_fixed -> ntc -> gnd.
ntc: circuit fixed resistor is 10000.00 ohm.
ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is kalman.
ntc: kalman q(process noise covariance) is 0.005.
ntc: kalman r(measurement noise covariance) is 0.50.
ntc: kalman p(estimation error covariance) is 1.00.
ntc: kalman x(estimated value) is read the current adc.
ntc: 1/3.
ntc: temperature is 31.06C.
ntc: 2/3.
ntc: temperature is 31.06C.
ntc: 3/3.
ntc: temperature is 31.05C.
```


```shell
ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=NONE

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is none.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 27.39C.
ntc: temperature is 22.87C.
ntc: temperature is 24.96C.
ntc: temperature is 22.87C.
```

```shell
ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=FIRST_ORDER_LAG

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is first order lag.
ntc: first order lag filter alpha is 0.80.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 26.89C.
ntc: temperature is 23.61C.
ntc: temperature is 24.68C.
ntc: temperature is 23.22C.
```

```shell
ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MEDIAN

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is median.
ntc: median filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.96C.
```

```shell
ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=ANTI_SPIKE_AVERAGE

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.25C.
```

```shell
ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MOVING_AVERAGE

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is moving average.
ntc: moving average filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 26.16C.
ntc: temperature is 25.00C.
ntc: temperature is 24.99C.
ntc: temperature is 24.55C.
```

```shell
ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=WEIGHTED_MOVING_AVERAGE

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is weighted moving average.
ntc: weighted moving average filter is 1.00 2.00 3.00 4.00 5.00.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 26.56C.
ntc: temperature is 24.63C.
ntc: temperature is 24.76C.
ntc: temperature is 24.11C.
```

```shell
ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=LIMITING

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is limiting.
ntc: limiting filter is 10.00C.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 27.39C.
ntc: temperature is 22.87C.
ntc: temperature is 24.96C.
ntc: temperature is 22.87C.
```

```shell
ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=KALMAN

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is kalman.
ntc: kalman q(process noise covariance) is 0.005.
ntc: kalman r(measurement noise covariance) is 0.50.
ntc: kalman p(estimation error covariance) is 1.00.
ntc: kalman x(estimated value) is 0.00.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 34.37C.
ntc: temperature is 31.26C.
ntc: temperature is 28.42C.
ntc: temperature is 27.56C.
ntc: temperature is 26.55C.
```


```shell
ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=NONE

ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is none.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 27.42C.
ntc: temperature is 22.84C.
ntc: temperature is 24.95C.
ntc: temperature is 22.84C.
```

```shell
ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=FIRST_ORDER_LAG

ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is first order lag.
ntc: first order lag filter alpha is 0.80.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 26.91C.
ntc: temperature is 23.59C.
ntc: temperature is 24.67C.
ntc: temperature is 23.19C.
```

```shell
ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MEDIAN

ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is median.
ntc: median filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.95C.
```

```shell
ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=ANTI_SPIKE_AVERAGE

ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.24C.
```

```shell
ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MOVING_AVERAGE

ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is moving average.
ntc: moving average filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 26.17C.
ntc: temperature is 25.00C.
ntc: temperature is 24.99C.
ntc: temperature is 24.54C.
```

```shell
ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=WEIGHTED_MOVING_AVERAGE

ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is weighted moving average.
ntc: weighted moving average filter is 1.00 2.00 3.00 4.00 5.00.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 26.58C.
ntc: temperature is 24.62C.
ntc: temperature is 24.75C.
ntc: temperature is 24.10C.
```

```shell
ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=LIMITING

ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is limiting.
ntc: limiting filter is 10.00C.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 27.42C.
ntc: temperature is 22.84C.
ntc: temperature is 24.95C.
ntc: temperature is 22.84C.
```

```shell
ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=KALMAN

ntc: algorithm is steinhart hart.
ntc: steinhart hart A is 1.129148e-03.
ntc: steinhart hart B is 2.341250e-04.
ntc: steinhart hart C is 8.767410e-08.
ntc: filter is kalman.
ntc: kalman q(process noise covariance) is 0.005.
ntc: kalman r(measurement noise covariance) is 0.50.
ntc: kalman p(estimation error covariance) is 1.00.
ntc: kalman x(estimated value) is 0.00.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 34.46C.
ntc: temperature is 31.32C.
ntc: temperature is 28.45C.
ntc: temperature is 27.59C.
ntc: temperature is 26.57C.
```


```shell
ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=NONE

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is none.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 27.41C.
ntc: temperature is 22.85C.
ntc: temperature is 24.96C.
ntc: temperature is 22.85C.
```

```shell
ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=FIRST_ORDER_LAG

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is first order lag.
ntc: first order lag filter alpha is 0.80.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 26.90C.
ntc: temperature is 23.60C.
ntc: temperature is 24.68C.
ntc: temperature is 23.21C.
```

```shell
ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MEDIAN

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is median.
ntc: median filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.96C.
```

```shell
ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=ANTI_SPIKE_AVERAGE

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.25C.
```

```shell
ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MOVING_AVERAGE

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is moving average.
ntc: moving average filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 26.17C.
ntc: temperature is 25.00C.
ntc: temperature is 24.99C.
ntc: temperature is 24.55C.
```

```shell
ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=WEIGHTED_MOVING_AVERAGE

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is weighted moving average.
ntc: weighted moving average filter is 1.00 2.00 3.00 4.00 5.00.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 26.58C.
ntc: temperature is 24.63C.
ntc: temperature is 24.76C.
ntc: temperature is 24.10C.
```

```shell
ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=LIMITING

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is limiting.
ntc: limiting filter is 10.00C.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 25.00C.
ntc: temperature is 27.41C.
ntc: temperature is 22.85C.
ntc: temperature is 24.96C.
ntc: temperature is 22.85C.
```

```shell
ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=KALMAN

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is kalman.
ntc: kalman q(process noise covariance) is 0.005.
ntc: kalman r(measurement noise covariance) is 0.50.
ntc: kalman p(estimation error covariance) is 1.00.
ntc: kalman x(estimated value) is 0.00.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 34.42C.
ntc: temperature is 31.30C.
ntc: temperature is 28.44C.
ntc: temperature is 27.58C.
ntc: temperature is 26.57C.
```


```shell
ntc -h

Usage:
  ntc (-i | --information)
  ntc (-h | --help)
  ntc (-p | --port)
  ntc (-t read-beta-formula | --test=read-beta-formula) [--circuit=<VCC_NTC_R_GND | VCC_R_NTC_GND>]
      [--fixed-resistor=<ohm>] [--beta=<value>] [--r25-resistor=<ohm>]
      [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>]
      [--times=<num>]
  ntc (-t read-steinhart-hart | --test=read-steinhart-hart) [--circuit=<VCC_NTC_R_GND | VCC_R_NTC_GND>]
      [--fixed-resistor=<ohm>] [--steinhart-hart-a=<value>] [--steinhart-hart-b=<value>] [--steinhart-hart-c=<value>]
      [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>]
      [--times=<num>]
  ntc (-t read-lookup-table-b3950-10k | --test=read-lookup-table-b3950-10k) [--circuit=<VCC_NTC_R_GND | VCC_R_NTC_GND>]
      [--fixed-resistor=<ohm>]
      [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>]
      [--times=<num>]
  ntc (-e read | --example=read) [--algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>]
      [--circuit=<VCC_NTC_R_GND | VCC_R_NTC_GND>] [--fixed-resistor=<ohm>]
      [--beta=<value>] [--r25-resistor=<ohm>]
      [--steinhart-hart-a=<value>] [--steinhart-hart-b=<value>] [--steinhart-hart-c=<value>]
      [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>]
      [--times=<num>]
  ntc (-e calc | --example=calc) [--algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>]
      [--resistor=<number0,...>]
      [--beta=<value>] [--r25-resistor=<ohm>]
      [--steinhart-hart-a=<value>] [--steinhart-hart-b=<value>] [--steinhart-hart-c=<value>]
      [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>]

Options:
      --algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>
                                 Set the algorithm.([default: beta-formula])
      --beta=<value>             Set the beta value.([default: 3950.0f])
      --circuit=<VCC_NTC_R_GND | VCC_R_NTC_GND>
                                 Set the circuit type.([default: VCC_R_NTC_GND])
  -e <read | calc>, --example=<read | calc>
                                 Run the driver example.
      --fixed-resistor=<ohm>     Set the fixed resistor.([default: 10000.0f])
      --filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE |
                WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>
                                 Set the filter.([default: NONE])
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
  -p, --port                     Display the pin connections of the current board.
      --r25-resistor=<ohm>       Set the R25 resistor.([default: 10000.0f])
      --resistor=<number0,...>   Set the resistor values.
  -t <read-beta-formula | read-steinhart-hart | read-lookup-table-b3950-10k>,
      --test=<read-beta-formula | read-steinhart-hart | read-lookup-table-b3950-10k>
                                 Run the driver test.
      --times=<num>              Set the running times.([default: 3])
      --steinhart-hart-a=<value> Set the Steinhart-Hart A coefficient.([default: 1.129148e-3])
      --steinhart-hart-b=<value> Set the Steinhart-Hart B coefficient.([default: 2.34125e-4])
      --steinhart-hart-c=<value> Set the Steinhart-Hart C coefficient.([default: 8.76741e-8])
```
