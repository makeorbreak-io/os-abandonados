

#include <Wire.h>
#include <LCD.h>
#include <DateTime.h>
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
   Light sensor, connected to the Analog Pin #2,, ranging from 0 to 1023 (10 bits)
*/
int lightSensorPin = 2;

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
   Variable containing the value from analogRead(humiditySensorPin)
*/
int humidityValue;

/**
   Variable containing the value from analogRead(lightSensorPin)
*/
int lightValue;

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
double humidityPercentage;

/**
   The average time, in seconds, between each automated bomb activation
*/
double averageTimer = 0;

/**
   Number of 50ms cycles between each automated cycle, the counter resets when a manual bomb activation occurs
*/
int cyclesAverage = 0;

/**
   Index used to calculate the timer average
*/
int timerAverageIndex = 1;

/**
   Default value of delay between each iteration
*/
int defaultDelay = 50;

/**
 * The average humidity, in %
 */
double humidityGlobalAverage = 0;

/**
 * Index used to calculate the humidity average
 */
int humidityAverageIndex = 1;

void setup() {
  pinMode(relePin, OUTPUT);
}

void loop() {

  humidityValue = analogRead(humiditySensorPin);
  lightValue = analogRead(lightSensorPin);

  buttonstate = digitalRead(buttonPin);

  humidityPercentage = (humidityValue * 100) / 1023;
  lightPercentage = (lightValue * 100) / 1023);

  if (buttonState == 0) {

  checkIfBombNeedsToBeActivated();

    lcdUpdate();

    deactivateBomb();

    delay(defaultDelay);

    timerUpdate();

  } else { // Manual water bomb activation

    manualMode();

  }

  calculateAverageHumidity();

}

/**
   Activates the water bomb if the humidity percentage goes below 70% (717/1023 = 70%), the bomb is deactivated, the rele is deactivated and the count timer is 0
*/
void checkIfBombNeedsToBeActivated() {
  if ((humidityValue <= 717) && (waterBombActivationTimer <= 0) && (releaux == 0) && (minimumDelayBetweenBombActivationTimer <= 0)) {
    activateBomb();
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

  cyclesAverage = cyclesAverage + 1;
}

/**
   Updates the LCD every 5 seconds (10 50ms cycles)
*/
void lcdUpdate() {
  if (lcdUpdateTimer = 0) { // atualizar valor mostrador

    lcd.begin(16, 2); //resolução do mostrador
    lcd.setCursor(0, 0); //início
    lcd.write("Hum:",  humidityPercentage, "% Light:", lightPercentage);

    lcdUpdateTimer = 100;
  }
}

/**
   Deactivates the water bomb if the 1.5 seconds timer reaches 0 and the bomb is activated
*/
void deactivateBomb() {
  if ((waterBombActivationTimer == 0) && (releaux == 1)) {
    digitalWrite(relePin, LOW);
    releaux = 0;
  }
}

/**
   Activates the water bomb
*/
void activateBomb() {
  digitalWrite(relePin, HIGH);
  releaux = 1;
}

/**
   Calculates the average time between each automated bomb activation
*/
void calculateAverageTime() {
  averageTimer = (averageTimer * timerAverageIndex + cyclesAverage * defaultDelay) / (timerAverageIndex + 1);
  timerAverageIndex = timerAverageIndex + 1;
}

/**
   Manual mode
*/
void manualMode() {
  activateBomb();
  cyclesAverage = 0;
}

/**
 * Calculates the average humidity value, in %
 */
void calculateAverageHumidity() {
  humidityGlobalAverage = (humidityGlobalAverage * humidityAverageIndex + humidityPercentage) / (humidityAverageIndex + 1);
  humidityAverageIndex = humidityAverageIndex + 1;
}

