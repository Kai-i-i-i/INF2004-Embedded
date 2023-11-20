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

## Work Package:
|Header1|Header2|Header3|
| --- | --- | --- |
| This | is a | table |
| This | is 2nd | row |
| This | is 3rd | row |

## Block and Flow Diagrams

![Block Diagram](https://user-images.githubusercontent.com/81953271/124010886-b571ca80-d9df-11eb-86ac-b358c48ac6aa.png "Block_Diagram")

![Flow Diagram](https://user-images.githubusercontent.com/81953271/124010886-b571ca80-d9df-11eb-86ac-b358c48ac6aa.png "Flow_Diagram")

