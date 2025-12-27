# OBD-II Telemetry Reader

An OBD-II telemetry reader built with an ESP32 and designed in KiCAD.

## Overview

This project reads vehicle diagnostic data from the OBD-II port and displays it either on an LCD screen or streams it wirelessly via Bluetooth.

## Hardware

- **Microcontroller**: ESP32-D0WD-V3 Rev 301 (Switching to S3 for internal USB support)
- **Design Software**: KiCAD 9
- **Input**: OBD-II connector (standard 16-pin)
- **Output Options**:
  - LCD display (I2C interface)
  - Bluetooth serial communication

## Features

- Real-time vehicle data monitoring
- Multiple data parameter support (RPM, speed, temperature, etc.)
- Wireless data transmission via Bluetooth
- Compact PCB design for automotive environment

## Future Enhancements

- WiFi data logging
- Mobile app integration
- Additional sensor support
- Data visualization dashboard


---


Built as a personal project.
