//Motion sensor & webserver 


#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <NewPing.h>

// Wi-Fi credentials
const char* ssid = "Your_SSID";         // Replace with your Wi-Fi SSID
const char* password = "Your_PASSWORD"; // Replace with your Wi-Fi password

// HC-SR04 ultrasonic sensor pins
#define TRIGGER_PIN 23
#define ECHO_PIN 18
#define MAX_DISTANCE 200

// RGB LED pins
#define RED_PIN 2
#define GREEN_PIN 4
#define BLUE_PIN 5

// Motion detection logic
bool ledOn = false;
bool debounce = false;
unsigned long lastTriggerTime = 0;
const unsigned long DEBOUNCE_DELAY = 2000;  // 2 seconds

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Create an AsyncWebServer on port 80
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialize LED pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define routes for the web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // HTML with updated design
    String html = "<!DOCTYPE html><html lang='en'><head>"
                  "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                  "<title>Smart Light Controller</title>"
                  "<style>"
                  "body {font-family: 'Arial', sans-serif; background: linear-gradient(to bottom, black, red); color: white; margin: 0; padding: 0;}"
                  "header {text-align: center; padding: 20px; font-size: 48px; font-weight: bold; background: -webkit-linear-gradient(white, grey); -webkit-background-clip: text; -webkit-text-fill-color: transparent;}"
                  "main {display: flex; flex-direction: column; align-items: center; justify-content: center; height: 80vh;}"
                  "button {padding: 15px 30px; margin: 10px; font-size: 18px; border: none; cursor: pointer; border-radius: 8px; color: white; background: linear-gradient(to bottom, black, #333);}"
                  ".on {color: green;}"
                  ".off {color: red;}"
                  ".on:hover {background: linear-gradient(to bottom, #333, black);}"
                  ".off:hover {background: linear-gradient(to bottom, #333, black);}"
                  "#status {margin-top: 20px; font-size: 20px;}"
                  "</style></head><body>"
                  "<header>Smart Light Controller</header>"
                  "<main>"
                  "<button class='on' onclick=\"sendRequest('on')\">Turn LED ON</button>"
                  "<button class='off' onclick=\"sendRequest('off')\">Turn LED OFF</button>"
                  "<p id='status'>LED is currently OFF</p>"
                  "</main>"
                  "<script>"
                  "function sendRequest(state) {"
                  "  fetch(`/led/${state}`).then(response => {"
                  "    if (response.ok) {"
                  "      return response.text();"
                  "    } else {"
                  "      throw new Error('Request failed');"
                  "    }"
                  "  }).then(text => {"
                  "    document.getElementById('status').innerText = text;"
                  "  }).catch(error => {"
                  "    console.error('Error:', error);"
                  "  });"
                  "}"
                  "</script></body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/led/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("LED turned ON via web");
    ledOn = true; // Sync motion logic with web command
    analogWrite(RED_PIN, 128);   // Medium red
    analogWrite(GREEN_PIN, 0);   // No green
    analogWrite(BLUE_PIN, 255);  // Full blue → purple
    request->send(200, "text/plain", "LED is currently ON");
  });

  server.on("/led/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("LED turned OFF via web");
    ledOn = false; // Sync motion logic with web command
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
    request->send(200, "text/plain", "LED is currently OFF");
  });

  // Start the Async web server
  server.begin();
}

void loop() {
  // Motion detection logic
  delay(300);  // Slight delay between pings
  unsigned int distance = sonar.ping_cm();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < 8 && !debounce) {
    debounce = true;
    lastTriggerTime = millis();

    // Toggle the light state
    ledOn = !ledOn;

    if (ledOn) {
      Serial.println("Person detected — turning LED ON");
      analogWrite(RED_PIN, 128);   // Medium red
      analogWrite(GREEN_PIN, 0);   // No green
      analogWrite(BLUE_PIN, 255);  // Full blue → purple
    } else {
      Serial.println("No person — turning LED OFF");
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
    }
  }

  if (debounce && (millis() - lastTriggerTime > DEBOUNCE_DELAY)) {
    debounce = false;
  }
}
