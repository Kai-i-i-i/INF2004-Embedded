# INF2004-Embedded
This project aims to develop an integrated Embedded Forensic Tool utilizing the Raspberry Pi Pico W to allow investigators to easily analyse and monitor communication between embedded devices and sensors ~~, as well as quickly identifying wireless networks in the vicinity for monitoring.~~

## Key Features:
1. Write findings/logs to SD card for easy viewing, storing of data collected.
2. Output of traffic and logs through host’s machine serial console using UART protocol.
3. Ability to sniff UART traffic on target embedded device by acting as a man-in-the-middle.
   1. Connect GPIO UART pins to target device to be able to read traffic.
   2. Output sniffed traffic onto UART console.
4. Ability to inject data into device using UART communications.
   1. Connect GPIO UART pins to target device to be able to inject traffic.
5. Ability to sniff SPI traffic on target embedded device.
   1. Connect the required pins for SPI communication to target devices.
   2. Pico device will sit as a man-in-the-middle and forward traffic out to MOSI to forward it to the intended device.
   3. Output sniffed SPI traffic onto UART console.
6. Ability to inject data into device using SPI communications.
   1. Spoof as the slave device and send modified or controlled data to the target embedded device.
7. Ability to sniff I2C traffic on target embedded device.
   1. Connect required pins for I2C traffic to the target device and sniff communication.
   2. Output sniffed traffic onto UART console.
8. Ability to inject data into device using I2C traffic.
   1. Connect required pins between two devices to sit as a man-in-the-middle, injecting or modifying traffic.
9. Hardware interface to identify which communication protocol is being used by target device.
    1. Pseudo buttons to select protocol.
    2. Device able to detect protocol being used.

## Warning
⚠️Note: All SPI and UART Pin numbers are GPIO numbers, not the actual Pin number 

## SPI Connection:
![image](https://github.com/Kai-i-i-i/INF2004-Embedded/blob/main/diagrams/spi_sniff.png)
![image](https://github.com/Kai-i-i-i/INF2004-Embedded/blob/main/diagrams/spi_inject.png)

| Other Devices (Sender) | SPI 0 (Middle) | SPI 1 (Receiver)|
| --- | --- | --- |
| MISO | 19 | 11 |
| MOSI | 16 | 12 |
| SCK | 18 | 10 |
| CS | 17 | 13 |

Sniffing: Connect GPIO 2 to GND  (SPI 0)\
Injecting: Connect GPIO 2 to GND and GPIO 0 to opposite GND 

## UART Connection:
![image](https://github.com/Kai-i-i-i/INF2004-Embedded/blob/main/diagrams/uart_sniff.png)
![image](https://github.com/Kai-i-i-i/INF2004-Embedded/blob/main/diagrams/uart_inject.png)

| Other Devices (Sender) | UART 0 (Receiver) |
| --- | --- |
| TX | 8 | 
| RX | 9 | 

Sniffing: Connect GPIO 1 to GND\
Injecting: Connect GPIO 1 to GND and GPIO 0 to opposite GND 
## Block and Flow Diagrams

![Block Diagram](https://github.com/Kai-i-i-i/INF2004-Embedded/blob/main/diagrams/Block.png "Block_Diagram")

![Flow Diagram](https://github.com/Kai-i-i-i/INF2004-Embedded/blob/main/diagrams/Flow.png "Flow_Diagram")

