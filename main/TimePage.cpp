#include "TimePage.h"
#include <WiFi.h>
#include <time.h>

struct tm timeinfo;

unsigned long lastTick = 0;
bool timeInitialized = false;

// ================= INIT =================
void initTimePage() {
  // Nothing needed for now
}

// ================= FETCH TIME =================
bool fetchTimeFromInternet() {

  if (WiFi.status() != WL_CONNECTED)
    return false;

  configTime(19800, 0, "pool.ntp.org");

  int retry = 0;

  while (!getLocalTime(&timeinfo) && retry < 10) {
    delay(500);
    retry++;
  }

  if (retry >= 10)
    return false;

  timeInitialized = true;
  lastTick = millis();

  return true;
}

// ================= UPDATE =================
void updateTimePage() {

  if (!timeInitialized) return;

  if (millis() - lastTick >= 1000) {
    lastTick = millis();

    timeinfo.tm_sec++;

    if (timeinfo.tm_sec >= 60) {
      timeinfo.tm_sec = 0;
      timeinfo.tm_min++;
    }

    if (timeinfo.tm_min >= 60) {
      timeinfo.tm_min = 0;
      timeinfo.tm_hour++;
    }

    if (timeinfo.tm_hour >= 24) {
      timeinfo.tm_hour = 0;
    }
  }
}

// ================= DRAW =================
void drawTimePage(Adafruit_SSD1306 &display) {

  display.setTextSize(1);
  display.setCursor(5, 5);
  display.print("Time");
  display.drawLine(0, 15, 120, 15, WHITE);

  display.setTextSize(2);

  if (!timeInitialized) {
    display.setCursor(10, 30);
    display.print("--:--:--");
    return;
  }

  char timeStr[10];
  sprintf(timeStr, "%02d:%02d:%02d",
          timeinfo.tm_hour,
          timeinfo.tm_min,
          timeinfo.tm_sec);

  display.setCursor(10, 30);
  display.print(timeStr);
}

// ================= REQUIRED FOR LINKER =================
void handleTimePageButton(int button) {
  // No buttons used currently
}