### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(ntc REQUIRED)
```

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

4. Run ntc calculate function,  number0 means input array, ohm means resistor in ohm, value means beta formula beta value or steinhart-hart value.

    ```shell
    ntc (-e calc | --example=calc) [--algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>] [--resistor=<number0,...>] [--beta=<value>] [--r25-resistor=<ohm>] [--steinhart-hart-a=<value>] [--steinhart-hart-b=<value>] [--steinhart-hart-c=<value>] [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>]
    ```

#### 3.2 Command Example

```shell
./ntc -i

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
./ntc -p

ntc: ADC connected to none.
```


```shell
./ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=NONE

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
./ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=FIRST_ORDER_LAG

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
./ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MEDIAN

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is median.
ntc: median filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.96C.
```

```shell
./ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=ANTI_SPIKE_AVERAGE

ntc: algorithm is beta formula.
ntc: beta formula beta value is 3950.00.
ntc: beta formula r25 resistor is 10000.00 ohm.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.25C.
```

```shell
./ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MOVING_AVERAGE

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
./ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=WEIGHTED_MOVING_AVERAGE

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
./ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=LIMITING

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
./ntc -e calc --algorithm=BETA-FORMULA --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --beta=3950.0 --r25-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=KALMAN

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
./ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=NONE

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
./ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=FIRST_ORDER_LAG

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
./ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MEDIAN

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
./ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=ANTI_SPIKE_AVERAGE

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
./ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MOVING_AVERAGE

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
./ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=WEIGHTED_MOVING_AVERAGE

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
./ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=LIMITING

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
./ntc -e calc --algorithm=STEINHART-HART --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --steinhart-hart-a=1.129148e-3 --steinhart-hart-b=2.34125e-4 --steinhart-hart-c=8.76741e-8 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=KALMAN

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
./ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=NONE

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
./ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=FIRST_ORDER_LAG

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
./ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MEDIAN

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is median.
ntc: median filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.96C.
```

```shell
./ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=ANTI_SPIKE_AVERAGE

ntc: algorithm is lookup table.
ntc: table length is 301.
ntc: filter is anti spike average.
ntc: anti spike average filter length is 5.
ntc: input resistor is 10000.00ohm 9000.00ohm 11000.00ohm 10020.00ohm 11000.00ohm 
ntc: temperature is 24.25C.
```

```shell
./ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=MOVING_AVERAGE

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
./ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=WEIGHTED_MOVING_AVERAGE

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
./ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=LIMITING

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
./ntc -e calc --algorithm=LOOKUP-TABLE-B3950-10K --circuit=VCC_R_NTC_GND --fixed-resistor=10000.0 --resistor=10000.0,9000.0,11000.0,10020.0,11000.0 --filter=KALMAN

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
./ntc -h

Usage:
  ntc (-i | --information)
  ntc (-h | --help)
  ntc (-p | --port)
  ntc (-e calc | --example=calc) [--algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>]
      [--resistor=<number0,...>]
      [--beta=<value>] [--r25-resistor=<ohm>]
      [--steinhart-hart-a=<value>] [--steinhart-hart-b=<value>] [--steinhart-hart-c=<value>]
      [--filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE | WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>]

Options:
      --algorithm=<BETA-FORMULA | STEINHART-HART | LOOKUP-TABLE-B3950-10K>
                                 Set the algorithm.([default: beta-formula])
      --beta=<value>             Set the beta value.([default: 3950.0f])
  -e <calc>, --example=<calc>
                                 Run the driver example.
      --filter=<NONE | FIRST_ORDER_LAG | MEDIAN | ANTI_SPIKE_AVERAGE | MOVING_AVERAGE |
                WEIGHTED_MOVING_AVERAGE | LIMITING | KALMAN>
                                 Set the filter.([default: NONE])
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
  -p, --port                     Display the pin connections of the current board.
      --r25-resistor=<ohm>       Set the R25 resistor.([default: 10000.0f])
      --resistor=<number0,...>   Set the resistor values.
      --steinhart-hart-a=<value> Set the Steinhart-Hart A coefficient.([default: 1.129148e-3])
      --steinhart-hart-b=<value> Set the Steinhart-Hart B coefficient.([default: 2.34125e-4])
      --steinhart-hart-c=<value> Set the Steinhart-Hart C coefficient.([default: 8.76741e-8])
```
