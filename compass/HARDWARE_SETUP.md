# Hardware Setup Guide

## Components Checklist

- [ ] ESP32-S3 Min board
- [ ] QMC5883L magnetometer module (GY-271)
- [ ] SSD1306 OLED display (64x32)
- [ ] USB-C cable (for programming)
- [ ] 4x 4.7kΩ resistors (I2C pull-ups, optional if built-in)
- [ ] Breadboard or PCB
- [ ] Jumper wires

## Wiring Schematic

```
ESP32-S3 Min              QMC5883L (GY-271)        SSD1306 OLED
┌─────────────┐          ┌──────────────┐         ┌──────────────┐
│             │          │              │         │              │
│ 3V3 ────────┼──────────┼──VCC          │        │ VCC           │
│ GPIO 8 ─────┼──────────┼──SDA ─────────┼────────┼── SDA         │
│ GPIO 9 ─────┼──────────┼──SCL ─────────┼────────┼── SCL         │
│ GND ────────┼──────────┼──GND ─────────┼────────┼── GND         │
│             │          │              │         │              │
└─────────────┘          └──────────────┘         └──────────────┘
```

## Step-by-Step Assembly

### Step 1: Prepare Breadboard
1. Place ESP32-S3 Min board on breadboard
2. Leave space on both sides for sensor modules

### Step 2: Connect Power Rails
1. Connect ESP32-S3 **3V3** pin to **positive (red)** breadboard rail
2. Connect ESP32-S3 **GND** pin to **negative (black)** breadboard rail

### Step 3: Mount QMC5883L Module
1. Insert QMC5883L module onto breadboard:
   - **VCC** → positive rail (3.3V)
   - **GND** → negative rail (Ground)
   - **SDA** → separate row for I2C bus
   - **SCL** → separate row for I2C bus

### Step 4: Mount SSD1306 Display
1. Insert SSD1306 module onto breadboard:
   - **VCC** → positive rail (3.3V)
   - **GND** → negative rail (Ground)
   - **SDA** → same row as QMC5883L SDA
   - **SCL** → same row as QMC5883L SCL

### Step 5: Connect to ESP32-S3
1. Connect I2C bus to ESP32-S3:
   - SDA wire → **GPIO 8** on ESP32-S3
   - SCL wire → **GPIO 9** on ESP32-S3

### Step 6: Power On and Test
1. Connect ESP32-S3 to computer via USB-C cable
2. Open Serial Monitor at 115200 baud
3. Should see initialization message

## Pin Mapping

| ESP32-S3 | Function | Device |
|----------|----------|--------|
| 3V3 | Power (3.3V) | Both modules |
| GND | Ground | Both modules |
| GPIO 8 | I2C SDA | QMC5883L + SSD1306 |
| GPIO 9 | I2C SCL | QMC5883L + SSD1306 |

## I2C Addresses

| Device | Address | Verify |
|--------|---------|--------|
| QMC5883L | 0x0D | Run I2C_SCANNER.ino |
| SSD1306 | 0x3C | Run I2C_SCANNER.ino |

## Troubleshooting

### No devices found on I2C bus
1. Check all connections are secure
2. Verify power supply (measure 3.3V)
3. Check GND connections
4. Try without pull-up resistors first
5. Run I2C_SCANNER.ino to diagnose

### Display not showing
1. Verify I2C address is 0x3C
2. Check SDA/SCL connections to GPIO 8 and 9
3. Confirm 3.3V power on display module

### Compass readings erratic
1. Verify GND connection
2. Move away from magnetic interference
3. Run calibration routine

## Power Consumption

| Component | Current | Notes |
|-----------|---------|-------|
| ESP32-S3 (idle) | 10-20mA | CPU at low frequency |
| ESP32-S3 (active) | 80-160mA | Both cores active |
| QMC5883L | 1-2mA | 200Hz sampling |
| SSD1306 | 5-15mA | Display on |
| **Total** | **~100-190mA** | Typical operation |

USB 2.0 (500mA) is sufficient for all components.
