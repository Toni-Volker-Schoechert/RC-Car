#include <Arduino.h>


// GPIO 18 Remote Pinnummer
int pin = 18;

unsigned long duration;


void setup() {

  Serial.begin(9600);

  pinMode(pin, INPUT);

}


void loop() {

  duration = pulseIn(pin, HIGH);

  Serial.println("Pulsdauer: ");
  Serial.print(duration);

}