# Arduino Leonardo - HW-504 Joystick JoyStick Controller

**Turn your Arduino Leonardo into a plug-and-play USB HID JoyStick to Play Games!**

This project uses the popular HW-504 analog joystick module to control your computer's cursor.

## Features
- **Plug-and-Play HID:** Emulates a standard USB mouse using the native USB capabilities of the ATmega32u4 chip.
- **Precise Cursor Control:** Analog axes with a built-in "dead zone" to prevent cursor drift.
- **Physical Click Feedback:** The onboard LED (Pin 13) lights up whenever the joystick button is pressed.
- **Customizable Speed:** Adjust cursor sensitivity easily via a single variable.

## Hardware Requirements
| Component | Quantity |
| :--- | :--- |
| Arduino Leonardo | 1 |
| HW-504 Analog Joystick Module | 1 |
| Jumper Wires (Male-to-Female or Male-to-Male) | 5 |


## Wiring Diagram
Connect the HW-504 joystick to your Arduino Leonardo as follows:

| HW-504 Pin | Label | Connect to Arduino |
| :--- | :--- | :--- |
| 1 | **GND** | **GND** |
| 2 | **+5V** | **5V** |
| 3 | **VRx** (X-Axis) | **A0** |
| 4 | **VRy** (Y-Axis) | **A1** |
| 5 | **SW** (Button) | **Digital Pin 2** |

![Pinout](<Arduino Leonardo.png>)

*(Optional: The built-in LED on Pin 13 is used for visual feedback.)*

![Wiring diagram placeholder - connect GND to GND, 5V to 5V, VRx to A0, VRy to A1, SW to Pin 2]

## Software Setup (Arduino IDE)
1.  Install the [Arduino IDE](https://www.arduino.cc/en/software) if you haven't already.
2.  Select your board: **Tools** -> **Board** -> **Arduino Leonardo**.
3.  Select the correct port: **Tools** -> **Port** -> (Select your Leonardo).
4.  No external libraries are required; this sketch uses the built-in `Mouse.h` library.

## The Code
Copy and paste the following code into your Arduino IDE and upload it to your Leonardo.

```cpp
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
```

## How to Use
1.  Upload the sketch to your Arduino Leonardo.
2.  The cursor will move immediately when you tilt the joystick.
3.  Press down on the joystick (the "SW" button) to perform a **left-click**.
4.  **Visual Feedback:** The built-in LED (Pin 13) will turn ON while the button is pressed.

## Customization & Upgrades

### Adjusting Cursor Speed
To make the cursor faster or slower, modify the `range` variable on line 8:
- **Higher number** (e.g., `20`) = Faster cursor movement.
- **Lower number** (e.g., `6`) = Slower, more precise movement.

### Adding a Right-Click Button
If you want to add a physical right-click button, wire an external pushbutton between **Pin 3** and **GND**. Add this code block inside the `loop()` function right after the left-click logic:

```cpp
// --- Right Click (Pin 3) ---
if (digitalRead(3) == LOW) {
  Mouse.click(MOUSE_RIGHT);
  delay(100); // Debounce delay
}
```


## Troubleshooting

| Issue | Solution |
| :--- | :--- |
| **Cursor doesn't move** | Ensure you are using an **Arduino Leonardo**, not an Uno or Mega. Double-check the wiring for VRx (A0) and VRy (A1). |
| **Button doesn't click** | Your code uses `INPUT_PULLUP`. The SW pin must be connected to **GND** when pressed. Perform the "paperclip test": briefly connect Pin 2 directly to GND. If the LED lights up, your HW-504 button is faulty or miswired. |
| **Cursor drifts when idle** | Adjust the `threshold` variable (line 10). Increase it (e.g., `threshold = range / 3`) to widen the dead zone. |
| **Double clicks on single press** | Increase the `delay(10);` inside the click handler to `delay(50);` to add more debounce time. |
