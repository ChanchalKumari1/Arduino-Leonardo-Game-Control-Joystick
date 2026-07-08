#include "Mouse.h"

const int xAxis = A0;
const int yAxis = A1;
const int buttonPin = 2;
const int ledPin = 13;   // Built-in LED on the Leonardo

int range = 12;
int responseDelay = 5;
int threshold = range / 4;
int center = range / 2;

bool lastButtonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  
  Mouse.begin();
}

void loop() {
  // 1. Move the cursor
  int xReading = readAxis(xAxis);
  int yReading = readAxis(yAxis);
  Mouse.move(xReading, yReading, 0);

  // 2. Read the button
  bool currentButtonState = digitalRead(buttonPin);
  
  // DEBUG: Blink the LED when the button is pressed (LOW)
  if (currentButtonState == LOW) {
    digitalWrite(ledPin, HIGH);  // Turn LED ON
  } else {
    digitalWrite(ledPin, LOW);   // Turn LED OFF
  }

  // 3. Handle Left Click (Edge detection: only on the moment of press)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    Mouse.click(MOUSE_LEFT);  
    delay(10);
  }
  
  lastButtonState = currentButtonState;
  delay(responseDelay);
}

int readAxis(int thisAxis) {
  int reading = analogRead(thisAxis);           
  reading = map(reading, 0, 1023, 0, range);    
  int distance = reading - center;              
  if (abs(distance) < threshold) {
    distance = 0;
  }
  return distance; 
}