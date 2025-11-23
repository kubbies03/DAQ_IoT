# **Embedded Systems Labs – Summary**

## **Lab 1 – UART Communication**


**Objective:** Implement UART communication between the EFR32xG21 board and a PC.
**Key Topics:**

* USART module overview and UART frame format
* Baud rate generation and oversampling
* TX/RX pin mapping using VCOM on the Wireless Starter Kit
* Adding USART drivers using Simplicity Studio Project Configurator

**Implementation:**

* Configure GPIO for PA5 (TX) and PA6 (RX)
* Initialize USART0 (115200-8-N-1)
* Send/receive data between MCU and PC terminal (e.g., MobaXterm)
* Demonstrate bidirectional serial communication using C code on both ends

---

## **Lab 2 – I²C Communication with Si7021 Sensor**


**Objective:** Interface the EFR32BG13 board with the Si7021 temperature & humidity sensor over I²C.
**Key Topics:**

* I²C bus fundamentals: SDA, SCL, addressing, ACK/NACK
* Si7021 command set and transfer sequences
* Reading humidity/temperature in Hold/No-Hold master modes
* CRC checking and formula for converting raw codes to real values

**Implementation:**

* Use I2CSPM driver and Sleep Timer for delay
* Send command 0xE5 (Measure RH – Hold Mode)
* Read 16-bit measurement and compute %RH using datasheet formula
* Output results through UART logging

---

## **Lab 3 – SPI Communication with Memory LCD-TFT LS013B7DH03**


**Objective:** Use SPI to control a 128×128 SHARP Memory LCD.
**Key Topics:**

* SPI modes (clock polarity & phase), SCLK up to 1.1 MHz
* Memory LCD interface: line-based update, command format (M0, M1, M2, AG0–AG7, D0–D127)
* GLIB graphics library & DMD driver architecture

**Implementation:**

* Build LCD driver using synchronous USART SPI mode
* Implement functions to draw text/graphics into a framebuffer
* Implement `DMD_updateDisplay()` to send line update frames
* Display custom strings such as “LAB 3 SPI” on the LCD

---

## **Lab 4 – DMA (LDMA) for UART–Memory Transfers**


**Objective:** Use LDMA to transfer data between UART buffers and memory without CPU load.
**Key Topics:**

* LDMA architecture: channels, descriptors, transfer size, REQ signals
* Memory-to-peripheral and peripheral-to-memory transfer flows
* FIFO buffer handling

**Implementation:**

* Channel 0: USART RX → RAM (inbuf)
* Channel 1: RAM (outbuf) → USART TX
* CPU processes received data and triggers transmit DMA
* Example app: Echo data back to PC using DMA on both RX and TX

**Assignments:**

* Count the most frequent character in a user-provided string
* Measure DMA latency with different buffer sizes (10/100/1000 bytes)

---

## **Lab 5 – BLE Advertisement**


**Objective:** Build a custom BLE advertising packet on the EFR32xG21.
**Key Topics:**

* Advertisement channels, intervals, discoverability modes
* PDU format and AD structures (Length–Type–Value)
* Common AD types: Flags (0x01), Complete Local Name (0x09), Manufacturer Data (0xFF)

**Implementation:**

* Create advertising set using Silicon Labs Bluetooth API
* Configure interval, channel map, and payload
* Construct custom AdvData including:

  * Flags
  * Manufacturer data (Student ID – 4 bytes)
  * Device name
* Scan using Simplicity Connect mobile app

---

## **Lab 6 – BLE Mesh (Provisioner, Client, Server)**


**Objective:** Build a BLE Mesh system using three roles: Provisioner, Client, Server.
**Key Topics:**

* Mesh concepts: provisioning, unicast addresses, network/app keys
* Node types: Relay, Proxy, Friend, LPN
* Vendor Model configuration and message opcodes

**Implementation:**

* **Provisioner:**

  * Factory reset
  * Scan unprovisioned devices
  * Assign unicast addresses and bind AppKey/NetKey
* **Client:**

  * Read sensor data (humidity/temperature) or generate mock values
  * Publish Vendor Model messages (opcode 0x01)
  * Periodic vs. manual sending using Button 0/1
* **Server:**

  * Receive sensor_status messages
  * Parse payload into temperature/humidity
  * Display values via UART logs

If you want, I can generate a **full polished GitHub README** in Markdown with formatting, images, and a table of contents.
