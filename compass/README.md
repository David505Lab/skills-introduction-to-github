# ESP32-S3 Digital Compass with QMC5883L & SSD1306

A feature-rich digital compass project for the ESP32-S3 microcontroller featuring dual display modes (digital and graphical) on a 64x32 OLED screen.

## Features

✨ **Dual Display Modes**
- **Digital Mode**: Large heading display (0-360°) with 8-point compass rose (N, NE, E, SE, S, SW, W, NW) + raw magnetic field values
- **Graphical Mode**: Visual compass needle with cardinal directions on a rotating compass rose

⚙️ **Magnetometer (QMC5883L)**
- High sensitivity magnetic field detection
- 200Hz sampling rate for smooth, responsive readings
- 8G measurement range
- Automatic calibration routine

📊 **Display**
- 64x32 pixel SSD1306 OLED
- I2C interface
- Clear, readable output optimized for small screen

🔧 **Easy Calibration**
- Built-in calibration routine via serial command
- Automatic offset and scale correction
- Magnetic declination adjustment for location accuracy

## Hardware Requirements

| Component | Model | Notes |
|-----------|-------|-------|
| Microcontroller | ESP32-S3 Min | 240MHz, dual-core |
| Magnetometer | QMC5883L (GY-271) | I2C interface |
| Display | SSD1306 | 64x32 pixels, I2C |

## Pin Connections

```
ESP32-S3 Min Board
├─ GPIO 8  (SDA) ──→ SSD1306 SDA + QMC5883L SDA
├─ GPIO 9  (SCL) ──→ SSD1306 SCL + QMC5883L SCL
├─ GND     ────────→ SSD1306 GND + QMC5883L GND
└─ 3.3V    ────────→ SSD1306 VCC + QMC5883L VCC

I2C Addresses:
├─ SSD1306: 0x3C
└─ QMC5883L: 0x0D
```

## Quick Start

### 1. Arduino IDE Setup
1. Install ESP32 board support via Board Manager
2. Install `Adafruit SSD1306` and `Adafruit GFX Library`

### 2. Upload
1. Select board: ESP32-S3
2. Upload: `esp32_compass.ino`
3. Serial Monitor: 115200 baud

### 3. Calibrate
Send `c` via serial monitor, then rotate device for 30 seconds

## Serial Commands

| Command | Action |
|---------|--------|
| `c` | Start calibration routine |
| `d` | Digital display mode |
| `g` | Graphical display mode |

## Files Included

- `esp32_compass.ino` - Main compass application
- `I2C_SCANNER.ino` - Device detection utility
- `HARDWARE_SETUP.md` - Detailed wiring guide
- `SCHEMATIC.txt` - Circuit diagrams
- `library_requirements.txt` - Library installation

For detailed setup, see HARDWARE_SETUP.md

**Last Updated**: July 2026
