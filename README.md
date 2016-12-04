# Flow Meter on ArduLogRTC

Program to measure a tipping bucket flow meter (reed switch contact) with Arduino Uno (ATMega328) ArduLogRTC shield from hobbyelectronics.co.uk.

Author: Jan Knappe
Contact: jan.knappe@tcd.ie

## Initializing Real Time Clock (RTC)
To initialize the RTC with current computer time, connect ArduLog Shield via FTDI breakout to the computer and load "ds1307" sketch from the RTClib library. Check correct RTC setting in serial monitor at baud rate 57600. The inbuilt coin cell battery will retain the time stamp setting until reprogrammed.
