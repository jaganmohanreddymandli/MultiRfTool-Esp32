#ifndef TIME_PAGE_H
#define TIME_PAGE_H

#include <Adafruit_SSD1306.h>

void initTimePage();
void updateTimePage();
void drawTimePage(Adafruit_SSD1306 &display);
void handleTimePageButton(int button);
bool fetchTimeFromInternet();

#endif