#include "WifiPage.h"
#include "TimePage.h"
#include <WiFi.h>
#include <time.h>

// -------- STORED NETWORKS --------
struct WifiCredential {
  const char* ssid;
  const char* password;
};

WifiCredential storedNetworks[] = {
  {"Rdx", "11100000"},
  {"Boss", "11100000"}
};

const int storedCount = sizeof(storedNetworks) / sizeof(storedNetworks[0]);

// -------- MENU STATES --------
enum WifiMenuState {
  WIFI_MENU_MAIN,
  WIFI_MENU_STORED,
  WIFI_MENU_SCAN,
  WIFI_MENU_MESSAGE
};

WifiMenuState menuState = WIFI_MENU_MAIN;

int selectedIndex = 0;
int scanResultCount = 0;
int scrollOffset = 0;

String messageText = "";
unsigned long messageTimer = 0;

// =================================================
void initWifiPage() {
  WiFi.mode(WIFI_STA);
}

void updateWifiPage() {

  // Auto exit message screen
  if (menuState == WIFI_MENU_MESSAGE) {
    if (millis() - messageTimer > 2000) {
      menuState = WIFI_MENU_MAIN;
      selectedIndex = 0;
    }
  }
}

// =================================================
void drawWifiPage(Adafruit_SSD1306 &display) {

  display.setTextSize(1);
  display.setCursor(5, 5);
  display.print("WiFi Menu");
  display.drawLine(0, 15, 120, 15, WHITE);

  if (menuState == WIFI_MENU_MESSAGE) {
    display.setCursor(10, 30);
    display.print(messageText);
    return;
  }

  // ---------------- MAIN MENU ----------------
  if (menuState == WIFI_MENU_MAIN) {

    const char* items[] = {
      "Update Time",
      "Stored WiFi",
      "Scan WiFi"
    };

    for (int i = 0; i < 3; i++) {

      if (i == selectedIndex)
        display.fillRect(0, 20 + i * 12, 120, 10, WHITE);

      display.setCursor(5, 22 + i * 12);
      display.setTextColor(i == selectedIndex ? BLACK : WHITE);
      display.print(items[i]);
      display.setTextColor(WHITE);
    }
  }

  // ---------------- STORED WIFI ----------------
  else if (menuState == WIFI_MENU_STORED) {

    for (int i = 0; i < storedCount; i++) {

      if (i == selectedIndex)
        display.fillRect(0, 20 + i * 12, 120, 10, WHITE);

      display.setCursor(5, 22 + i * 12);
      display.setTextColor(i == selectedIndex ? BLACK : WHITE);
      display.print(storedNetworks[i].ssid);
      display.setTextColor(WHITE);
    }
  }

  // ---------------- SCAN WIFI ----------------
  else if (menuState == WIFI_MENU_SCAN) {

    for (int i = 0; i < 3; i++) {

      int index = i + scrollOffset;
      if (index >= scanResultCount) break;

      if (index == selectedIndex)
        display.fillRect(0, 20 + i * 12, 120, 10, WHITE);

      display.setCursor(5, 22 + i * 12);
      display.setTextColor(index == selectedIndex ? BLACK : WHITE);
      display.print(WiFi.SSID(index));
      display.setTextColor(WHITE);
    }
  }
}

// =================================================
void handleWifiPageButton(int button) {

  // BUTTON MAP:
  // 0 = UP
  // 1 = DOWN
  // 2 = OK
  // 3 = NEXT (tool level right)
  // 4 = PREV (tool level left)
  // 5 = BACK

  // Ignore input during message screen
  if (menuState == WIFI_MENU_MESSAGE) return;

  // ================= TOOL LEVEL =================
  if (menuState == WIFI_MENU_MAIN) {

    if (button == 3) {   // RIGHT
      selectedIndex++;
      if (selectedIndex > 2) selectedIndex = 2;
    }

    if (button == 4) {   // LEFT
      selectedIndex--;
      if (selectedIndex < 0) selectedIndex = 0;
    }

    if (button == 2) {   // ENTER

      // ---- UPDATE TIME ----
      if (selectedIndex == 0) {

        if (WiFi.status() != WL_CONNECTED) {
          messageText = "No WiFi!";
        }
        else {
          bool success = fetchTimeFromInternet();

          if (success)
            messageText = "Time Updated!";
          else
            messageText = "Update Failed!";
        }

        menuState = WIFI_MENU_MESSAGE;
        messageTimer = millis();
      }

      // ---- STORED WIFI ----
      else if (selectedIndex == 1) {
        menuState = WIFI_MENU_STORED;
        selectedIndex = 0;
      }

      // ---- SCAN WIFI ----
      else if (selectedIndex == 2) {
        scanResultCount = WiFi.scanNetworks();
        menuState = WIFI_MENU_SCAN;
        selectedIndex = 0;
        scrollOffset = 0;
      }
    }
  }

  // ================= SUB MENU =================
  else {

    if (button == 0) {  // UP
      selectedIndex--;
      if (selectedIndex < 0) selectedIndex = 0;

      if (menuState == WIFI_MENU_SCAN &&
          selectedIndex < scrollOffset)
        scrollOffset--;
    }

    if (button == 1) {  // DOWN
      selectedIndex++;

      if (menuState == WIFI_MENU_STORED &&
          selectedIndex >= storedCount)
        selectedIndex = storedCount - 1;

      if (menuState == WIFI_MENU_SCAN &&
          selectedIndex >= scanResultCount)
        selectedIndex = scanResultCount - 1;

      if (menuState == WIFI_MENU_SCAN &&
          selectedIndex >= scrollOffset + 3)
        scrollOffset++;
    }

    if (button == 5) {  // BACK
      menuState = WIFI_MENU_MAIN;
      selectedIndex = 0;
      return;
    }

    if (button == 2) {  // SELECT

      if (menuState == WIFI_MENU_STORED) {

        messageText = "Connecting...";
        menuState = WIFI_MENU_MESSAGE;
        messageTimer = millis();

        WiFi.begin(
          storedNetworks[selectedIndex].ssid,
          storedNetworks[selectedIndex].password
        );

        unsigned long startAttempt = millis();

        while (WiFi.status() != WL_CONNECTED &&
               millis() - startAttempt < 5000) {
          delay(100);
        }

        if (WiFi.status() == WL_CONNECTED)
          messageText = "Connected!";
        else
          messageText = "Failed!";
      }
    }
  }
}