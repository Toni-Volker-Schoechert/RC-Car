#ifndef STEERING_H
#define STEERING_H

// Initialisiert den Servomotor mit ID und Startposition
void setupSteering();

// Position um x Schritte nach links verstellen
void moveLeft();

// Position um x Schritte nach rechts verstellen
void moveRight();

// Gibt die aktuelle Position zurück
int getSteeringPosition();

#endif
