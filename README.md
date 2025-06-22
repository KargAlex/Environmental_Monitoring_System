ARM Assembly & C Project - Autonomous Environmental Monitoring System
Overview

This embedded systems project implements an autonomous environmental monitoring system using the STM32F401RE microcontroller. It is developed in C using ARM CMSIS libraries and integrates real-time sensor readings, interrupt-driven control, and peripheral interaction.

The system continuously monitors temperature and humidity using the DHT11 sensor and provides visual feedback through an onboard LED. A capacitive touch sensor allows the user to dynamically interact with the system, enabling or disabling output display in real-time.
Requirements

    Microcontroller: Nucleo STM32F401RE

    Peripherals: DHT11 sensor (temperature & humidity), capacitive touch sensor, onboard LED

    Tools: Keil uVision, PuTTY/Tera Term (for UART display)

    Languages: C (ARM CMSIS-based)

Functionality
1. DHT11 Sensor Reading

    Periodically triggered (every few seconds) using a hardware timer.

    Reads temperature and humidity values via GPIO-based communication protocol.

    Timing-critical implementation (similar to Adafruit’s bit parsing logic).

2. UART Output

    Transmits formatted sensor data to a terminal (e.g., Tera Term) via UART.

    Example output:

    Temperature: 25°C  
    Humidity: 60%  
    --------------------

3. LED Control Logic

    Based on sensor data:

        If temperature > 30°C → LED blinks fast.

        If humidity > 70% → LED blinks slowly.

        Otherwise → LED remains OFF.

4. Touch Sensor Interrupt

    Connected to an external interrupt line (EXTI).

    On touch:

        Toggles output display mode:

            Enabled → UART shows live sensor readings.

            Disabled → UART silenced (data collected silently).

        Status change and touch count printed via UART.

5. System Behavior Summary

    All components work autonomously after startup.

    Sensor reads and outputs are timer-based.

    Touch sensor gives user control over system verbosity.

    LED offers immediate physical feedback based on environmental conditions.

Files

    main.c – Core program: peripheral initialization, DHT11 decoding, UART communication, timer configuration, interrupt logic.

    Drivers/ – Low-level code and custom drivers for DHT11 and touch sensor handling.

📌 Developed for the "Microprocessors & Peripherals" course, AUTh – Spring 2025.
