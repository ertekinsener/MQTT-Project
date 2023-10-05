#include <Arduino.h>

const int buttonPin = 2;
const int led1Pin = 3;
const int led2Pin = 4;
volatile int systemState = 0;
int buttonPressCount = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING);

  Serial.begin(9600);  
  sendStatus();
}

void loop() {
  // Your main code can go here if needed.
}

void buttonInterrupt() {
  systemState = (systemState + 1) % 4;
  buttonPressCount++; // Increment button press count
  updateLEDs();
  sendStatus();
}

void updateLEDs() {
  digitalWrite(led1Pin, (systemState & 0x01) ? HIGH : LOW);
  digitalWrite(led2Pin, (systemState & 0x02) ? HIGH : LOW);
}

void sendStatus() {
  Serial.print("$$");
  Serial.print(systemState);
  Serial.print(",");
  Serial.print((systemState & 0x01) ? "On" : "Off");
  Serial.print(",");
  Serial.print((systemState & 0x02) ? "On" : "Off");
  Serial.print(",");
  Serial.print(buttonPressCount);  
  Serial.println("**");
}
