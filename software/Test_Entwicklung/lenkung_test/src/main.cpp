#include <Arduino.h>
#include "remote.h"
#include "steering.h"

#ifndef WiFi_SSID
#define WiFi_SSID "Leobots"
#define WiFi_PW   "leobots42"
#endif

void setup() {
  Serial.begin(9600);
  setupSteering();                 // Initialisiert Dynamixel
  setupRemote(WiFi_SSID, WiFi_PW); // Startet WLAN + Webserver
}

void loop() {
  handleRemoteClient();           // Bearbeitet Webanfragen
}
