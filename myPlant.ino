
/**
   Light sensor, connected to the Analog Pin #2,, ranging from 0 to 1023
*/
int lightSensorPin = 2;

/**
   Humidity sensor, connected to the Analog Pin #1, ranging from 0 (wet) to 1023 (dry)
*/
int humiditySensorPin = 1;

/**
   Rele, connected to the Digital Pin #10, ranging from 0 to 1
*/
int relePin = 10;

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
   Represents the default number of 50ms cycles the water bomb will be activated
*/
int waterBombActivationTimerDefault = 30; // 30 = 1,5s

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
   Default minimum required time between the water bomb activations
*/
int minimumDelayBetweenBombActivationTimerDefault = 1200; // 1200 = 1 min
/**
   Timer to update the LCD
*/
int lcdUpdateTimer = 20;

/**
   humidityValue converted to %
*/
double humidityPercentage;

/**
   lightValue converted to %
*/
double lightPercentage;

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
int timerAverageIndex = 0;

/**
   Default value of delay between each iteration
*/
int defaultDelay = 50;

/**
   The average humidity, in %
*/
double humidityGlobalAverage = 0;

/**
   Index used to calculate the humidity average
*/
int humidityAverageIndex = 0;

/**
   Standard time it takes for the lcd to update (in # of 50ms cycles)
*/
int lcdStandardTimer = 12000; // 12000 = 10 mins

/**
   Minimum humidity desired for the plant (358 = ~65%)
*/
int minimumHumidityDesired = 358;


void setup() {
  pinMode(relePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  humidityValue = analogRead(humiditySensorPin);
  lightValue = analogRead(lightSensorPin);

  buttonState = digitalRead(buttonPin);

  humidityPercentage = 1023 - humidityValue;
  humidityPercentage = humidityPercentage * 100;
  humidityPercentage = humidityPercentage / 1023;
  lightPercentage = lightValue * 100;
  lightPercentage = lightPercentage / 1023;

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
  if ((humidityValue > minimumHumidityDesired) && (waterBombActivationTimer <= 0) && (releaux == 0) && (minimumDelayBetweenBombActivationTimer <= 0)) {
    activateBomb();

    waterBombActivationTimer = waterBombActivationTimerDefault;
    minimumDelayBetweenBombActivationTimer = minimumDelayBetweenBombActivationTimerDefault;
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
   Updates the LCD every 5 seconds (100 50ms cycles)
*/
void lcdUpdate() {
  if (lcdUpdateTimer == 0) {
    double totalTime = cyclesAverage * 50;

    lcdUpdateTimer = lcdStandardTimer;
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
  double totalTime = cyclesAverage * 50;

  digitalWrite(relePin, HIGH);
  releaux = 1;
  calculateAverageTime();
  cyclesAverage = 0;
}

/**
   Calculates the average time between each automated bomb activation
*/
void calculateAverageTime() {
  timerAverageIndex = timerAverageIndex + 1;
  averageTimer = (averageTimer * timerAverageIndex + cyclesAverage * defaultDelay) / (timerAverageIndex + 1);
}

/**
   Manual mode
*/
void manualMode() {
  activateBomb();
  cyclesAverage = 0;
}

/**
   Calculates the average humidity value, in %
*/
void calculateAverageHumidity() {
  humidityAverageIndex = humidityAverageIndex + 1;
  humidityGlobalAverage = (humidityGlobalAverage * humidityAverageIndex + humidityPercentage) / (humidityAverageIndex + 1);
}


