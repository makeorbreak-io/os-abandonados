

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27 //Define I2C Address where the PCF8574A is

#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

/**
   Humidity sensor, connected to the Analog Pin #1, ranging from 0 to 1023 (10 bits)
*/
int humiditySensorPin = 1;

/**
   Rele, connected to the Digital Pin #1, ranging from 0 to 1
*/
int relePin = 1;

/**
   Manual button, connected to the Digital Pin #4, ranging from 0 to 1
*/
int buttonPin = 4;

/**
   Variable containing the value from analogRal(humiditySensorPin)
*/
int humidityValue;

/**
   Represents the number of 50ms cycles the water bomb will be activated
*/
int waterBombActivationTimer = 0;

/**
   Variable containing the value from digitalRead(buttonPin)
*/
int buttonState;

/**
   Rele's state
*/
int releaux = 0;

/**
   Minimum required time between the water bomb activations
*/
int minimumDelayBetweenBombActivationTimer = 0;

/**
   Timer to update the LCD
*/
int lcdUpdateTimer = 100;

/**
   humidityValue converted to %
*/
double percentage;

void setup() {
  pinMode(rele, OUTPUT);
}

void loop() {

  humidityValue = analogRead(humiditySensorPin);
  buttonstate = digitalRead(buttonPin);
  percentage = (humitystate * 100) / 1023;

  if (buttonState == 0) {

    // checkIfBombNeedsToBeActivated(humidityValue, waterBombActivationTimer, minimumDelayBetweenBombActivationTimer, releaux);
    checkIfBombNeedsToBeActivated();

    /**
       Updates the LCD every 5 seconds (10 50ms cycles)
    */
    if (lcdUpdateTimer = 0) { // atualizar valor mostrador

      LCD.begin(16, 2); //resolução do mostrador
      lcd.setCursor(0, 0); //início
      lcd.write("Humidade: ",  percentage);

      lcdUpdateTimer = 100;
    }

    /**
       Deactivates the water bomb if the 1.5 seconds timer reaches 0 and the bomb is activated
    */
    if ((waterBombActivationTimer == 0) && (releaux == 1)) {
      digitalWrite(relePin, LOW);
      releaux = 0;
    }

    delay(50);

    timerUpdate();


  } else { // Manual water bomb activation

    digitalWrite(relePin, HIGH);
    releaux = 1;

  }

}

/**
   Activates the water bomb if the humidity percentage goes below 70% (717/1023 = 70%), the bomb is deactivated, the rele is deactivated and the count timer is 0
*/
void checkIfBombNeedsToBeActivated() {
  if ((humidityValue < 717) && (waterBombActivationTimer == 0) && (releaux == 0) && (minimumDelayBetweenBombActivationTimer == 0)) {
    digitalWrite(relePin, HIGH);
    releaux = 1;
    waterBombActivationTimer = 30;
    minimumDelayBetweenBombActivationTimer = 800;
  }
}

/**
   Updates all timers
*/
void timerUpdate() {
  if (waterBombActivationTimer != 0) {
    waterBombActivationTimer = waterBombActivationTimer - 1;
  }

  if (minimumDelayBetweenBombActivationTimer != 0) {
    minimumDelayBetweenBombActivationTimer = minimumDelayBetweenBombActivationTimer - 1;
  }

  lcdUpdateTimer = lcdUpdateTimer - 1;
}


