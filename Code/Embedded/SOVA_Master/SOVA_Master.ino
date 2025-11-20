#include <Wire.h>
#include "HUSKYLENS.h"
#include <LiquidCrystal_I2C.h>

HUSKYLENS huskylens;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // use the scanner result here

const int WATER_BOTTLE_ID = 2;
const int LED_PIN = 13;

void setup() {
  Serial.begin(9600);
  Serial.println("START");
  delay(500);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Wire.begin();
  // Optional, if noise: Wire.setClock(50000);

  // Init LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HUSKY + LCD");

  // Init HuskyLens
  if (!huskylens.begin(Wire)) {
    Serial.println("HUSKYLENS not found");
    lcd.setCursor(0, 1);
    lcd.print("Husky ERR");
    // don’t while(1); keep loop alive
  } else {
    Serial.println("HUSKYLENS OK");
    lcd.setCursor(0, 1);
    lcd.print("Ready");
  }
}

void loop() {
  if (!huskylens.request()) {
    Serial.println("Req fail");
    lcd.setCursor(0, 0);
    lcd.print("No Husky    ");
    delay(300);
    return;
  }

  if (!huskylens.available()) {
    lcd.setCursor(0, 0);
    lcd.print("Bottle: NO  ");
    digitalWrite(LED_PIN, LOW);
    delay(200);
    return;
  }

  bool bottle = false;

  while (huskylens.available()) {
    HUSKYLENSResult r = huskylens.read();
    if (r.command == COMMAND_RETURN_BLOCK && r.ID == WATER_BOTTLE_ID) {
      bottle = true;
    }
  }

  if (bottle) {
    digitalWrite(LED_PIN, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Bottle: YES ");
  } else {
    digitalWrite(LED_PIN, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Bottle: NO  ");
  }

  delay(100);
}
