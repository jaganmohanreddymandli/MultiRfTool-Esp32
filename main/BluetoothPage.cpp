#include "BluetoothPage.h"
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define MAX_DEVICES 5

BLEScan* pBLEScan;

String deviceNames[MAX_DEVICES];
int deviceCount = 0;

unsigned long lastScanTime = 0;

// ================= INIT =================
void initBluetoothPage() {
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);
}

// ================= UPDATE =================
void updateBluetoothPage() {

  // scan every 5 seconds
  if (millis() - lastScanTime < 5000) return;

  lastScanTime = millis();
  deviceCount = 0;

  BLEScanResults results = pBLEScan->start(3, false);

  int count = results.getCount();

  for (int i = 0; i < count && i < MAX_DEVICES; i++) {

    BLEAdvertisedDevice device = results.getDevice(i);

    if (device.haveName()) {
      deviceNames[deviceCount++] = device.getName().c_str();
    } else {
      deviceNames[deviceCount++] = "Unknown";
    }
  }

  pBLEScan->clearResults();
}

// ================= DRAW =================
void drawBluetoothPage(Adafruit_SSD1306 &display) {

  display.setTextSize(1);
  display.setCursor(5, 5);
  display.print("Bluetooth");
  display.drawLine(0, 15, 120, 15, WHITE);

  if (deviceCount == 0) {
    display.setCursor(10, 30);
    display.print("Scanning...");
    return;
  }

  for (int i = 0; i < deviceCount; i++) {
    display.setCursor(5, 20 + i * 10);
    display.print(deviceNames[i]);
  }
}

// ================= BUTTON =================
void handleBluetoothPageButton(int button) {
  // no interaction yet
}
