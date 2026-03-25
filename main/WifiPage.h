#ifndef WIFI_PAGE_H
#define WIFI_PAGE_H

#include <Adafruit_SSD1306.h>

void initWifiPage();
void updateWifiPage();
void drawWifiPage(Adafruit_SSD1306 &display);
void handleWifiPageButton(int button);

#endif