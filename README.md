# Smart Light | Room Simulator 

![IMG_4387](https://github.com/user-attachments/assets/a8ba303a-0b9f-41e4-aa1d-15d36f3af8ff)

Presence‑activated RGB light using an ESP32, an HC‑SR04 ultrasonic sensor, and a KY‑016 RGB LED. The light toggles when someone is detected close to the sensor and can also be controlled via a simple web page.


## What it does
- Measures distance with HC‑SR04; when an object is detected closer than ~8 cm, the light toggles on/off (with a 2 s debounce).
- Serves a minimal web UI to turn the light ON/OFF over Wi‑Fi.
- RGB “on” color is purple (R=128, G=0, B=255); “off” is all off.

Core sketch: [Smartlight.ino](./Smartlight.ino)  
Optional extra (clap toggle demo): [SoundSensor.ino](./SoundSensor.ino)

## Hardware and wiring
- ESP32 development board
- HC‑SR04 ultrasonic sensor
- KY‑016 RGB LED + 3 current‑limiting resistors (e.g., 220 Ω)

Pins (from Smartlight.ino):
- HC‑SR04
  - TRIG → GPIO 23
  - ECHO → GPIO 18
  - VCC → 5V, GND → GND
  - Important: Echo output is 5V. Use a level shifter or resistor divider to ~3.3V before ESP32 GPIO 18.
- KY‑016 RGB LED
  - R → GPIO 2 (through resistor)
  - G → GPIO 4 (through resistor)
  - B → GPIO 5 (through resistor)
  - Common → GND (common‑cathode) is assumed by the code. If your LED is common‑anode (common to 3.3V), invert the duty (e.g., write 255 - value).

Optional clap sensor (separate demo, not used with Wi‑Fi):
- Microphone signal → GPIO 15 (ADC). Note: On ESP32, ADC2 (GPIO 15) can’t be read while Wi‑Fi is active.

## Setup
Requirements:
- Arduino IDE with ESP32 boards package (Espressif)
- Libraries: ESPAsyncWebServer, AsyncTCP, NewPing

Configure Wi‑Fi in Smartlight.ino:
```cpp
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
```

Build and flash:
- Select your ESP32 board and port in Arduino IDE, then Upload.
- Open Serial Monitor (115200) to get the assigned IP address.

## Use
- Web UI: http://<device-ip>/
- Endpoints:
  - GET /led/on  → turn light on
  - GET /led/off → turn light off

## How it works (quick)
- Every ~300 ms, the HC‑SR04 distance is sampled via NewPing.
- If distance > 0 and < 8 cm and not in debounce, the code toggles the current light state and starts a 2 s debounce.
- Web commands keep the state in sync and immediately update the RGB output.

Built in a 24‑hour hackathon by:
- Jennifer Corte
- Aleanny Burgos
- Christian Ramirez
- Jeraldine Rodriguez
