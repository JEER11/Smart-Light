//extra >> turns on with one clap, and off with a second clap


int mic = 15;
int micval;
bool LEDStatus = false;

// RGB pins
const int RED_PIN = 2;
const int GREEN_PIN = 4;
const int BLUE_PIN = 5;

void setup(){
  Serial.begin(115200);
  pinMode(mic, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Start with light off
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
}

void loop(){
  micval = analogRead(mic);
  Serial.println(micval);

  if (micval == 816  micval == 833  micval == 1143  micval == 926  
      micval == 909  micval == 848  micval == 815) {

    Serial.println("One clap detected!");

    if (!LEDStatus) {
      // Turn ON RGB light (example: purple)
      analogWrite(RED_PIN, 128);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 255);
      LEDStatus = true;
      Serial.println("RGB Light ON");
    } else {
      // Turn OFF RGB light
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      LEDStatus = false;
      Serial.println("RGB Light OFF");
    }

    delay(500); // Prevent double detection
  }
}
