const int buttonPin = 2;
const int relayPin = 5;
const int lowBeamPin = 3;
const int highBeamPin = 4;

bool lastButtonState = HIGH;
bool fogOn = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 40; // Button debounce

// Glitch filter timing (ms)
const unsigned long glitchTime = 400;  // longer filtering for beam glitches
const unsigned long gracePeriod = 1000; // ms before forcing OFF

// For glitch filter
unsigned long lastLowBeamHigh = 0;
unsigned long lastHighBeamHigh = 0;
bool filteredLowBeam = false;
bool filteredHighBeam = false;

// For grace period
unsigned long beamsOffStart = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Button to GND
  pinMode(relayPin, OUTPUT);
  pinMode(lowBeamPin, INPUT);
  pinMode(highBeamPin, INPUT);
  digitalWrite(relayPin, HIGH);     // Relay OFF at startup

  Serial.begin(9600);
  Serial.println("Setup complete. Waiting for button...");
}

void loop() {
  // -------- Beam Input Glitch Filter --------
  bool rawLowBeam = digitalRead(lowBeamPin);
  bool rawHighBeam = digitalRead(highBeamPin);

  // Low beam filter
  if (rawLowBeam == HIGH) {
    lastLowBeamHigh = millis();
    filteredLowBeam = true;
  } else if (millis() - lastLowBeamHigh > glitchTime) {
    filteredLowBeam = false;
  }

  // High beam filter
  if (rawHighBeam == HIGH) {
    lastHighBeamHigh = millis();
    filteredHighBeam = true;
  } else if (millis() - lastHighBeamHigh > glitchTime) {
    filteredHighBeam = false;
  }

  bool beamsOn = filteredLowBeam || filteredHighBeam;

  // -------- Button Debounce --------
  bool buttonState = digitalRead(buttonPin);
  static bool debouncedButton = HIGH;

  if (buttonState != lastButtonState) {
    lastDebounceTime = millis();
    lastButtonState = buttonState;
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    debouncedButton = buttonState;
  }

  // -------- Debug Info --------
  Serial.print("Low beam input: ");
  Serial.print(rawLowBeam == HIGH ? "HIGH " : "LOW ");
  Serial.print("Filtered: ");
  Serial.print(filteredLowBeam ? "ON " : "OFF ");
  Serial.print("| High beam input: ");
  Serial.print(rawHighBeam == HIGH ? "HIGH " : "LOW ");
  Serial.print("Filtered: ");
  Serial.print(filteredHighBeam ? "ON " : "OFF ");
  Serial.print("| beamsOn logic: ");
  Serial.println(beamsOn ? "TRUE (toggle allowed)" : "FALSE (forced OFF)");

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

  // -------- Grace Period Forced OFF --------
  if (!beamsOn) {
    if (beamsOffStart == 0) beamsOffStart = millis();
    if ((millis() - beamsOffStart > gracePeriod) && fogOn) {
      fogOn = false;
      Serial.println("No beam input — fog light forced OFF and reset (after grace period).");
    }
  } else {
    beamsOffStart = 0;
  }

  static bool lastRelayState = false;
  if (lastRelayState != fogOn) {
    Serial.print("Relay OUTPUT: ");
    Serial.println(fogOn ? "ON (LOW)" : "OFF (HIGH)");
    lastRelayState = fogOn;
  }

  digitalWrite(relayPin, fogOn ? LOW : HIGH);

  delay(10); // Fast, but gives enough time for debug output
}

