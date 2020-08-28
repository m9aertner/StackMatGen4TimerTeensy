EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "StackMatGen4TimerTeensy LED"
Date "2020-08-29"
Rev "1.0"
Comp "Matthias Gärtner"
Comment1 "MIT License"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:AudioJack3 J1
U 1 1 5F4A974C
P 1150 4200
F 0 "J1" H 1132 4525 50  0000 C CNN
F 1 "AudioJack3" H 1132 4434 50  0000 C CNN
F 2 "" H 1150 4200 50  0001 C CNN
F 3 "~" H 1150 4200 50  0001 C CNN
	1    1150 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 4300 2100 4300
Wire Wire Line
	2100 3300 1750 3300
$Comp
L teensy:Teensy-LC U2
U 1 1 5F49851B
P 3250 4250
F 0 "U2" H 3275 5487 60  0001 C CNN
F 1 "Teensy-LC" H 3275 5381 60  0000 C CNN
F 2 "" H 3250 3700 60  0000 C CNN
F 3 "" H 3250 3700 60  0000 C CNN
	1    3250 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 4900 1750 4900
Connection ~ 1750 4900
Wire Wire Line
	1750 4900 1750 5400
Text Notes 700  4750 0    50   ~ 0
TRS tip input goes to\nTeensy pin 9 (PCB label)\non physical pin 11.
Wire Wire Line
	1750 3300 1750 4100
Wire Wire Line
	1350 4100 1750 4100
Connection ~ 1750 4100
Wire Wire Line
	1750 4100 1750 4900
NoConn ~ 1350 4200
$Comp
L power:+5V #PWR?
U 1 1 5F4BF3C9
P 5700 2400
F 0 "#PWR?" H 5700 2250 50  0001 C CNN
F 1 "+5V" H 5715 2573 50  0000 C CNN
F 2 "" H 5700 2400 50  0001 C CNN
F 3 "" H 5700 2400 50  0001 C CNN
	1    5700 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 2400 5700 2400
Wire Wire Line
	4450 5200 5000 5200
Wire Wire Line
	4450 5100 4900 5100
Wire Wire Line
	4450 5000 4800 5000
Text Notes 4500 5100 0    50   ~ 0
CS (LOAD)
Text Notes 4650 5000 0    50   ~ 0
CLK
Text Notes 4850 5200 0    50   ~ 0
DIN
Wire Wire Line
	1750 5400 4600 5400
Wire Wire Line
	4450 4100 4600 4100
Wire Wire Line
	4450 4000 4600 4000
Wire Wire Line
	5000 4000 5000 2400
$Comp
L Connector:USB_OTG J2
U 1 1 5F4D0C79
P 4550 2250
F 0 "J2" V 4561 2580 50  0001 L CNN
F 1 "Teensy USB" V 4300 2000 50  0000 L CNN
F 2 "" H 4700 2200 50  0001 C CNN
F 3 " ~" H 4700 2200 50  0001 C CNN
	1    4550 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	1750 3300 1750 2250
Wire Wire Line
	1750 2250 4150 2250
Connection ~ 1750 3300
Wire Wire Line
	4750 2550 4750 4000
Wire Wire Line
	4600 3900 4450 3900
Wire Wire Line
	4600 3900 4600 4000
Connection ~ 4600 4000
Wire Wire Line
	4550 2550 4550 3400
Wire Wire Line
	4550 3400 4450 3400
Connection ~ 4750 4000
Wire Wire Line
	4600 4000 4750 4000
Wire Wire Line
	4750 4000 5000 4000
Wire Wire Line
	4450 2550 4450 3300
Connection ~ 4600 5400
Connection ~ 5700 2400
Wire Wire Line
	5000 5200 5000 4800
Wire Wire Line
	4900 5100 4900 4700
Wire Wire Line
	4800 5000 4800 4600
NoConn ~ 6200 4500
NoConn ~ 6200 4400
NoConn ~ 6200 4300
NoConn ~ 6200 4200
NoConn ~ 6200 4100
Connection ~ 5200 4500
Wire Wire Line
	5200 4400 5200 4500
Connection ~ 5200 4400
Wire Wire Line
	5200 4300 5200 4400
Connection ~ 5200 4300
Wire Wire Line
	5200 4200 5200 4300
Connection ~ 5200 4200
Connection ~ 5200 5000
Wire Wire Line
	5200 4100 5200 4200
Wire Wire Line
	4600 5400 5200 5400
Connection ~ 5200 5400
Wire Wire Line
	4800 4600 5200 4600
Wire Wire Line
	4900 4700 5200 4700
Wire Wire Line
	5000 4800 5200 4800
Connection ~ 5700 5400
Wire Wire Line
	5700 5400 5700 5550
$Comp
L power:GND #PWR?
U 1 1 5F4B92B6
P 5700 5550
F 0 "#PWR?" H 5700 5300 50  0001 C CNN
F 1 "GND" H 5705 5377 50  0000 C CNN
F 2 "" H 5700 5550 50  0001 C CNN
F 3 "" H 5700 5550 50  0001 C CNN
	1    5700 5550
	1    0    0    -1  
$EndComp
Connection ~ 5200 5100
Wire Wire Line
	5200 5000 5200 5100
Wire Wire Line
	5200 5400 5700 5400
Wire Wire Line
	5200 5100 5200 5400
$Comp
L 74xx:74HCT541 U1
U 1 1 5F49749E
P 5700 4600
F 0 "U1" H 5400 5350 50  0001 C CNN
F 1 "74HCT541" H 5400 5250 50  0000 C CNN
F 2 "" H 5700 4600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HCT541" H 5700 4600 50  0001 C CNN
	1    5700 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 5400 7000 5400
Wire Wire Line
	5700 2400 7000 2400
Wire Wire Line
	5700 2400 5700 3800
$Comp
L Driver_LED:MAX7219 U?
U 1 1 5F514077
P 8150 3600
F 0 "U?" H 8150 4781 50  0001 C CNN
F 1 "MAX7219" H 7900 4550 50  0000 C CNN
F 2 "" H 8100 3650 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf" H 8200 3450 50  0001 C CNN
	1    8150 3600
	1    0    0    -1  
$EndComp
$Comp
L Driver_LED:MAX7219 U?
U 1 1 5F518809
P 9300 3600
F 0 "U?" H 9300 4781 50  0001 C CNN
F 1 "MAX7219" H 9050 4550 50  0000 C CNN
F 2 "" H 9250 3650 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf" H 9350 3450 50  0001 C CNN
	1    9300 3600
	1    0    0    -1  
$EndComp
$Comp
L Driver_LED:MAX7219 U?
U 1 1 5F51AF1A
P 10400 3600
F 0 "U?" H 10400 4781 50  0001 C CNN
F 1 "MAX7219" H 10150 4550 50  0000 C CNN
F 2 "" H 10350 3650 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf" H 10450 3450 50  0001 C CNN
	1    10400 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 5400 7000 4600
Wire Wire Line
	7000 2400 7000 2600
Wire Wire Line
	6600 4800 6600 4400
$Comp
L Driver_LED:MAX7219 U3
U 1 1 5F4E30C4
P 7000 3600
F 0 "U3" H 7000 4781 50  0001 C CNN
F 1 "MAX7219" H 6750 4550 50  0000 C CNN
F 2 "" H 6950 3650 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf" H 7050 3450 50  0001 C CNN
	1    7000 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 4800 6600 4800
Wire Wire Line
	6200 4700 6500 4700
Wire Wire Line
	6600 4300 6550 4300
Wire Wire Line
	6550 4300 6550 4600
Connection ~ 6550 4600
Wire Wire Line
	6200 4600 6550 4600
Wire Wire Line
	6600 4200 6500 4200
Wire Wire Line
	6500 4200 6500 4700
Connection ~ 6500 4700
Wire Wire Line
	6500 4700 7650 4700
Wire Wire Line
	9950 4600 9950 4300
Wire Wire Line
	9950 4300 10000 4300
Wire Wire Line
	9900 4700 9900 4200
Wire Wire Line
	9900 4200 10000 4200
Wire Wire Line
	8900 4400 8550 4400
Wire Wire Line
	8900 4300 8850 4300
Wire Wire Line
	8850 4300 8850 4600
Connection ~ 8850 4600
Wire Wire Line
	8900 4200 8800 4200
Wire Wire Line
	8800 4200 8800 4700
Connection ~ 8800 4700
Wire Wire Line
	8800 4700 9900 4700
Wire Wire Line
	7750 4400 7400 4400
Wire Wire Line
	7750 4300 7700 4300
Wire Wire Line
	7700 4300 7700 4600
Connection ~ 7700 4600
Wire Wire Line
	7750 4200 7650 4200
Wire Wire Line
	7650 4200 7650 4700
Connection ~ 7650 4700
Wire Wire Line
	7650 4700 8800 4700
Wire Wire Line
	8150 4600 8150 5400
Wire Wire Line
	8150 5400 7000 5400
Connection ~ 7000 5400
Wire Wire Line
	9300 4600 9300 5400
Wire Wire Line
	9300 5400 8150 5400
Connection ~ 8150 5400
Wire Wire Line
	10400 4600 10400 5400
Wire Wire Line
	10400 5400 9300 5400
Connection ~ 9300 5400
Wire Wire Line
	7000 2400 8150 2400
Wire Wire Line
	10400 2400 10400 2600
Connection ~ 7000 2400
Wire Wire Line
	9300 2600 9300 2400
Connection ~ 9300 2400
Wire Wire Line
	9300 2400 10400 2400
Wire Wire Line
	8150 2600 8150 2400
Connection ~ 8150 2400
Wire Wire Line
	8150 2400 9300 2400
Wire Wire Line
	10000 4400 9700 4400
Wire Notes Line
	6400 1950 6400 5650
Wire Notes Line
	6400 5650 11000 5650
Wire Notes Line
	11000 5650 11000 1950
Wire Notes Line style dash_dot
	11000 1950 6400 1950
Text Notes 10000 5600 0    50   ~ 0
LED Matrix Module 4x8x8
Wire Wire Line
	5200 4500 5200 5000
$Comp
L Device:C C?
U 1 1 5F59E222
P 5000 4150
F 0 "C?" H 5115 4196 50  0000 L CNN
F 1 "C" H 5115 4105 50  0000 L CNN
F 2 "" H 5038 4000 50  0001 C CNN
F 3 "~" H 5000 4150 50  0001 C CNN
	1    5000 4150
	1    0    0    -1  
$EndComp
Connection ~ 5000 4000
Wire Wire Line
	4600 4100 4600 5400
Wire Wire Line
	5000 4300 5200 4300
Wire Wire Line
	6550 4600 7700 4600
Wire Wire Line
	7700 4600 8850 4600
Wire Wire Line
	8850 4600 9950 4600
$EndSCHEMATC
