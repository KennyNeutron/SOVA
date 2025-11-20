#include <Wire.h>
#include "HUSKYLENS.h"

HUSKYLENS huskylens;

const int WATER_BOTTLE_ID = 1;   // change if bottle is another ID
const int LED_PIN = 13;

void printResult(const HUSKYLENSResult& result);

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println(F("HUSKYLENS Water Bottle Detector - I2C Mode"));

  Wire.begin();

  Serial.println(F("Initializing HUSKYLENS..."));
  if (!huskylens.begin(Wire)) {
    Serial.println(F("ERROR: HUSKYLENS not found. Check wiring and protocol (I2C)."));
    // Do NOT lock in while(1) so you still see loop prints:
    return;
  }

  Serial.println(F("HUSKYLENS initialized successfully."));
  Serial.println(F("Make sure it's in the correct algorithm mode and bottle is visible on screen."));
}

void loop() {
  // If begin() failed, huskylens won't work; just show a heartbeat
  static bool initChecked = false;
  static bool initOK = false;

  if (!initChecked) {
    // Try one dummy request to see if device responds
    initOK = huskylens.request();
    initChecked = true;
    if (!initOK) {
      Serial.println(F("WARNING: HUSKYLENS did not respond to first request."));
    }
  }

  if (!huskylens.request()) {
    Serial.println(F("Request failed (no response from HUSKYLENS)."));
    digitalWrite(LED_PIN, LOW);
    delay(300);
    return;
  }

  if (!huskylens.isLearned()) {
    Serial.println(F("No learned objects stored in HUSKYLENS."));
    digitalWrite(LED_PIN, LOW);
    delay(500);
    return;
  }

  if (!huskylens.available()) {
    Serial.println(F("No object detected in current frame."));
    digitalWrite(LED_PIN, LOW);
    delay(200);
    return;
  }

  bool waterBottleDetected = false;

  while (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();

    // Only handle BLOCK results (bounding boxes)
    if (result.command == COMMAND_RETURN_BLOCK) {
      Serial.print(F("Detected ID: "));
      Serial.println(result.ID);

      if (result.ID == WATER_BOTTLE_ID) {
        waterBottleDetected = true;
        Serial.println(F(">>> WATER BOTTLE DETECTED <<<"));
        printResult(result);
      }
    }
  }

  if (waterBottleDetected) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(100);
}

void printResult(const HUSKYLENSResult& result) {
  Serial.print(F("ID: "));
  Serial.println(result.ID);
  Serial.print(F("Center: ("));
  Serial.print(result.xCenter);
  Serial.print(F(", "));
  Serial.print(result.yCenter);
  Serial.println(F(")"));

  Serial.print(F("Size: "));
  Serial.print(result.width);
  Serial.print(F(" x "));
  Serial.println(result.height);
}
