

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


int humid = 1; // pin 1 analogico
int rele = 1;  //pin 1 digital
int humitystate;
int aux = 0;
int button = 4;  //pin 4 digital
int buttonstate;
int releaux = 0;
int count = 0;
int count2 = 0;
double percent;

void setup() {
 pinMode(rele, OUTPUT);

}

void loop() {


humitystate = analogRead(humid);
buttonstate = digitalRead(button);
percent = (humitystate*100)/1023;

  if (buttonstate == 0){  


    if ((humitystate<717)&&(aux==0)&&(releaux==0)&&(count==0)){ // 70% 1024
        digitalWrite(rele,HIGH);
        releaux=1;
        aux=30;
        count = 1600;
      }

      

    
    if (aux!=0){
        aux=aux-1;
      }
  
    if (count!=0){
        count=count-1;
      }

    if (count2 > 100){ // atualizar valor mostrador

      LCD.begin(16,2); //resolução do mostrador
      //lcd.setCursor(0,0); //início
      
      //lcd.print("Humidade :)");
      
      
      
      
      
      
      
      
      
      count2=0;
     }

    count2 = count2 + 1;
    
    delay(50);


    
    
    if ((aux==0)&&(releaux==1)){
        digitalWrite(rele,LOW);
        releaux = 0;
      }

  
  } else {

    
    digitalWrite(rele,HIGH);
    releaux = 1;
  
  }

}


