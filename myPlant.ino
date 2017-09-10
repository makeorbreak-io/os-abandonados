#include <Wire.h>
#include <rgb_lcd.h>

/**
   Light sensor, connected to the Analog Pin #2,, ranging from 0(dark) to 1023
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
int waterBombActivationTimerDefault = 60; // 60 = 3s

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
int minimumDelayBetweenBombActivationTimerDefault = 12000; // 12000 = 10 min

/**
   Timer to update the LCD
*/
int lcdUpdateTimer = 10;

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
long cyclesAverage = 0;

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
long humidityAverageIndex = 0;

/**
   Standard time it takes for the lcd to update (in # of 50ms cycles)
*/
int lcdStandardTimer = 10; // 10 = 0.5 sec

/**
   Minimum humidity desired for the plant (310 = ~70%, 358 = ~65%)
*/
int minimumHumidityDesired = 358;

/**
  The current menu index
*/
int menuIndex = 0;

/*
   Number of cycles representing the countdown for the water bomb to activate
*/
int manualModeCurrentIndex = 0;

/*
   Number cycles representing the time it takes for the water bomb to activate
*/
int manualModeDefaultIndex = 100;

/**
   Declaring the lcd
*/
rgb_lcd lcd;

/**
   The background colours of the lcd
*/
int colorR;
int colorG;
int colorB;

void setup() {
  pinMode(relePin, OUTPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0);


  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("                 M");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("                My");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 13);
  lcd.print("               MyP");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("              MyPl");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("             MyPla");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("            MyPlan");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("           MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("          MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("         MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("        MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("       MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("      MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 9);
  lcd.print("     MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("    MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("   MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("  MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print(" MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("MyPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("yPlant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("Plant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("Lant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("ant");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("nt");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("t");
  delay(250);

  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("");
  delay(250);



  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("MyPlant");
  delay(1000);

}

void loop() {

  humidityValue = analogRead(humiditySensorPin);
  lightValue = analogRead(lightSensorPin);

  buttonState = digitalRead(buttonPin);

  humidityPercentage = 1023 - humidityValue;
  humidityPercentage = humidityPercentage * 100;
  humidityPercentage = humidityPercentage / 1023;

  if (buttonState == 0 && menuIndex != 4) {

    checkIfBombNeedsToBeActivated();

    deactivateBomb();

  } else if (buttonState == 1) { // On button press

    if (menuIndex < 3) {
      menuIndex = menuIndex + 1;
    } else if (menuIndex == 3) {
      menuIndex = menuIndex + 1;
      manualModeCurrentIndex = 0;
    } else {
      deactivateBomb();
      menuIndex = 0;
    }
    delay(500);
  }

  calculateAverageHumidity();

  if (menuIndex == 4) {
    manualModeCurrentIndex = manualModeCurrentIndex + 1;
  }

  lcdUpdate();

  delay(defaultDelay);

  timerUpdate();
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
   Updates the LCD every 0.5 seconds (10 50ms cycles)
*/
void lcdUpdate() {
  if (lcdUpdateTimer == 0) {

    if (menuIndex == 0) {
      lcdHumidityMenu();
    } else if (menuIndex == 1) {
      lcdLightMenu();
    } else if (menuIndex == 2) {
      lcdLastIrrigationMenu();
    } else if (menuIndex == 3) {
      lcdAverageHumidityMenu();
    } else if (menuIndex == 4) {
      lcdManualMode();
    }

    lcdUpdateTimer = lcdStandardTimer;

  }

}

/*
  LCD's Humidity menu
*/
void lcdHumidityMenu() {

  if (humidityPercentage > 65) {
    colorR = 0;
    colorG = 255;
    colorB = 0;
  } else {
    colorR = 255;
    colorG = 0;
    colorB = 0;
  }

  lcdMessageAndColorHumidity("Humidity:", 4, humidityPercentage);
}

/**
    Changes the lcd message and color according to the plants humidity percentage
*/
void lcdMessageAndColorHumidity(String aspect, int start, double percentage) {
  lcd.setRGB(colorR, colorG, colorB);
  lcd.clear();
  lcd.setCursor(start, 0);
  lcd.print(aspect);
  lcd.setCursor(5, 1);
  lcd.print(percentage);
  lcd.setCursor(12, 1);
  lcd.print("%");
}

/**
    Changes the lcd message and color according to the plants light percentage
*/
void lcdMessageAndColorLight(String aspect, int start, String state) {
  lcd.setRGB(colorR, colorG, colorB);
  lcd.clear();
  lcd.setCursor(start, 0);
  lcd.print(aspect);
  lcd.setCursor(5, 1);
  lcd.print(state);
}

/*
  LCD's light menu
*/
void lcdLightMenu() {
  String state;
  if (lightValue > 250) {
    colorR = 0;
    colorG = 255;
    colorB = 0;
    state = "Good";
  } else if (lightValue <= 250 && lightValue > 50) {
    colorR = 255;
    colorG = 255;
    colorB = 0;
    state = "Medium";
  } else if (lightValue <= 50) {
    colorR = 255;
    colorG = 0;
    colorB = 0;
    state = "Low";
  }
  Serial.println(lightValue);
  lcdMessageAndColorLight("Light:", 5, state);

}

/*
  LCD's time since last irrigation menu
*/
void lcdLastIrrigationMenu() {
  double totalTime = cyclesAverage * 0.050;
  lcd.setRGB(54, 81, 94);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Last Irrigation");
  lcd.setCursor(3, 1);
  lcd.print((int) totalTime / 60);
  lcd.setCursor(5, 1);
  lcd.print("min");
  lcd.setCursor(9, 1);
  lcd.print((int)totalTime % 60);
  lcd.setCursor(11, 1);
  lcd.print("sec");
}

/**
   Deactivates the water bomb if the 1.5 seconds timer reaches 0 and the bomb is activated
*/
void deactivateBomb() {
  if ((waterBombActivationTimer == 0) && (releaux == 1)) {
    digitalWrite(relePin, LOW);
    releaux = 0;
    minimumDelayBetweenBombActivationTimer = minimumDelayBetweenBombActivationTimerDefault - 1;
  }
}

/**
   Activates the water bomb
*/
void activateBomb() {
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
void lcdManualMode() {

  countDown();

  if (manualModeCurrentIndex >= manualModeDefaultIndex) {

    lcd.setRGB(100, 65, 0);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Manual Mode");
    lcd.setCursor(2, 1);
    lcd.print("Press to Stop");

    activateBomb();
    cyclesAverage = 0;
    waterBombActivationTimer = 0;
  }
}

/*
   Represents the countdown for the manual mode to activate
*/
void countDown() {
  if (manualModeCurrentIndex <= 20) {
    repaintManualModeCountDown(5);
  } else if (manualModeCurrentIndex <= 40) {
    repaintManualModeCountDown(4);
  } else if (manualModeCurrentIndex <= 60) {
    repaintManualModeCountDown(3);
  } else if (manualModeCurrentIndex <= 80) {
    repaintManualModeCountDown(2);
  } else {
    repaintManualModeCountDown(1);
  }
}

/*
   Repaints the lcd, representing the countdown for the manual mode to begin
*/
void repaintManualModeCountDown(int index) {
  lcd.setRGB(58, 44, 86);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Manual Mode");
  lcd.setCursor(8, 1);
  lcd.print(index);
}

/**
   Calculates the average humidity value, in %
*/
void calculateAverageHumidity() {
  humidityAverageIndex = humidityAverageIndex + 1;
  humidityGlobalAverage = (humidityGlobalAverage * humidityAverageIndex + humidityPercentage) / (humidityAverageIndex + 1);
}

/*
   Menu showing the average humidity in the plant
*/
void lcdAverageHumidityMenu() {
  lcd.setRGB(255, 182, 193);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Average Hum%");
  lcd.setCursor(5, 1);
  lcd.print(humidityGlobalAverage);
  lcd.setCursor(12, 1);
  lcd.print("%");
}

