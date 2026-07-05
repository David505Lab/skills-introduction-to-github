/*
  I2C Device Scanner
  Scans for I2C devices on GPIO 8 (SDA) and GPIO 9 (SCL)
  
  Expected output:
  Device found at address: 0x0D   (QMC5883L)
  Device found at address: 0x3C   (SSD1306)
*/

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\nI2C Scanner for ESP32-S3");
  Serial.println("Scanning I2C bus on GPIO 8 (SDA) and GPIO 9 (SCL)");
  Serial.println("==================================================");
}

void loop() {
  int deviceCount = 0;
  
  Serial.println("\nScanning...");
  
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("✓ Device found at address: 0x");
      if (i < 16) Serial.print("0");
      Serial.print(i, HEX);
      Serial.print(" (decimal: ");
      Serial.print(i);
      Serial.print(") - ");
      
      if (i == 0x0D) {
        Serial.println("QMC5883L Magnetometer");
      } else if (i == 0x3C) {
        Serial.println("SSD1306 OLED Display (64x32)");
      } else if (i == 0x3D) {
        Serial.println("SSD1306 OLED Display (alternate address)");
      } else {
        Serial.println("Unknown device");
      }
      deviceCount++;
    }
  }
  
  Serial.println("\n==================================================");
  Serial.print("Total devices found: ");
  Serial.println(deviceCount);
  
  if (deviceCount == 0) {
    Serial.println("\n⚠️  No devices found!");
  } else if (deviceCount >= 2) {
    Serial.println("\n✓ Success! Both devices detected!");
  }
  
  Serial.println("\nScanning again in 5 seconds...\n");
  delay(5000);
}
