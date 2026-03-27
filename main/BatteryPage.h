#ifndef BATTERY_PAGE_H
#define BATTERY_PAGE_H

#include <Adafruit_SSD1306.h>

void initBatteryPage();
void updateBatteryPage();
void drawBatteryPage(Adafruit_SSD1306 &display);
void handleBatteryPageButton(int button);

#endif