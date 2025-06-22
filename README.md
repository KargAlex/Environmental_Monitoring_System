# Autonomous Environmental Monitoring System – STM32F401RE

## 📌 Overview

This project implements an autonomous environmental monitoring system using the ARM Cortex-M4-based STM32F401RE microcontroller. It combines temperature-humidity sensing, user interaction through touch input and UART, and output signaling via an onboard LED.

Designed for the *"Microprocessors and Peripherals"* course (8th semester, ECE AUTH, Spring 2025), the project demonstrates the integration and management of multiple peripherals using C, hardware timers, and interrupts.

---

## 🧰 Hardware & Tools

- **Microcontroller**: Nucleo STM32F401RE (ARM Cortex-M4)
- **Sensors & Peripherals**:
  - DHT11 – Temperature and Humidity Sensor
  - Capacitive Touch Sensor
  - Onboard LED (PA5)
- **Tools**:
  - Keil uVision5 (IDE)
  - PuTTY / Tera Term (for UART monitoring)

---

## ⚙️ System Functionality

### 1. 📡 Sensor Reading (DHT11)
- The DHT11 is interfaced using GPIO-based bit-banging (custom protocol implementation).
- Data is sampled every **2 seconds** using **TIM2**.
- Sensor data includes:
  - Temperature in °C
  - Relative humidity in %
- Readings are printed over UART in a human-readable format.

### 2. ✋ Touch Sensor Interaction
- A capacitive touch sensor triggers **EXTI** (external interrupt).
- On each touch:
  - The current temperature and humidity reading is immediately shown over UART.
  - A touch counter is incremented and logged.
  - The LED toggles state as visual feedback.

### 3. 💡 LED Control
- **Onboard LED (PA5)** responds to:
  - Touch inputs (toggles on each press)
  - Sensor reading events (blinks briefly on each read)
- LED actions are handled in the main loop or timer ISR.

### 4. 🖥️ UART Communication
- UART is used for:
  - Sending sensor readings
  - Logging touch events
  - Debugging information
- Configured at **115200 baud**, 8N1.

---

## 🔧 Implementation Details

- **Language**: C
- **Timers**:
  - TIM2: triggers DHT11 read every 2 seconds (interrupt-based)
- **Interrupts**:
  - EXTI: handles touch sensor input
  - Timer interrupt: handles periodic DHT11 sampling

---

## 🗂 File Structure
/Project_Root/
│
├── Core/
│ ├── Src/
│ │ ├── main.c # Main program logic
│ │ ├── dht11.c # DHT11 read protocol
│ │ └── touch.c # Touch interrupt logic
│ └── Inc/
│ ├── dht11.h
│ └── touch.h
├── Drivers/ CMSIS and ARM University drivers
├── README.md # This file
└── ...

