#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include "Pages.h"
#include "TimePage.h"
#include "WifiPage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// -------- BUTTON PINS --------
#define BTN_UP     32
#define BTN_DOWN   33
#define BTN_LEFT   25
#define BTN_RIGHT  26
#define BTN_OK     27

PageType currentPage = PAGE_TIME;

unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;
bool inSubMenu = false;

// =================================================
// SETUP
// =================================================
void setup() {

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  showBootScreen();

  initTimePage();
  initWifiPage();
}


// =================================================
// LOOP
// =================================================
void loop() {

  display.clearDisplay();

  handleButtons();
  updateCurrentPage();
  drawCurrentPage();
  drawSidebar();

  display.display();
}


// =================================================
// BOOT SCREEN
// =================================================


void showBootScreen() {

  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(3);   // Bigger size for bold look

  String text = "RDX";

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  int x = (SCREEN_WIDTH - w) / 2;
  int y = (SCREEN_HEIGHT - h) / 2;

  display.setCursor(x, y);
  display.print(text);

  display.display();
  delay(2000);
}


// =================================================
// BUTTON HANDLING
// =================================================
void forwardButtonsToPage(int button) {

  switch (currentPage) {

    case PAGE_TIME:
      handleTimePageButton(button);
      break;

    case PAGE_WIFI:
      handleWifiPageButton(button);
      break;
  }
}

void handleButtons() {

  if (millis() - lastButtonPress < debounceDelay) return;

  // ---------------- TOOL LEVEL ----------------
  if (!inSubMenu) {

    if (digitalRead(BTN_DOWN) == LOW) {
      currentPage = (PageType)((currentPage + 1) % PAGE_COUNT);
      lastButtonPress = millis();
      return;
    }

    if (digitalRead(BTN_UP) == LOW) {
      currentPage = (PageType)((currentPage - 1 + PAGE_COUNT) % PAGE_COUNT);
      lastButtonPress = millis();
      return;
    }

    if (digitalRead(BTN_RIGHT) == LOW) {
      forwardButtonsToPage(3);  // NEXT SUB OPTION
      lastButtonPress = millis();
      return;
    }

    if (digitalRead(BTN_LEFT) == LOW) {
      forwardButtonsToPage(4);  // PREVIOUS SUB OPTION
      lastButtonPress = millis();
      return;
    }

    if (digitalRead(BTN_OK) == LOW) {
      inSubMenu = true;
      forwardButtonsToPage(2);  // ENTER
      lastButtonPress = millis();
      return;
    }
  }

  // ---------------- SUB MENU LEVEL ----------------
  else {

    if (digitalRead(BTN_UP) == LOW) {
      forwardButtonsToPage(0);  // UP
      lastButtonPress = millis();
      return;
    }

    if (digitalRead(BTN_DOWN) == LOW) {
      forwardButtonsToPage(1);  // DOWN
      lastButtonPress = millis();
      return;
    }

    if (digitalRead(BTN_LEFT) == LOW) {
      inSubMenu = false;        // BACK
      forwardButtonsToPage(5);  // BACK EVENT
      lastButtonPress = millis();
      return;
    }

    if (digitalRead(BTN_OK) == LOW) {
      forwardButtonsToPage(2);  // SELECT
      lastButtonPress = millis();
      return;
    }
  }
}


// =================================================
// PAGE CONTROL
// =================================================
void updateCurrentPage() {

  switch (currentPage) {

    case PAGE_TIME:
      updateTimePage();
      break;

    case PAGE_WIFI:
      updateWifiPage();
      break;
  }
}

void drawCurrentPage() {

  switch (currentPage) {

    case PAGE_TIME:
      drawTimePage(display);
      break;

    case PAGE_WIFI:
      drawWifiPage(display);
      break;
  }
}


// =================================================
// SIDEBAR
// =================================================
void drawSidebar() {

  int total = PAGE_COUNT;
  int barHeight = SCREEN_HEIGHT / total;

  for (int i = 0; i < total; i++) {

    if (i == currentPage) {
      display.fillRect(120, i * barHeight, 8, barHeight, WHITE);
    } else {
      display.drawRect(120, i * barHeight, 8, barHeight, WHITE);
    }
  }
}