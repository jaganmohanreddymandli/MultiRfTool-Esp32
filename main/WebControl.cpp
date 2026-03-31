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
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 Control</title>

<style>
body {
  margin:0;
  font-family: 'Segoe UI', sans-serif;
  background: linear-gradient(135deg, #1e1e2f, #2c2c54);
  color:white;
  text-align:center;
}

h2 {
  margin-top:20px;
  font-weight:500;
}

.container {
  display:flex;
  flex-direction:column;
  align-items:center;
  justify-content:center;
  margin-top:30px;
}

.grid {
  display:grid;
  grid-template-columns: 80px 80px 80px;
  grid-gap:15px;
}

.btn {
  width:80px;
  height:80px;
  border-radius:20px;
  border:none;
  font-size:18px;
  background: rgba(255,255,255,0.1);
  color:white;
  backdrop-filter: blur(10px);
  box-shadow: 0 4px 15px rgba(0,0,0,0.3);
  transition: 0.2s;
}

.btn:active {
  transform: scale(0.9);
  background: #00adb5;
}

.center {
  background: #00adb5;
  font-weight:bold;
}

.footer {
  margin-top:30px;
  font-size:12px;
  opacity:0.6;
}
</style>
</head>

<body>

<h2>ESP32 Remote</h2>

<div class="container">

  <div class="grid">
    <div></div>
    <button class="btn" onclick="send('UP')">▲</button>
    <div></div>

    <button class="btn" onclick="send('LEFT')">◀</button>
    <button class="btn center" onclick="send('OK')">●</button>
    <button class="btn" onclick="send('RIGHT')">▶</button>

    <div></div>
    <button class="btn" onclick="send('DOWN')">▼</button>
    <div></div>
  </div>

</div>

<div class="footer">
  Connected to ESP32
</div>

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

