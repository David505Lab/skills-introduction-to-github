#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Math.h>

// Display setup: 64x32 SSD1306
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// QMC5883L Magnetometer
#define QMC5883L_ADDR 0x0D

// QMC5883L Registers
#define QMC5883L_REG_XOUT_LSB 0x00
#define QMC5883L_REG_CONF1 0x09
#define QMC5883L_REG_CONF2 0x0A
#define QMC5883L_REG_RESET 0x0B
#define QMC5883L_REG_STATUS 0x06

// Calibration values
float magOffsetX = 0;
float magOffsetY = 0;
float magScaleX = 1.0;
float magScaleY = 1.0;

// Variables
float heading = 0;
int displayMode = 0;
unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\nESP32-S3 Compass with QMC5883L");
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();
  
  if (!initQMC5883L()) {
    Serial.println("QMC5883L initialization failed!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("QMC5883L Failed");
    display.display();
    while (1);
  }
  
  Serial.println("QMC5883L initialized successfully");
  delay(500);
}

void loop() {
  unsigned long now = millis();
  
  if (now - lastUpdate >= 50) {
    lastUpdate = now;
    
    float mx, my, mz;
    readQMC5883L(mx, my, mz);
    heading = calculateHeading(mx, my);
    
    if (now % 10000 < 5000) {
      displayMode = 0;
    } else {
      displayMode = 1;
    }
    
    if (displayMode == 0) {
      drawDigitalDisplay(heading, mx, my, mz);
    } else {
      drawGraphicalDisplay(heading);
    }
  }
  
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'c') {
      calibrate();
    } else if (cmd == 'd') {
      displayMode = 0;
    } else if (cmd == 'g') {
      displayMode = 1;
    }
  }
}

bool initQMC5883L() {
  Wire.begin();
  delay(10);
  
  Wire.beginTransmission(QMC5883L_ADDR);
  Wire.write(QMC5883L_REG_RESET);
  Wire.write(0x80);
  if (Wire.endTransmission() != 0) return false;
  delay(50);
  
  Wire.beginTransmission(QMC5883L_ADDR);
  Wire.write(QMC5883L_REG_CONF1);
  Wire.write(0x0D);
  if (Wire.endTransmission() != 0) return false;
  delay(10);
  
  Wire.beginTransmission(QMC5883L_ADDR);
  Wire.write(QMC5883L_REG_CONF2);
  Wire.write(0x00);
  if (Wire.endTransmission() != 0) return false;
  
  delay(100);
  return true;
}

void readQMC5883L(float &mx, float &my, float &mz) {
  Wire.beginTransmission(QMC5883L_ADDR);
  Wire.write(QMC5883L_REG_XOUT_LSB);
  Wire.endTransmission();
  
  Wire.requestFrom(QMC5883L_ADDR, 6);
  
  if (Wire.available() >= 6) {
    int16_t x = Wire.read() | (Wire.read() << 8);
    int16_t y = Wire.read() | (Wire.read() << 8);
    int16_t z = Wire.read() | (Wire.read() << 8);
    
    mx = (x - magOffsetX) * magScaleX;
    my = (y - magOffsetY) * magScaleY;
    mz = z;
  }
}

float calculateHeading(float mx, float my) {
  float heading = atan2(my, mx) * 180 / PI;
  float magneticDeclination = 0;
  heading += magneticDeclination;
  
  if (heading < 0) heading += 360;
  if (heading >= 360) heading -= 360;
  
  return heading;
}

void drawDigitalDisplay(float heading, float mx, float my, float mz) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.println("COMPASS");
  
  display.setTextSize(2);
  display.setCursor(0, 10);
  if (heading < 100) display.print(" ");
  if (heading < 10) display.print(" ");
  display.print((int)heading);
  display.print("d");
  
  display.setTextSize(1);
  display.setCursor(42, 10);
  display.println(getDirectionLabel(heading));
  
  display.setTextSize(1);
  display.setCursor(0, 28);
  display.print("X:");
  display.print((int)mx);
  display.print(" Y:");
  display.print((int)my);
  
  display.display();
}

void drawGraphicalDisplay(float heading) {
  display.clearDisplay();
  
  int centerX = 32;
  int centerY = 16;
  int radius = 12;
  
  display.drawCircle(centerX, centerY, radius, SSD1306_WHITE);
  
  drawCompassPoint(centerX, centerY, radius, 0, 'N');
  drawCompassPoint(centerX, centerY, radius, 90, 'E');
  drawCompassPoint(centerX, centerY, radius, 180, 'S');
  drawCompassPoint(centerX, centerY, radius, 270, 'W');
  
  float rad = (heading - 90) * PI / 180;
  int needleX = centerX + (radius - 2) * cos(rad);
  int needleY = centerY + (radius - 2) * sin(rad);
  display.drawLine(centerX, centerY, needleX, needleY, SSD1306_WHITE);
  
  display.fillCircle(centerX, centerY, 2, SSD1306_WHITE);
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("H:");
  if (heading < 100) display.print(" ");
  display.print((int)heading);
  display.print("d");
  
  display.setCursor(42, 0);
  display.println(getDirectionLabel(heading));
  
  display.display();
}

void drawCompassPoint(int centerX, int centerY, int radius, int angle, char direction) {
  float rad = (angle - 90) * PI / 180;
  int x = centerX + (radius + 3) * cos(rad);
  int y = centerY + (radius + 3) * sin(rad);
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x - 2, y - 3);
  display.write(direction);
}

const char* getDirectionLabel(float heading) {
  if (heading >= 337.5 || heading < 22.5) return "N";
  if (heading >= 22.5 && heading < 67.5) return "NE";
  if (heading >= 67.5 && heading < 112.5) return "E";
  if (heading >= 112.5 && heading < 157.5) return "SE";
  if (heading >= 157.5 && heading < 202.5) return "S";
  if (heading >= 202.5 && heading < 247.5) return "SW";
  if (heading >= 247.5 && heading < 292.5) return "W";
  if (heading >= 292.5 && heading < 337.5) return "NW";
  return "?";
}

void calibrate() {
  Serial.println("\nStarting calibration...");
  Serial.println("Rotate the device in all directions for 30 seconds");
  
  float minX = 32767, maxX = -32768;
  float minY = 32767, maxY = -32768;
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Calibrating...");
  display.println("Rotate device");
  display.println("in all");
  display.println("directions...");
  display.display();
  
  unsigned long startTime = millis();
  while (millis() - startTime < 30000) {
    float mx, my, mz;
    readQMC5883L(mx, my, mz);
    
    if (mx < minX) minX = mx;
    if (mx > maxX) maxX = mx;
    if (my < minY) minY = my;
    if (my > maxY) maxY = my;
    
    delay(50);
  }
  
  magOffsetX = (maxX + minX) / 2.0;
  magOffsetY = (maxY + minY) / 2.0;
  
  float scaleX = (maxX - minX) / 2.0;
  float scaleY = (maxY - minY) / 2.0;
  
  float maxScale = max(scaleX, scaleY);
  magScaleX = maxScale / scaleX;
  magScaleY = maxScale / scaleY;
  
  Serial.print("Calibration complete:");
  Serial.print(" OffsetX="); Serial.print(magOffsetX);
  Serial.print(" OffsetY="); Serial.print(magOffsetY);
  Serial.print(" ScaleX="); Serial.print(magScaleX);
  Serial.print(" ScaleY="); Serial.println(magScaleY);
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Calibration");
  display.println("Complete!");
  display.display();
  delay(2000);
}
