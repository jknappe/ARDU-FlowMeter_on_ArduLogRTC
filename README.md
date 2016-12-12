# Flow Meter on ArduLogRTC

Program to measure a tipping bucket flow meter (reed switch contact) with Arduino Uno (ATMega328) ArduLogRTC shield from hobbyelectronics.co.uk.

Author: Jan Knappe
Contact: jan.knappe@tcd.ie

## Parts

 * ArduLog Shield (including ATMega328 microcontroller, micro-SD card slot, RTC)
 * MikroE Volt charger board for battery and external power supply
 * FTDI breakput board to USB micro-B
 * panel mount push button with LED (as reset button + status LED)
 * 2-pin panel mount connector (male pins)
 * 2-pin cable connector (female pins)
 * magnetic reed switch with cable
 * neodymium switch magnet
 * 45Ah 3.7V Li-Po battery
 * 2 polarized 6-pin connectors male 
 * 3 polarized 6-pin connectors female
 * 2 polarized 2-pin connectors male
 * 2 polarized 2-pin connectors female
 * micro-SD card
 * CR1225 button cell
 * 230x150x85 mm waterproof enclosure
 * USB cable USB-A to USB micro-B
 * several jumper cables and female/male connectors 
 
## Wiring diagram

## Communication with logger

To communicate with the ArduLog

## Initializing Real Time Clock (RTC)
To initialize the RTC with current computer time, connect ArduLog Shield via FTDI breakout to the computer and load "ds1307" sketch from the RTClib library. Check correct RTC setting in serial monitor at baud rate 57600. The inbuilt coin cell battery will retain the time stamp setting until reprogrammed.
