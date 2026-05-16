[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTC

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntc/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Ein Thermistor ist ein Sensorwiderstand, dessen Widerstandswert sich mit der Temperatur ändert. Thermistoren gehören zur Gruppe der variablen Widerstände und finden breite Anwendung in verschiedenen elektronischen Bauteilen, beispielsweise in Überspannungsschutzgeräten, Temperatursensoren, rückstellbaren Sicherungen und selbstregulierenden Heizungen. Im Gegensatz zu Widerstandsthermometern, die reine Metalle verwenden, bestehen Thermistoren typischerweise aus Keramik oder Polymeren.

LibDriver NTC ist ein umfassender NTC-Treiber von LibDriver. Er bietet Temperaturkonvertierung mittels Nachschlagetabelle, B-Wert und Steinhart-Hart-Formel, Leerlauf- und Kurzschlusserkennung für NTC-Sensoren, Verzögerungsfilter erster Ordnung, Medianfilter, Mittelwertfilter mit Impulsunterdrückung, gleitende Mittelwerte, gewichtete gleitende Mittelwerte, Amplitudenbegrenzungsfilter, Kalman-Filter und weitere Funktionen. LibDriver ist MISRA-konform.

Das LibDriver NTC Tool wurde speziell für LibDriver NTC entwickelt. Mit diesem Tool lassen sich schnell die für die Lookup-Tabellen-Methode benötigten Header-Dateien anhand von Excel-Daten generieren. Zusätzlich kann es die Steinhart-Hart-Koeffizienten A, B und C aus Temperatur-Widerstands-Daten von drei Punkten berechnen.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
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
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver NTC-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver NTC ADC.

/test enthält den Testcode des LibDriver NTC-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver NTC-Beispielcode.

/doc enthält das LibDriver NTC-Offlinedokument.

/Datenblatt enthält NTC-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

/utils enthält das LibDriver NTC Tool.

### Installieren

Verweisen Sie auf eine plattformunabhängige ADC-Schnittstellenvorlage und stellen Sie Ihren Plattform-ADC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

LibDriver NTC integriert verschiedene NTC-Filteralgorithmen, darunter Filterung erster Ordnung mit Verzögerung, Medianfilterung, Mittelwertfilterung zur Unterdrückung von Impulsstörungen, gleitende Mittelwertfilterung, gewichtete gleitende Mittelwertfilterung, Amplitudenbegrenzungsfilterung und Kalman-Filterung. Durch eine geeignete Filterverarbeitung der Rohdaten lassen sich oft bessere Ergebnisse erzielen.

#### FIRST_ORDER_LAG_FILTER

Der Verzögerungsfilter erster Ordnung ist ein Verfahren, das Softwarealgorithmen nutzt, um die Wirkung eines Hardware-RC-Tiefpassfilters zu simulieren. Durch die Berechnung eines gewichteten Mittelwerts aus dem aktuellen Abtastwert und den bisherigen Filterergebnissen werden hochfrequente Störungen und Aussetzer effektiv herausgefiltert, Nutzsignale erhalten und die Datenausgabe geglättet und stabilisiert.

$$
y(n) = \alpha \cdot x(n) + (1 - \alpha) \cdot y(n - 1)
$$

#### MEDIAN

Die Medianfilterung beinhaltet die N-fache Abtastung nacheinander, das Sortieren der Ergebnisse und die Auswahl des Medianwerts. Dadurch werden gelegentliche Impulsstörungen wirksam unterdrückt.

$$
y(n) = med \{ x(n-k), ..., x(n), ..., x(n+k) \}
$$

#### ANTI_SPIKE_AVERAGE

Der impulsstörresistente Mittelwertfilter ist ein digitaler Filteralgorithmus, der die Vorteile der Medianfilterung und der arithmetischen Mittelwertfilterung vereint. Nach dem Entfernen der Maximal- und Minimalwerte aus der Abtastfolge berechnet er den Mittelwert. Dadurch werden impulsartige Störungen effektiv eliminiert und periodisches Rauschen geglättet, was die Stabilität und Genauigkeit der Abtastdaten gewährleistet.

$$
y = \frac{1}{n-2} \left ( \sum\limits_{i = 1}^{n} {{{x}_{i}} - ({{x}_{(1)}} + {{x}_{(n)}})} \right )
$$

#### MOVING_AVERAGE

Der gleitende Mittelwertfilter verwendet eine Warteschlange mit fester Länge nach dem FIFO-Prinzip (First-In First-Out), fügt jedes Mal neue Abtastwerte hinzu und berechnet den Mittelwert der Warteschlange.

$$
y(n) = \frac{1}{N} \sum \limits_{i = 0}^{N-1}{x(n-i)}
$$

#### WEIGHTED_MOVING_AVERAGE

Basierend auf dem gleitenden Durchschnitt weist der gewichtete gleitende Durchschnittsfilter den Abtastwerten, die näher am aktuellen Zeitpunkt liegen, höhere Gewichte zu.

$$
y(n) = \frac {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}} \cdot x(n-i)}} {\sum \nolimits_{i = 0}^{N-1} {{{w}_{i}}}}
$$

#### LIMITING

Der Begrenzungsfilter vergleicht die Differenz zwischen dem aktuellen Abtastwert und dem vorherigen gültigen Wert. Überschreitet die Differenz den festgelegten Schwellenwert, wird der Wert verworfen.

$$
y(n) = \begin{cases} 
x(n) & |x(n) - y(n-1)| \le \Delta \\ 
y(n-1) & |x(n) - y(n-1)| > \Delta 
\end{cases}
$$

#### KALMAN

Der Kalman-Filter ist ein effizienter, rekursiver Filter, der den internen Zustand eines dynamischen Systems aus einer verrauschten Sequenz schätzt, indem er Systemmodellvorhersagen und Sensormessdaten kombiniert. Er findet breite Anwendung in den Bereichen Sensordatenfusion, Navigation und Regelungstechnik. Sein Kern besteht im iterativen Prozess der „Vorhersage-Aktualisierung“, der den mittleren quadratischen Fehler der Zustandsschätzung minimiert.

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

LibDriver NTC bietet ein Tool zur schnellen Generierung der für die Lookup-Tabellen-Methode benötigten Header-Dateien. Mit diesem Tool lassen sich Excel-Dateien schnell in Header-Dateien konvertieren, die mit LibDriver NTC kompatibel sind. Zusätzlich kann das Tool Steinhart-Hart-Koeffizienten berechnen. Durch Auswahl von drei beliebigen, nicht übereinstimmenden Punkten aus den Excel-Daten berechnet das Tool die Steinhart-Hart-Koeffizienten A, B und C. Das Tool bietet sowohl eine Kommandozeilen- als auch eine grafische Benutzeroberfläche.

Das LibDriver NTC-Tool befindet sich im Verzeichnis `/utils`. Die Datei `ntc_utils.py` ist das eigentliche Programm des LibDriver NTC-Tools, `ntc_table_template.xlsx` ist eine Excel-Datenvorlage zum Import von NTC-Temperatur-Widerstands-Daten verschiedener Modelle, und `requirements.txt` beschreibt die Abhängigkeiten des LibDriver NTC-Tools. Der Ordner `resource` enthält die benötigten Ressourcen. Während der Ausführung des Tools wird eine Konfigurationsdatei `config.json` generiert, um die Benutzereinstellungen zu speichern. Die generierte Header-Datei, standardmäßig `driver_ntc_table.h` genannt, wird immer im Verzeichnis `/utils` erstellt.

1. Abhängigkeiten installieren

    ```shell
    # go to the LibDriver NTC Tool dir
    cd utils
    
    # install requirements
    pip install -r requirements.txt
    ```
    
2. Hilfebefehl
   
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

1. Generieren Sie die C-Headerdatei für die Lookup-Tabellen-Methode

   ```shell
   python ntc_utils.py -c header -i ntc_table_template.xlsx -o driver_ntc_table.h

   c header generator
   input excel file: ntc_table_template.xlsx
   output c header file: driver_ntc_table.h
   success
   ```

   -c header generiert eine Lookup-Tabellenmethode C Headerdatei Befehl

   -i Pfad zur Excel-Datei; die Tabellen „Temperature_c“, „Resistance_kohm“ und „ntc“ in der Excel-Vorlage dürfen nicht verändert werden

   -o Gibt den Namen der C-Headerdatei aus, die sich im Verzeichnis /utils befindet

2. Berechnen Sie den Steinhart-Hart-Koeffizienten

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

   -c steinhart-hart Befehl zur Berechnung der Steinhart-Hart-Koeffizienten

   -i Pfad zur Excel-Datei; die Tabellen „Temperature_c“, „Resistance_kohm“ und „ntc“ in der Excel-Vorlage dürfen nicht verändert werden

   -t0 Steinhart-Hart-Koeffizientenberechnung: Temperaturpunkt 0, die drei Punkte dürfen nicht identisch sein und müssen in der Excel-Datei mit den Temperaturpunkten enthalten sein

   -t1 Steinhart-Hart-Koeffizientenberechnung: Temperaturpunkt 1, drei Punkte dürfen nicht gleich sein und müssen in der Excel-Datei mit den Temperaturpunkten enthalten sein

   -t2 Steinhart-Hart-Koeffizientenberechnung: Temperaturpunkt 2, drei Punkte dürfen nicht gleich sein und müssen in der Excel-Datei mit den Temperaturpunkten enthalten sein

#### GUI

Starten Sie das Programm

```shell
python ntc_utils.py -c gui
```

-c gui Befehl zum Starten des Programms

Die Startoberfläche sieht wie folgt aus. In der oberen rechten Ecke befindet sich eine Sprachauswahlliste, in der Sie English，简体中文，繁體中文，日本語， Deutsch and 한국어.

<div align=center>
<img src="/doc/image/de/main.png" width="750" height="561"/>
</div>

Klicken Sie auf „Excel importieren“. Das Programm importiert die Excel-Daten und erstellt die Widerstands-Temperatur-Kennlinie. Geben Sie den Namen der exportierten Header-Datei an, klicken Sie auf „C-Header-Datei generieren“, um eine Lookup-Tabellen-C-Header-Datei zu erstellen und das Verzeichnis zu öffnen, in dem die Datei gespeichert ist.

<div align=center>
<img src="/doc/image/de/table.png" width="750" height="561"/>
</div>

Legen Sie die Steinhart-Hart-Koeffizienten für die Berechnung der Temperaturpunkte T0, T1 und T2 fest. Klicken Sie auf „Steinhart-Hart berechnen“, um die Steinhart-Hart-Koeffizienten anhand dieser drei Punkte zu berechnen und die Ergebnisse anzuzeigen. Stellen Sie gleichzeitig die Widerstands-Temperatur-Kennlinie anhand dieser Parameter grafisch dar, um die Abweichungen von den Originaldaten zu vergleichen.

<div align=center>
<img src="/doc/image/de/sh.png" width="750" height="561"/>
</div>

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ntc/index.html](https://www.libdriver.com/docs/ntc/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.