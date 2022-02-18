EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
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
L RF_Module:ESP32-WROOM-32 U1
U 1 1 62072A49
P 2600 3700
F 0 "U1" H 2600 5281 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 2600 5190 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 2600 2200 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 2300 3750 50  0001 C CNN
	1    2600 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 6208BBF7
P 2600 5150
F 0 "#PWR0101" H 2600 4900 50  0001 C CNN
F 1 "GND" H 2605 4977 50  0000 C CNN
F 2 "" H 2600 5150 50  0001 C CNN
F 3 "" H 2600 5150 50  0001 C CNN
	1    2600 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1500 2600 1700
$Comp
L Device:C C3
U 1 1 6208DCCB
P 1800 1850
F 0 "C3" H 1915 1896 50  0000 L CNN
F 1 "0.1uF" H 1915 1805 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D4.0mm_P2.00mm" H 1838 1700 50  0001 C CNN
F 3 "~" H 1800 1850 50  0001 C CNN
	1    1800 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 6208DCD1
P 1300 1850
F 0 "C2" H 1415 1896 50  0000 L CNN
F 1 "10uF" H 1415 1805 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D4.0mm_P2.00mm" H 1338 1700 50  0001 C CNN
F 3 "~" H 1300 1850 50  0001 C CNN
	1    1300 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 2000 1800 2000
$Comp
L power:GND #PWR0102
U 1 1 6208DCDB
P 1800 2000
F 0 "#PWR0102" H 1800 1750 50  0001 C CNN
F 1 "GND" H 1805 1827 50  0000 C CNN
F 2 "" H 1800 2000 50  0001 C CNN
F 3 "" H 1800 2000 50  0001 C CNN
	1    1800 2000
	1    0    0    -1  
$EndComp
Connection ~ 2600 1700
Wire Wire Line
	2600 1700 2600 2300
Wire Wire Line
	2000 2500 1100 2500
Wire Wire Line
	1100 2500 1100 2400
$Comp
L Device:R R1
U 1 1 6208E6BA
P 1100 2250
F 0 "R1" H 1170 2296 50  0000 L CNN
F 1 "10K" H 1170 2205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 1030 2250 50  0001 C CNN
F 3 "~" H 1100 2250 50  0001 C CNN
	1    1100 2250
	1    0    0    -1  
$EndComp
Text Label 1100 2000 2    50   ~ 0
VDD33
Wire Wire Line
	1100 2000 1100 2100
$Comp
L Device:C C1
U 1 1 6208FDDA
P 650 2650
F 0 "C1" H 765 2696 50  0000 L CNN
F 1 "0.1uF" H 765 2605 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D4.0mm_P2.00mm" H 688 2500 50  0001 C CNN
F 3 "~" H 650 2650 50  0001 C CNN
	1    650  2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	650  2500 1100 2500
Connection ~ 1100 2500
$Comp
L power:GND #PWR0103
U 1 1 620907CB
P 650 2800
F 0 "#PWR0103" H 650 2550 50  0001 C CNN
F 1 "GND" H 655 2627 50  0000 C CNN
F 2 "" H 650 2800 50  0001 C CNN
F 3 "" H 650 2800 50  0001 C CNN
	1    650  2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 2500 3950 2500
Text Label 3500 2500 0    50   ~ 0
Boot_Option
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 62092936
P 6700 2900
F 0 "J1" H 6780 2892 50  0000 L CNN
F 1 "Conn_01x02" H 6780 2801 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6700 2900 50  0001 C CNN
F 3 "~" H 6700 2900 50  0001 C CNN
	1    6700 2900
	1    0    0    -1  
$EndComp
Text Notes 6650 2850 0    50   ~ 0
Boot Option\n
Wire Wire Line
	6000 2900 6500 2900
$Comp
L power:GND #PWR0104
U 1 1 62094E3B
P 6000 3100
F 0 "#PWR0104" H 6000 2850 50  0001 C CNN
F 1 "GND" H 6005 2927 50  0000 C CNN
F 2 "" H 6000 3100 50  0001 C CNN
F 3 "" H 6000 3100 50  0001 C CNN
	1    6000 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3000 6000 3100
Wire Wire Line
	6000 3000 6500 3000
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 62097780
P 4400 2350
F 0 "J3" V 4364 2162 50  0000 R CNN
F 1 "Conn_01x03" V 4273 2162 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4400 2350 50  0001 C CNN
F 3 "~" H 4400 2350 50  0001 C CNN
	1    4400 2350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3200 2600 4300 2600
Wire Wire Line
	4300 2600 4300 2550
Wire Wire Line
	4400 2550 4400 2800
Wire Wire Line
	4400 2800 3200 2800
Wire Wire Line
	4500 2550 4600 2550
Wire Wire Line
	4600 2550 4600 2600
$Comp
L power:GND #PWR0105
U 1 1 6209A315
P 4600 2600
F 0 "#PWR0105" H 4600 2350 50  0001 C CNN
F 1 "GND" H 4605 2427 50  0000 C CNN
F 2 "" H 4600 2600 50  0001 C CNN
F 3 "" H 4600 2600 50  0001 C CNN
	1    4600 2600
	1    0    0    -1  
$EndComp
Text Notes 4250 2300 0    50   ~ 0
UART Prog
Text Label 3250 2600 0    50   ~ 0
TXD
Text Label 3250 2800 0    50   ~ 0
RXD
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 6209B1FA
P 6600 3950
F 0 "J2" H 6680 3942 50  0000 L CNN
F 1 "Conn_01x04" H 6680 3851 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 6600 3950 50  0001 C CNN
F 3 "~" H 6600 3950 50  0001 C CNN
	1    6600 3950
	1    0    0    -1  
$EndComp
Text Notes 6550 3800 0    50   ~ 0
JTAG
Wire Wire Line
	6400 3850 6150 3850
Wire Wire Line
	6400 3950 6150 3950
Wire Wire Line
	6400 4150 6150 4150
Text Label 5650 4050 0    50   ~ 0
MTCK
Text Label 5650 4150 0    50   ~ 0
MTDO
Text Label 6000 2900 0    50   ~ 0
Boot_Option
Wire Wire Line
	6400 4050 6150 4050
$Comp
L Device:R R2
U 1 1 6209F2AC
P 6000 3850
F 0 "R2" V 5793 3850 50  0000 C CNN
F 1 "100" V 5884 3850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 5930 3850 50  0001 C CNN
F 3 "~" H 6000 3850 50  0001 C CNN
	1    6000 3850
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 6209FB1B
P 6000 3950
F 0 "R3" V 5793 3950 50  0000 C CNN
F 1 "100" V 5884 3950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 5930 3950 50  0001 C CNN
F 3 "~" H 6000 3950 50  0001 C CNN
	1    6000 3950
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 620A09BA
P 6000 4050
F 0 "R4" V 5793 4050 50  0000 C CNN
F 1 "100" V 5884 4050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 5930 4050 50  0001 C CNN
F 3 "~" H 6000 4050 50  0001 C CNN
	1    6000 4050
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 620A0BC0
P 6000 4150
F 0 "R5" V 5793 4150 50  0000 C CNN
F 1 "100" V 5884 4150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 5930 4150 50  0001 C CNN
F 3 "~" H 6000 4150 50  0001 C CNN
	1    6000 4150
	0    1    1    0   
$EndComp
Wire Wire Line
	5850 3850 5650 3850
Wire Wire Line
	5850 3950 5650 3950
Wire Wire Line
	5850 4050 5650 4050
Wire Wire Line
	5850 4150 5650 4150
Text Label 5650 3850 0    50   ~ 0
MTMS
Text Label 5650 3950 0    50   ~ 0
MTDI
Text Label 3250 3300 0    50   ~ 0
MTMS
Wire Wire Line
	3200 3300 3450 3300
Text Label 3250 3100 0    50   ~ 0
MTDI
Text Label 3250 3200 0    50   ~ 0
MTCK
Text Label 3250 3400 0    50   ~ 0
MTDO
Wire Wire Line
	3200 3100 3450 3100
Wire Wire Line
	3200 3200 3450 3200
Wire Wire Line
	3200 3400 3450 3400
Wire Wire Line
	5950 4850 6100 4850
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 620B0A16
P 6300 4850
F 0 "J4" H 6380 4842 50  0000 L CNN
F 1 "Conn_01x02" H 6380 4751 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6300 4850 50  0001 C CNN
F 3 "~" H 6300 4850 50  0001 C CNN
	1    6300 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 620B0F7E
P 5950 5050
F 0 "#PWR0106" H 5950 4800 50  0001 C CNN
F 1 "GND" H 5955 4877 50  0000 C CNN
F 2 "" H 5950 5050 50  0001 C CNN
F 3 "" H 5950 5050 50  0001 C CNN
	1    5950 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 4950 5950 4950
Wire Wire Line
	5950 4950 5950 5050
Wire Wire Line
	5950 4750 5950 4850
Text Label 5950 4750 2    50   ~ 0
VDD33
Text Notes 6250 4800 0    50   ~ 0
Battery Input
$Comp
L Connector_Generic:Conn_01x08 J6
U 1 1 620BF760
P 3950 4400
F 0 "J6" H 4030 4392 50  0000 L CNN
F 1 "Conn_01x08" H 4030 4301 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 3950 4400 50  0001 C CNN
F 3 "~" H 3950 4400 50  0001 C CNN
	1    3950 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4800 3750 4800
Wire Wire Line
	3750 4700 3200 4700
Wire Wire Line
	3200 4600 3750 4600
Wire Wire Line
	3200 4500 3750 4500
Wire Wire Line
	3200 4400 3750 4400
Wire Wire Line
	3750 4300 3200 4300
Wire Wire Line
	3200 4200 3750 4200
Wire Wire Line
	3750 4100 3200 4100
Text Notes 3900 4050 0    50   ~ 0
IO Port
Text Label 3200 4100 0    50   ~ 0
IO23
Text Label 3200 4200 0    50   ~ 0
IO25
Text Label 3200 4300 0    50   ~ 0
IO26
Text Label 3200 4400 0    50   ~ 0
IO27
Text Label 3200 4500 0    50   ~ 0
IO32
Text Label 3200 4600 0    50   ~ 0
IO33
Text Label 3200 4700 0    50   ~ 0
IO34
Text Label 3200 4800 0    50   ~ 0
IO35
$Comp
L Connector_Generic:Conn_01x06 J5
U 1 1 620D7FA6
P 1350 4000
F 0 "J5" H 1268 3475 50  0000 C CNN
F 1 "Conn_01x06" H 1268 3566 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 1350 4000 50  0001 C CNN
F 3 "~" H 1350 4000 50  0001 C CNN
	1    1350 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	2000 3800 1550 3800
Wire Wire Line
	1550 3900 2000 3900
Wire Wire Line
	1550 4000 2000 4000
Wire Wire Line
	1550 4100 2000 4100
Wire Wire Line
	2000 4200 1550 4200
Wire Wire Line
	1550 3700 2000 3700
Text Label 1550 3700 0    50   ~ 0
SDO
Text Label 1550 3800 0    50   ~ 0
SDI
Text Label 1550 3900 0    50   ~ 0
SHD
Text Label 1550 4000 0    50   ~ 0
SWP
Text Label 1550 4100 0    50   ~ 0
SCK
Text Label 1550 4200 0    50   ~ 0
SCS
Text Notes 1300 4250 1    50   ~ 0
Serial Data Port
NoConn ~ 3200 2700
NoConn ~ 3200 2900
NoConn ~ 3200 3000
NoConn ~ 3200 3500
NoConn ~ 3200 3600
NoConn ~ 3200 3700
NoConn ~ 3200 3800
NoConn ~ 3200 3900
NoConn ~ 3200 4000
NoConn ~ 2000 2700
NoConn ~ 2000 2800
Text Label 2600 1500 0    50   ~ 0
VDD33
$Comp
L power:GNDPWR #PWR0107
U 1 1 620F8175
P 8450 5100
F 0 "#PWR0107" H 8450 4900 50  0001 C CNN
F 1 "GNDPWR" H 8454 4946 50  0000 C CNN
F 2 "" H 8450 5050 50  0001 C CNN
F 3 "" H 8450 5050 50  0001 C CNN
	1    8450 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 5100 8450 4950
Wire Wire Line
	8450 4950 8850 4950
Wire Wire Line
	8850 4950 8850 5150
$Comp
L power:GND #PWR0108
U 1 1 620FAE3A
P 8850 5150
F 0 "#PWR0108" H 8850 4900 50  0001 C CNN
F 1 "GND" H 8855 4977 50  0000 C CNN
F 2 "" H 8850 5150 50  0001 C CNN
F 3 "" H 8850 5150 50  0001 C CNN
	1    8850 5150
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 620F76DC
P 8450 4600
F 0 "#FLG0101" H 8450 4675 50  0001 C CNN
F 1 "PWR_FLAG" H 8450 4773 50  0000 C CNN
F 2 "" H 8450 4600 50  0001 C CNN
F 3 "~" H 8450 4600 50  0001 C CNN
	1    8450 4600
	1    0    0    -1  
$EndComp
Text Label 8850 4600 2    50   ~ 0
VDD33
Wire Wire Line
	8450 4600 8450 4750
Wire Wire Line
	8450 4750 8850 4750
Wire Wire Line
	8850 4750 8850 4600
Wire Wire Line
	2600 5100 2600 5150
Wire Wire Line
	1300 1700 1800 1700
Connection ~ 1800 1700
Wire Wire Line
	1800 1700 2600 1700
Connection ~ 1800 2000
$EndSCHEMATC