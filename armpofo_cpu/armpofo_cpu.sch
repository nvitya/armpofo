EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_ST_STM32H7:STM32H750VBTx U?
U 1 1 5F84866B
P 4900 3900
F 0 "U?" H 4250 6450 50  0000 C CNN
F 1 "STM32H750VBTx" H 5450 1200 50  0000 C CNN
F 2 "Package_QFP:LQFP-100_14x14mm_P0.5mm" H 4200 1300 50  0001 R CNN
F 3 "https://www.st.com/resource/en/datasheet/stm32h750ib.pdf" H 4900 3900 50  0001 C CNN
	1    4900 3900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x16 J?
U 1 1 5F85E862
P 14200 5700
F 0 "J?" H 14280 5692 50  0000 L CNN
F 1 "Conn_01x16" H 14280 5601 50  0000 L CNN
F 2 "" H 14200 5700 50  0001 C CNN
F 3 "~" H 14200 5700 50  0001 C CNN
	1    14200 5700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x20 J?
U 1 1 5F85FE78
P 14200 3400
F 0 "J?" H 14280 3392 50  0000 L CNN
F 1 "Conn_01x20" H 14280 3301 50  0000 L CNN
F 2 "" H 14200 3400 50  0001 C CNN
F 3 "~" H 14200 3400 50  0001 C CNN
	1    14200 3400
	1    0    0    -1  
$EndComp
$Comp
L Connector:Micro_SD_Card J?
U 1 1 5F863101
P 10400 1800
F 0 "J?" H 10350 2517 50  0000 C CNN
F 1 "Micro_SD_Card" H 10350 2426 50  0000 C CNN
F 2 "" H 11550 2100 50  0001 C CNN
F 3 "http://katalog.we-online.de/em/datasheet/693072010801.pdf" H 10400 1800 50  0001 C CNN
	1    10400 1800
	1    0    0    -1  
$EndComp
$Comp
L Memory_Flash:W25Q32JVSS U?
U 1 1 5F865B35
P 8500 5100
F 0 "U?" H 8150 5450 50  0000 C CNN
F 1 "W25Q32JVSS" H 8800 4750 50  0000 C CNN
F 2 "Package_SO:SOIC-8_5.23x5.23mm_P1.27mm" H 8500 5100 50  0001 C CNN
F 3 "http://www.winbond.com/resource-files/w25q32jv%20revg%2003272018%20plus.pdf" H 8500 5100 50  0001 C CNN
	1    8500 5100
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_OTG J?
U 1 1 5F8667BF
P 9700 3800
F 0 "J?" H 9757 4267 50  0000 C CNN
F 1 "USB_OTG" H 9757 4176 50  0000 C CNN
F 2 "" H 9850 3750 50  0001 C CNN
F 3 " ~" H 9850 3750 50  0001 C CNN
	1    9700 3800
	1    0    0    -1  
$EndComp
Text Label 13500 2500 0    50   ~ 0
LCD-5V
Text Label 13500 2600 0    50   ~ 0
LCD-5V
Text Label 13500 2700 0    50   ~ 0
LCD-5V
Text Label 13500 2800 0    50   ~ 0
LCD-D0
Text Label 13500 2900 0    50   ~ 0
LCD-D1
Text Label 13500 3000 0    50   ~ 0
LCD-D2
Text Label 13500 3100 0    50   ~ 0
LCD-D3
Text Label 13500 3200 0    50   ~ 0
LCD-D4
Text Label 13500 3300 0    50   ~ 0
LCD-D5
Text Label 13500 3400 0    50   ~ 0
LCD-D6
Text Label 13500 3500 0    50   ~ 0
LCD-D7
Text Label 13300 3600 0    50   ~ 0
LCD-CON-PWM
Text Label 13300 3700 0    50   ~ 0
SOUND-LATCH
Text Label 13500 3800 0    50   ~ 0
LCD-RS
Text Label 13500 3900 0    50   ~ 0
LCD-E
Text Label 13500 4000 0    50   ~ 0
LCD-CS
Text Label 13500 4100 0    50   ~ 0
LCD-R\~w
Text Label 13500 4300 0    50   ~ 0
GND
Text Label 13500 4400 0    50   ~ 0
GND
Text Label 13500 4200 0    50   ~ 0
GND
Wire Wire Line
	13500 2500 14000 2500
Wire Wire Line
	13500 2600 14000 2600
Wire Wire Line
	13500 2700 14000 2700
Wire Wire Line
	13500 2800 14000 2800
Wire Wire Line
	14000 2900 13500 2900
Wire Wire Line
	14000 3000 13500 3000
Wire Wire Line
	14000 3100 13500 3100
Wire Wire Line
	14000 3200 13500 3200
Wire Wire Line
	14000 3300 13500 3300
Wire Wire Line
	14000 3400 13500 3400
Wire Wire Line
	14000 3500 13500 3500
Wire Wire Line
	14000 3600 13300 3600
Wire Wire Line
	14000 3700 13300 3700
Wire Wire Line
	14000 3800 13500 3800
Wire Wire Line
	14000 3900 13500 3900
Wire Wire Line
	14000 4000 13500 4000
Wire Wire Line
	14000 4100 13500 4100
Wire Wire Line
	14000 4200 13500 4200
Wire Wire Line
	14000 4300 13500 4300
Wire Wire Line
	14000 4400 13500 4400
Text Label 13500 5000 0    50   ~ 0
KEY-R1
Text Label 13500 5100 0    50   ~ 0
KEY-R2
Text Label 13500 5200 0    50   ~ 0
KEY-R3
Text Label 13500 5300 0    50   ~ 0
KEY-R4
Text Label 13500 5400 0    50   ~ 0
KEY-R5
Text Label 13500 5500 0    50   ~ 0
KEY-R6
Text Label 13500 5600 0    50   ~ 0
KEY-R7
Text Label 13500 5700 0    50   ~ 0
KEY-R8
Text Label 13500 5800 0    50   ~ 0
KEY-C1
Text Label 13500 5900 0    50   ~ 0
KEY-C2
Text Label 13500 6000 0    50   ~ 0
KEY-C3
Text Label 13500 6100 0    50   ~ 0
KEY-C4
Text Label 13500 6200 0    50   ~ 0
KEY-C5
Text Label 13500 6300 0    50   ~ 0
KEY-C6
Text Label 13500 6400 0    50   ~ 0
KEY-C7
Text Label 13500 6500 0    50   ~ 0
KEY-C8
Wire Wire Line
	14000 5000 13500 5000
Wire Wire Line
	14000 5100 13500 5100
Wire Wire Line
	14000 5200 13500 5200
Wire Wire Line
	14000 5300 13500 5300
Wire Wire Line
	14000 5400 13500 5400
Wire Wire Line
	14000 5500 13500 5500
Wire Wire Line
	14000 5600 13500 5600
Wire Wire Line
	14000 5700 13500 5700
Wire Wire Line
	14000 5800 13500 5800
Wire Wire Line
	14000 5900 13500 5900
Wire Wire Line
	14000 6000 13500 6000
Wire Wire Line
	14000 6100 13500 6100
Wire Wire Line
	14000 6200 13500 6200
Wire Wire Line
	14000 6300 13500 6300
Wire Wire Line
	14000 6400 13500 6400
Wire Wire Line
	14000 6500 13500 6500
Text Notes 13550 2250 0    50   ~ 0
Hitachi HD61830\nLCD Controller
Text Notes 13600 4800 0    50   ~ 0
8 x 8 Key Matrix
Text Label 3500 4000 0    50   ~ 0
LCD-D0
Text Label 3500 4100 0    50   ~ 0
LCD-D1
Text Label 3500 4200 0    50   ~ 0
LCD-D2
Text Label 3500 4300 0    50   ~ 0
LCD-D3
Text Label 3500 4400 0    50   ~ 0
LCD-D4
Text Label 3500 4500 0    50   ~ 0
LCD-D5
Text Label 3500 4600 0    50   ~ 0
LCD-D6
Text Label 3500 4700 0    50   ~ 0
LCD-D7
Text Label 6500 3200 2    50   ~ 0
LCD-CON-PWM
Text Label 3400 5900 0    50   ~ 0
SOUND-LATCH
Text Label 3500 3900 0    50   ~ 0
LCD-RS
Text Label 6300 3300 2    50   ~ 0
LCD-E
Text Label 3400 5800 0    50   ~ 0
LCD-CS
Text Label 3400 5700 0    50   ~ 0
LCD-R|~w
Text Label 6300 3400 2    50   ~ 0
QSPI-CLK
Text Label 7500 5200 0    50   ~ 0
QSPI-CLK
Wire Wire Line
	8000 5200 7500 5200
Wire Wire Line
	8000 5000 7500 5000
Wire Wire Line
	5800 3400 6300 3400
Wire Wire Line
	4000 3900 3500 3900
Wire Wire Line
	4000 4000 3500 4000
Wire Wire Line
	4000 4100 3500 4100
Wire Wire Line
	4000 4200 3500 4200
Wire Wire Line
	4000 4300 3500 4300
Wire Wire Line
	4000 4400 3500 4400
Wire Wire Line
	4000 4500 3500 4500
Wire Wire Line
	4000 4600 3500 4600
Wire Wire Line
	4000 4700 3500 4700
Wire Wire Line
	5800 3300 6300 3300
Wire Wire Line
	5800 3200 6500 3200
Text Label 7500 5000 0    50   ~ 0
QSPI-CS
Text Label 6300 4200 2    50   ~ 0
QSPI-CS
Wire Wire Line
	6300 4200 5800 4200
Wire Wire Line
	4000 5800 3400 5800
Wire Wire Line
	5100 6700 5000 6700
Connection ~ 4700 6700
Wire Wire Line
	4700 6700 4600 6700
Connection ~ 4800 6700
Wire Wire Line
	4800 6700 4700 6700
Connection ~ 4900 6700
Wire Wire Line
	4900 6700 4800 6700
Connection ~ 5000 6700
Wire Wire Line
	5000 6700 4900 6700
$Comp
L power:GND #PWR?
U 1 1 5F88FEB6
P 4600 6800
F 0 "#PWR?" H 4600 6550 50  0001 C CNN
F 1 "GND" H 4605 6627 50  0000 C CNN
F 2 "" H 4600 6800 50  0001 C CNN
F 3 "" H 4600 6800 50  0001 C CNN
	1    4600 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 6800 4600 6700
Connection ~ 4600 6700
Wire Wire Line
	4000 5700 3400 5700
$Comp
L power:GND #PWR?
U 1 1 5F89799B
P 8500 5500
F 0 "#PWR?" H 8500 5250 50  0001 C CNN
F 1 "GND" H 8505 5327 50  0000 C CNN
F 2 "" H 8500 5500 50  0001 C CNN
F 3 "" H 8500 5500 50  0001 C CNN
	1    8500 5500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5F898153
P 8500 4700
F 0 "#PWR?" H 8500 4550 50  0001 C CNN
F 1 "+3V3" H 8650 4750 50  0000 C CNN
F 2 "" H 8500 4700 50  0001 C CNN
F 3 "" H 8500 4700 50  0001 C CNN
	1    8500 4700
	1    0    0    -1  
$EndComp
Text Label 3500 6000 0    50   ~ 0
QSPI-IO0
Wire Wire Line
	4000 6000 3500 6000
Text Label 3500 6100 0    50   ~ 0
QSPI-IO1
Wire Wire Line
	3500 6100 4000 6100
Wire Wire Line
	4000 6200 3500 6200
Text Label 3500 6200 0    50   ~ 0
QSPI-IO3
Text Label 3500 3400 0    50   ~ 0
QSPI-IO2
Wire Wire Line
	3500 3400 4000 3400
Text Label 9500 5200 2    50   ~ 0
QSPI-IO2
Text Label 9500 4900 2    50   ~ 0
QSPI-IO0
Text Label 9500 5000 2    50   ~ 0
QSPI-IO1
Text Label 9500 5300 2    50   ~ 0
QSPI-IO3
Wire Wire Line
	9500 5300 9000 5300
Wire Wire Line
	9500 5200 9000 5200
Wire Wire Line
	9500 5000 9000 5000
Wire Wire Line
	9500 4900 9000 4900
Text Label 6200 2600 2    50   ~ 0
USB-DM
Text Label 10500 3900 2    50   ~ 0
USB-DM
Text Label 6200 2700 2    50   ~ 0
USB-DP
Wire Wire Line
	6200 2600 5800 2600
Wire Wire Line
	6200 2700 5800 2700
Text Label 10500 3800 2    50   ~ 0
USB-DP
Wire Wire Line
	10500 3800 10000 3800
Wire Wire Line
	10500 3900 10000 3900
Wire Wire Line
	9500 1500 8900 1500
Text Label 8900 1500 0    50   ~ 0
SDCARD-D2
Text Label 8900 1600 0    50   ~ 0
SDCARD-D3
Text Label 8900 1700 0    50   ~ 0
SDCARD-CMD
Text Label 8900 2100 0    50   ~ 0
SDCARD-D0
Text Label 8900 2200 0    50   ~ 0
SDCARD-D1
Text Label 8900 1900 0    50   ~ 0
SDCARD-CK
Wire Wire Line
	9500 2200 8900 2200
Wire Wire Line
	9500 2100 8900 2100
Wire Wire Line
	9500 1900 8900 1900
Wire Wire Line
	9500 1700 8900 1700
Wire Wire Line
	9500 1600 8900 1600
Text Notes 3500 3000 0    50   ~ 0
EXTOSC_HS
Text Notes 5850 6400 0    50   ~ 0
EXTOSC_32k
Text Label 6200 2800 2    50   ~ 0
SWDIO
Text Label 6200 2900 2    50   ~ 0
SWDC
Wire Wire Line
	5800 2800 6200 2800
Wire Wire Line
	5800 2900 6200 2900
Text Label 6200 2400 2    50   ~ 0
TRACE-TX
Text Label 6200 2500 2    50   ~ 0
TRACE-RX
Wire Wire Line
	6200 2400 5800 2400
Wire Wire Line
	5800 2500 6200 2500
Text Label 6350 5800 2    50   ~ 0
SDCARD-D1
Text Label 6350 5700 2    50   ~ 0
SDCARD-D0
Wire Wire Line
	6350 5800 5800 5800
Wire Wire Line
	5800 5700 6350 5700
Text Label 6350 5900 2    50   ~ 0
SDCARD-D2
Text Label 6350 6000 2    50   ~ 0
SDCARD-D3
Text Label 6350 6100 2    50   ~ 0
SDCARD-CK
Text Label 3400 5100 0    50   ~ 0
SDCARD-CMD
Wire Wire Line
	3400 5100 4000 5100
Wire Wire Line
	6350 5900 5800 5900
Wire Wire Line
	5800 6000 6350 6000
Wire Wire Line
	5800 6100 6350 6100
Wire Wire Line
	4000 1700 3500 1700
Wire Wire Line
	4000 1500 3500 1500
Text Label 3500 1500 0    50   ~ 0
RESET
Text Label 3500 1700 0    50   ~ 0
BOOT0
Text Label 6300 3000 2    50   ~ 0
KEY-R1
Text Label 3500 5200 0    50   ~ 0
KEY-R2
Text Label 3500 5300 0    50   ~ 0
KEY-R3
Text Label 3500 5400 0    50   ~ 0
KEY-R4
Text Label 3500 5500 0    50   ~ 0
KEY-R5
Text Label 3500 5600 0    50   ~ 0
KEY-R6
Text Label 6300 3500 2    50   ~ 0
KEY-R7
Text Label 6300 3600 2    50   ~ 0
KEY-R8
Text Label 6300 4000 2    50   ~ 0
KEY-C1
Text Label 6300 4100 2    50   ~ 0
KEY-C2
Text Label 3500 3200 0    50   ~ 0
KEY-C3
Text Label 3500 3300 0    50   ~ 0
KEY-C4
Text Label 3500 3500 0    50   ~ 0
KEY-C5
Text Label 3500 3600 0    50   ~ 0
KEY-C6
Text Label 3500 3700 0    50   ~ 0
KEY-C7
Text Label 3500 3800 0    50   ~ 0
KEY-C8
Wire Wire Line
	4000 5900 3400 5900
Wire Wire Line
	3500 5200 4000 5200
Wire Wire Line
	3500 5300 4000 5300
Wire Wire Line
	4000 5400 3500 5400
Wire Wire Line
	4000 5500 3500 5500
Wire Wire Line
	6300 4100 5800 4100
Wire Wire Line
	3500 3200 4000 3200
Wire Wire Line
	4000 3300 3500 3300
Wire Wire Line
	4000 3500 3500 3500
Wire Wire Line
	3500 3600 4000 3600
Wire Wire Line
	4000 3700 3500 3700
Wire Wire Line
	3500 3800 4000 3800
$Comp
L Connector_Generic:Conn_01x08 J?
U 1 1 5F9E857F
P 7700 1300
F 0 "J?" H 7780 1292 50  0000 L CNN
F 1 "Conn_01x08" H 7780 1201 50  0000 L CNN
F 2 "" H 7700 1300 50  0001 C CNN
F 3 "~" H 7700 1300 50  0001 C CNN
	1    7700 1300
	1    0    0    -1  
$EndComp
Text Label 7000 1000 0    50   ~ 0
GND
Text Label 7000 1100 0    50   ~ 0
RESET
Text Label 7000 1200 0    50   ~ 0
3V3
Text Label 7000 1300 0    50   ~ 0
BOOT0
Text Label 7000 1400 0    50   ~ 0
SWDC
Text Label 7000 1500 0    50   ~ 0
SWDIO
Text Label 7000 1700 0    50   ~ 0
TRACE-TX
Text Label 7000 1600 0    50   ~ 0
TRACE-RX
Wire Wire Line
	7500 1000 7000 1000
Wire Wire Line
	7000 1100 7500 1100
Wire Wire Line
	7000 1200 7500 1200
Wire Wire Line
	7000 1300 7500 1300
Wire Wire Line
	7000 1400 7500 1400
Wire Wire Line
	7000 1500 7500 1500
Wire Wire Line
	7000 1600 7500 1600
Wire Wire Line
	7000 1700 7500 1700
Text Label 6350 6200 2    50   ~ 0
WAKEUP
Wire Wire Line
	6350 6200 5800 6200
Text Label 6750 6200 2    50   ~ 0
WAKEUP
Text Label 3150 1500 0    50   ~ 0
RESET
Text Label 3150 1700 0    50   ~ 0
BOOT0
Text Label 6200 5200 2    50   ~ 0
ADC_VBAT
Wire Wire Line
	6200 5200 5800 5200
Text Label 6600 5200 2    50   ~ 0
ADC_VBAT
$Comp
L Connector_Generic:Conn_02x16_Odd_Even J?
U 1 1 5F8D19FF
P 10700 8200
F 0 "J?" H 10750 9117 50  0000 C CNN
F 1 "Conn_02x16_Odd_Even" H 10750 9026 50  0000 C CNN
F 2 "" H 10700 8200 50  0001 C CNN
F 3 "~" H 10700 8200 50  0001 C CNN
	1    10700 8200
	1    0    0    -1  
$EndComp
Text Label 11500 8600 2    50   ~ 0
PC1
Text Label 10000 7900 0    50   ~ 0
PA1
Text Label 10000 8000 0    50   ~ 0
PA2
Text Label 10000 8200 0    50   ~ 0
PA4
Text Label 10000 8300 0    50   ~ 0
PA5
Text Label 10000 8500 0    50   ~ 0
PA7
Text Label 11500 8700 2    50   ~ 0
PC4
Text Label 11500 8800 2    50   ~ 0
PC5
Text Label 11500 8100 2    50   ~ 0
PB11
Text Label 11500 8200 2    50   ~ 0
PB12
Text Label 11500 8300 2    50   ~ 0
PB13
Text Label 11500 8400 2    50   ~ 0
PB14
Text Label 11500 8500 2    50   ~ 0
PB15
Text Label 10000 8600 0    50   ~ 0
PA8
Text Label 10000 8700 0    50   ~ 0
PD0
Text Label 10000 8800 0    50   ~ 0
PD1
Text Label 11500 7800 2    50   ~ 0
PB5
Text Label 11500 7900 2    50   ~ 0
PB6
Text Label 11500 8000 2    50   ~ 0
PB7
Text Label 10000 9000 0    50   ~ 0
GND
Text Label 10000 7700 0    50   ~ 0
3V3
Text Label 10000 7500 0    50   ~ 0
5VINT
Text Label 11500 9000 2    50   ~ 0
GND
Text Label 11500 7500 2    50   ~ 0
5VINT
Text Label 10000 7600 0    50   ~ 0
GND
Text Label 11500 7600 2    50   ~ 0
GND
Text Label 11500 7700 2    50   ~ 0
3V3
Text Label 10000 8900 0    50   ~ 0
3V3
Text Label 11500 8900 2    50   ~ 0
3V3
Text Label 10000 8400 0    50   ~ 0
PA6
Text Label 10000 7800 0    50   ~ 0
PA0
Text Label 10000 8100 0    50   ~ 0
PA3
Wire Wire Line
	10000 7500 10500 7500
Wire Wire Line
	11000 7500 11500 7500
Wire Wire Line
	10000 7600 10500 7600
Wire Wire Line
	11000 7600 11500 7600
Wire Wire Line
	10000 7700 10500 7700
Wire Wire Line
	10000 7800 10500 7800
Wire Wire Line
	10000 7900 10500 7900
Wire Wire Line
	10000 8000 10500 8000
Wire Wire Line
	10000 8100 10500 8100
Wire Wire Line
	10000 8200 10500 8200
Wire Wire Line
	10000 8300 10500 8300
Wire Wire Line
	10000 8400 10500 8400
Wire Wire Line
	10000 8500 10500 8500
Wire Wire Line
	10000 8600 10500 8600
Wire Wire Line
	10000 8700 10500 8700
Wire Wire Line
	10000 8800 10500 8800
Wire Wire Line
	10000 8900 10500 8900
Wire Wire Line
	10000 9000 10500 9000
Wire Wire Line
	11000 7900 11500 7900
Wire Wire Line
	11000 8000 11500 8000
Wire Wire Line
	11000 8100 11500 8100
Wire Wire Line
	11000 8200 11500 8200
Wire Wire Line
	11000 8300 11500 8300
Wire Wire Line
	11000 8400 11500 8400
Wire Wire Line
	11000 8500 11500 8500
Wire Wire Line
	11000 8600 11500 8600
Wire Wire Line
	11000 8700 11500 8700
Wire Wire Line
	11000 8800 11500 8800
Wire Wire Line
	11000 8900 11500 8900
Wire Wire Line
	11000 9000 11500 9000
Wire Wire Line
	11000 7700 11500 7700
Wire Wire Line
	11500 7800 11000 7800
Text Label 6100 1600 2    50   ~ 0
PA1
Text Label 6100 1700 2    50   ~ 0
PA2
Text Label 6100 1900 2    50   ~ 0
PA4
Text Label 6100 2000 2    50   ~ 0
PA5
Text Label 6100 2200 2    50   ~ 0
PA7
Text Label 6100 2300 2    50   ~ 0
PA8
Text Label 3700 4900 0    50   ~ 0
PD0
Text Label 3700 5000 0    50   ~ 0
PD1
Text Label 6100 2100 2    50   ~ 0
PA6
Text Label 6100 1500 2    50   ~ 0
PA0
Text Label 6100 1800 2    50   ~ 0
PA3
Text Label 6100 5000 2    50   ~ 0
PC1
Text Label 6100 5300 2    50   ~ 0
PC4
Text Label 6100 5400 2    50   ~ 0
PC5
Text Label 6100 4300 2    50   ~ 0
PB11
Text Label 6100 4400 2    50   ~ 0
PB12
Text Label 6100 4500 2    50   ~ 0
PB13
Text Label 6100 4600 2    50   ~ 0
PB14
Text Label 6100 4700 2    50   ~ 0
PB15
Text Label 6100 3700 2    50   ~ 0
PB5
Text Label 6100 3800 2    50   ~ 0
PB6
Text Label 6100 3900 2    50   ~ 0
PB7
NoConn ~ 5800 5600
NoConn ~ 5800 5500
NoConn ~ 4000 6300
NoConn ~ 4000 6400
NoConn ~ 5800 4900
NoConn ~ 5800 5100
Wire Wire Line
	6100 5400 5800 5400
Wire Wire Line
	6100 5300 5800 5300
Wire Wire Line
	6100 5000 5800 5000
Wire Wire Line
	6100 4700 5800 4700
Wire Wire Line
	6100 4600 5800 4600
Wire Wire Line
	6100 4500 5800 4500
Wire Wire Line
	6100 4400 5800 4400
Wire Wire Line
	6100 4300 5800 4300
Wire Wire Line
	6100 3900 5800 3900
Wire Wire Line
	6100 3800 5800 3800
Wire Wire Line
	6100 3700 5800 3700
Wire Wire Line
	6100 1500 5800 1500
Wire Wire Line
	5800 1600 6100 1600
Wire Wire Line
	5800 1700 6100 1700
Wire Wire Line
	5800 1800 6100 1800
Wire Wire Line
	5800 1900 6100 1900
Wire Wire Line
	5800 2000 6100 2000
Wire Wire Line
	5800 2100 6100 2100
Wire Wire Line
	5800 2200 6100 2200
Wire Wire Line
	5800 2300 6100 2300
Wire Wire Line
	3700 4900 4000 4900
Wire Wire Line
	3700 5000 4000 5000
Wire Wire Line
	6300 4000 5800 4000
Wire Wire Line
	6300 3600 5800 3600
Wire Wire Line
	6300 3500 5800 3500
Wire Wire Line
	4000 5600 3500 5600
Wire Wire Line
	6300 3000 5800 3000
$EndSCHEMATC
