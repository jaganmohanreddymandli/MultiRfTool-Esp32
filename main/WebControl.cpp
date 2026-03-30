#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

// ===== AP CONFIG =====
const char* ssid = "Rdx-MicroServer";
const char* password = "11100000";

// ===== HTML PAGE =====
String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP Control</title>
  <style>
    body { text-align:center; font-family:sans-serif; }
    button {
      width:80px; height:50px;
      margin:10px; font-size:16px;
    }
  </style>
</head>
<body>

<h2>ESP32 Remote</h2>

<button onclick="send('UP')">UP</button><br>
<button onclick="send('LEFT')">LEFT</button>
<button onclick="send('OK')">OK</button>
<button onclick="send('RIGHT')">RIGHT</button><br>
<button onclick="send('DOWN')">DOWN</button>

<script>
function send(cmd){
  fetch("/cmd?btn=" + cmd);
}
</script>

</body>
</html>
)rawliteral";

// ===== HANDLE ROOT =====
void handleRoot() {
  server.send(200, "text/html", webpage);
}

// ===== HANDLE BUTTON =====
extern void forwardButtonsToPage(int button);

void handleCommand() {

  String btn = server.arg("btn");

  if (btn == "UP") forwardButtonsToPage(0);
  if (btn == "DOWN") forwardButtonsToPage(1);
  if (btn == "OK") forwardButtonsToPage(2);
  if (btn == "RIGHT") forwardButtonsToPage(3);
  if (btn == "LEFT") forwardButtonsToPage(4);

  server.send(200, "text/plain", "OK");
}

// ===== INIT =====
void initWebControl() {

  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/cmd", handleCommand);

  server.begin();
}

// ===== LOOP =====
void updateWebControl() {
  server.handleClient();
}

