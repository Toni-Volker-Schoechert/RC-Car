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
