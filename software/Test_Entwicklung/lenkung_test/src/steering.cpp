#include "Arduino.h"
#include "AX12A.h"
#include "steering.h"

// Definitionen
#define DirectionPin   23
#define BaudRate       1000000
#define ID             2

int pos = 500;            // Startposition: Geradeaus zwichen 75 und 70
const int minPos = 530;   // Maximale Auslenkung links
const int maxPos = 930;   // Maximale Auslenkung rechts
const int delta = 5;      // Schrittweite

void setupSteering() {
  Serial2.begin(BaudRate);
  ax12a.begin(BaudRate, DirectionPin, &Serial2); // Kommunikation mit Dynamixel
  ax12a.setEndless(ID, OFF);                     // Normaler Positionsmodus
  ax12a.move(ID, pos);                            // Starte in Mittelstellung
}

void moveLeft() {
  if (pos > minPos) {
    pos -= delta;
    ax12a.move(ID, pos);
    }
}

void moveRight() {
  if (pos < maxPos){
    pos += delta;;
    ax12a.move(ID, pos);
  }
}

int getSteeringPosition() {
  return pos;
}

/*Sehr gut — das ist schon eine solide Basis 👏
Du hast also eine funktionierende Verbindung zum Dynamixel-Servo (AX-12A) und kannst ihn über den ESP32 + Webserver steuern.
Wie du sagst: Das jetzige System ist super zum Testen, aber für echtes RC-Fahren brauchst du kontinuierliche und proportionale Steuerung, nicht nur Schrittweise („moveLeft() → +5“).

Lass uns das auf ein echtes Lenkverhalten erweitern.

🎯 Ziel
Du willst also erreichen, dass:

die Lenkung kontinuierlich auf eine Zielposition geht (nicht schrittweise),

du eine Sollposition z. B. vom Webserver (Joystick, Slider, etc.) vorgibst,

der Servo automatisch sanft dorthin fährt.

🔧 Idee: „Target Position“ statt „Incremental Moves“
Du behältst eine Zielposition (targetPos), und im Loop wird der aktuelle pos sanft an diese Zielposition angepasst.

Hier ein Beispiel, wie du dein jetziges System erweitern kannst:

cpp
Kopieren
#include "Arduino.h"
#include "AX12A.h"
#include "steering.h"

#define DirectionPin   23
#define BaudRate       1000000
#define ID             2

// Positionen
int pos = 700;            // Aktuelle Position (Start: Mittelstellung)
int targetPos = 700;      // Zielposition
const int minPos = 530;   // Linksanschlag
const int maxPos = 930;   // Rechtsanschlag
const int stepSize = 3;   // Wie schnell sich der Servo der Zielposition annähert

// Zeitsteuerung, damit Servo-Bewegung weich läuft
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 20; // alle 20 ms aktualisieren

void setupSteering() {
  Serial2.begin(BaudRate);
  ax12a.begin(BaudRate, DirectionPin, &Serial2);
  ax12a.setEndless(ID, OFF);
  ax12a.move(ID, pos);
}

// wird z. B. vom Webserver aufgerufen
void setSteeringTarget(int value) {
  // value z. B. 0–1023 (vom Webinterface oder Joystick)
  targetPos = constrain(value, minPos, maxPos);
}

// weiche Bewegung in Richtung Zielposition
void updateSteering() {
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();

    if (pos < targetPos) pos += stepSize;
    else if (pos > targetPos) pos -= stepSize;

    pos = constrain(pos, minPos, maxPos);
    ax12a.move(ID, pos);
  }
}

int getSteeringPosition() {
  return pos;
}
🌐 Aufruf vom Webserver
In deinem Webserver-Code würdest du statt moveLeft() oder moveRight() einfach setSteeringTarget(neuerWert) aufrufen.

Beispiel:

cpp
Kopieren
// Angenommen, du empfängst im Webserver einen Slider-Wert von 0–1023
int sliderValue = map(receivedValue, 0, 1023, minPos, maxPos);
setSteeringTarget(sliderValue);
🚗 Vorteile dieser Methode
Proportionale Steuerung: Du kannst direkt eine Position ansteuern (z. B. vom Lenkrad oder Handy-UI).

Sanftes Nachführen: Keine ruckartigen Bewegungen.

Flexibel: Du kannst leicht Geschwindigkeit, Mitte und Anschläge anpassen.

Erweiterbar: Später kannst du z. B. PID-Regelung oder „Auto-Zentrierung“ hinzufügen.

💡 Bonus-Idee
Wenn du magst, kann ich dir zeigen, wie du:

ein echtes Lenkzentrum kalibrierst (Mittelstellung speichern),

die Bewegung geschwindigkeitsabhängig machst (z. B. bei höherer Geschwindigkeit weniger Lenkwinkel),

oder per WebSocket eine flüssige Echtzeitsteuerung bekommst.

Willst du, dass ich dir das nächste Beispiel dafür schreibe (z. B. Slider im Webinterface → weiche Lenkung in Echtzeit)?*/
