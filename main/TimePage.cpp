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

    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
  }
}

// ================= DRAW =================
void drawTimePage(Adafruit_SSD1306 &display) {

  display.setTextSize(1);
  display.setCursor(5, 5);
  display.print("Date & Time");
  display.drawLine(0, 15, 120, 15, WHITE);

  display.setTextSize(1);

  if (!timeInitialized) {
    display.setCursor(10, 30);
    display.print("--/--/---- --:--:--");
    return;
  }

  char dateTimeStr[25];

  sprintf(dateTimeStr,
          "%02d/%02d/%04d %02d:%02d:%02d",
          timeinfo.tm_mday,
          timeinfo.tm_mon + 1,
          timeinfo.tm_year + 1900,
          timeinfo.tm_hour,
          timeinfo.tm_min,
          timeinfo.tm_sec);

  display.setCursor(5, 35);
  display.print(dateTimeStr);
}


// ================= REQUIRED FOR LINKER =================
void handleTimePageButton(int button) {
  // No buttons used currently
}