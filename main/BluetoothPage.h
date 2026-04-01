#ifndef BLUETOOTH_PAGE_H
#define BLUETOOTH_PAGE_H

#include <Adafruit_SSD1306.h>

void initBluetoothPage();
void updateBluetoothPage();
void drawBluetoothPage(Adafruit_SSD1306 &display);
void handleBluetoothPageButton(int button);

#endif
