const int buttonPin = 2;
const int relayPin = 5;
const int lowBeamPin = 3;
const int highBeamPin = 4;

bool lastButtonState = HIGH;
bool fogOn = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 40; // 40ms is good for most mechanical buttons

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Button to GND
  pinMode(relayPin, OUTPUT);
  pinMode(lowBeamPin, INPUT); // Use voltage divider to 12V signal
  pinMode(highBeamPin, INPUT);
  digitalWrite(relayPin, HIGH);     // Relay OFF at startup

  Serial.begin(9600);
  Serial.println("Setup complete. Waiting for button...");
}

void loop() {
  bool buttonState = digitalRead(buttonPin);
  static bool debouncedButton = HIGH;

  // Debounce logic
  if (buttonState != lastButtonState) {
    lastDebounceTime = millis();
    lastButtonState = buttonState;
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    debouncedButton = buttonState;
  }

  bool lowBeam = digitalRead(lowBeamPin);
  bool highBeam = digitalRead(highBeamPin);

  bool beamsOn = (lowBeam == HIGH) || (highBeam == HIGH);

  // Only toggle ON the transition from not pressed to pressed (debounced)
  static bool lastDebouncedButton = HIGH;
  if (lastDebouncedButton == HIGH && debouncedButton == LOW) {
    if (beamsOn) {
      fogOn = !fogOn;
      Serial.print("TOGGLE FOG: ");
      Serial.println(fogOn ? "ON" : "OFF");
    } else {
      Serial.println("Button pressed, but NO BEAM input active — FOG NOT ALLOWED");
    }
  }
  lastDebouncedButton = debouncedButton;

  // Force OFF if both beams are OFF
  if (!beamsOn && fogOn) {
    fogOn = false;
    Serial.println("No beam input — fog light forced OFF and reset.");
  }

  static bool lastRelayState = false;
  if (lastRelayState != fogOn) {
    Serial.print("Relay OUTPUT: ");
    Serial.println(fogOn ? "ON (LOW)" : "OFF (HIGH)");
    lastRelayState = fogOn;
  }

  digitalWrite(relayPin, fogOn ? LOW : HIGH);

  delay(5); // Fast loop, no missed presses
}


