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
F 0 "U?" H 4900 1011 50  0000 C CNN
F 1 "STM32H750VBTx" H 4900 920 50  0000 C CNN
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
P 8400 1800
F 0 "J?" H 8350 2517 50  0000 C CNN
F 1 "Micro_SD_Card" H 8350 2426 50  0000 C CNN
F 2 "" H 9550 2100 50  0001 C CNN
F 3 "http://katalog.we-online.de/em/datasheet/693072010801.pdf" H 8400 1800 50  0001 C CNN
	1    8400 1800
	1    0    0    -1  
$EndComp
$Comp
L Memory_Flash:W25Q32JVSS U?
U 1 1 5F865B35
P 8500 5100
F 0 "U?" H 8500 5681 50  0000 C CNN
F 1 "W25Q32JVSS" H 8500 5590 50  0000 C CNN
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
$EndSCHEMATC
