#include "BatteryPage.h"
#include <Arduino.h>

// ===== CONFIG =====
#define BATTERY_PIN 34     // ADC pin
#define ADC_MAX 4095.0
#define REF_VOLTAGE 3.3

// Voltage divider (IMPORTANT!)
#define VOLTAGE_DIVIDER 2.0   // change based on your resistor values

// Battery range
#define MAX_VOLTAGE 4.2
#define MIN_VOLTAGE 3.3

float batteryVoltage = 0.0;
int batteryPercent = 0;

// ================= INIT =================
void initBatteryPage() {
  analogReadResolution(12);
  #define BATTERY_PIN 34
}

// ================= UPDATE =================
void updateBatteryPage() {

int adc = 0;
  for (int i = 0; i < 10; i++) {
    adc += analogRead(BATTERY_PIN);
  }
  adc /= 10;

  batteryVoltage = (adc / ADC_MAX) * REF_VOLTAGE * VOLTAGE_DIVIDER;

  // Convert to %
  batteryPercent = (batteryVoltage - MIN_VOLTAGE) * 100 /
                   (MAX_VOLTAGE - MIN_VOLTAGE);

  if (batteryPercent > 100) batteryPercent = 100;
  if (batteryPercent < 0) batteryPercent = 0;
}

// ================= DRAW =================
void drawBatteryPage(Adafruit_SSD1306 &display) {

  display.setTextSize(1);
  display.setCursor(5, 5);
  display.print("Battery");
  display.drawLine(0, 15, 120, 15, WHITE);

  // Battery outline
  int x = 20;
  int y = 25;
  int w = 80;
  int h = 20;

  display.drawRect(x, y, w, h, WHITE);
  display.fillRect(x + w, y + 5, 3, 10, WHITE); // tip

  // Fill level
  int fillWidth = (w - 4) * batteryPercent / 100;
  display.fillRect(x + 2, y + 2, fillWidth, h - 4, WHITE);

  // Percentage text
  display.setTextSize(1);
  display.setCursor(40, 50);
  display.print(batteryPercent);
  display.print("%");

  // Voltage
  display.setCursor(35, 58);
  display.print(batteryVoltage, 2);
  display.print("V");
}

// ================= BUTTON =================
void handleBatteryPageButton(int button) {
  // no buttons needed
}