# ESP32 BLE UART Demo

This project demonstrates how to use the ESP32’s **Bluetooth Low Energy (BLE)** capabilities to send and receive data from an iOS device (iPhone or iPad) using the **BLE UART** protocol.

Built for learning and experimentation — based on the ESP32 BLE UART example, with modifications to work with **Adafruit’s Bluefruit Connect** app.

---

## Features

- Connect ESP32 to iPhone/iPad over BLE
- Send and receive messages using **Bluefruit Connect**
- Modify and extend BLE services and characteristics
- Monitor serial output for debugging

---

## Getting Started

### 1. Hardware

- ESP32 Dev Board
- iPhone or iPad with BLE support
- USB cable for programming

### 2. Software

- Arduino IDE or PlatformIO
- ESP32 board package installed
- [Bluefruit Connect App](https://apps.apple.com/us/app/adafruit-bluefruit-connect/id830125974) (iOS or Android)

---

## Setup

1. Clone this repo  
   ```bash
   git clone https://github.com/SubuktageenKhan/esp-32-ble-uart-ios-demo.git
   
2. Open the .ino file in Arduino IDE

3. Select your ESP32 board and correct port under Tools

4. Upload the sketch

5. Open Bluefruit Connect on your phone, connect to the ESP32, and use the controlpad to send signals to your ESP32
