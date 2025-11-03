#include <WiFi.h>
#include <WebServer.h>
#include "remote.h"
#include "steering.h"

WebServer server(80);

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Lenkung</title>
  <style>
    body {
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
      margin: 0;
      font-family: Arial, sans-serif;
      background-color: #f0f0f0;
    }

    h1 {
      font-size: 36px;
      margin-bottom: 30px;
    }

    button {
      font-size: 24px;
      padding: 20px 40px;
      margin: 10px;
      cursor: pointer;
      border: none;
      border-radius: 8px;
      background-color: #4CAF50;
      color: white;
      transition: background-color 0.3s;
    }

    button:hover {
      background-color: #45a049;
    }

    p {
      font-size: 24px;
      margin-top: 20px;
    }
  </style>
  <script>
    function sendCommand(cmd) {
      fetch('/' + cmd);
    }

    function updatePosition() {
      fetch('/pos')
        .then(response => response.text())
        .then(data => {
          document.getElementById("position").innerText = "Position: " + data;
        });
    }

    setInterval(updatePosition, 500); // Alle 500 ms abfragen
  </script>
</head>
<body>
  <h1>Lenkung</h1>
  <button onclick="sendCommand('left')">Links</button>
  <button onclick="sendCommand('right')">Rechts</button>
  <p id="position">Position: ...</p>
</body>
</html>
)rawliteral";

// --- WLAN-Verbindung ---
void connectToWiFi(const char* ssid, const char* password) {
  Serial.print("Verbinde mit WLAN: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nVerbunden! IP-Adresse: " + WiFi.localIP().toString());
}

// --- HTTP-Endpunkte ---
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleLeft() {
  moveLeft();
  server.send(200, "text/plain", "OK");
}

void handleRight() {
  moveRight();
  server.send(200, "text/plain", "OK");
}

void handlePosition() {
  int pos = getSteeringPosition();
  server.send(200, "text/plain", String(pos));
}

// --- Setup-Funktion für Webserver ---
void setupRemote(const char* ssid, const char* password) {
  connectToWiFi(ssid, password);

  server.on("/", handleRoot);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/pos", handlePosition); // Neue API für Position

  server.begin();
  Serial.println("Webserver gestartet.");
}

void handleRemoteClient() {
  server.handleClient();
}
