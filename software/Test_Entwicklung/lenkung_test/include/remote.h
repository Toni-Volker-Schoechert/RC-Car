#ifndef REMOTE_H
#define REMOTE_H

// Initialisiert WLAN-Verbindung und Webserver (inkl. LED-Steuerung per Webseite)
void setupRemote(const char* ssid, const char* password);

// Wird in loop() aufgerufen, damit der Webserver Anfragen verarbeitet
void handleRemoteClient();

#endif
