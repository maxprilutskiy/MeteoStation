#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  

LiquidCrystal_I2C lcd(0x27,16,2); // Параметры: Адрес устройства, размер экрана  

#define rxPin 2
#define txPin 3

SoftwareSerial btSerial(rxPin, txPin);

bool isDisplayingMode = true;
 
void setup()
{
   lcd.init();
   lcd.backlight();
   // define pin modes for tx, rx pins:
   pinMode(rxPin, INPUT);
   pinMode(txPin, OUTPUT);
   btSerial.begin(38400); 
   Serial.begin(38400);
   Serial.println("Serial started");
   
           
   lcd.print("    Waiting");
   lcd.setCursor(0, 1);
   lcd.print(" for connection");
   
   btSerial.println("AT");
   while (!btSerial.available());
   while (btSerial.available() > 0)
        char c = btSerial.read();
   btSerial.println("AT+INIT");
   while (!btSerial.available());
   while (btSerial.available() > 0)
        char c = btSerial.read();
   btSerial.println("AT+INQ");
   while (!btSerial.available());
   while (btSerial.available() > 0)
        char c = btSerial.read();
   btSerial.println("AT+LINK=2014,5,191146");  
   while (!btSerial.available());
   while (btSerial.available() > 0)
        char c = btSerial.read();   
}

void loop()
{   
  int i = 0;
  char someChar[32] = {0};
  // when characters arrive over the serial port...
  bool availible = Serial.available();
  if(availible) {
    do{
        someChar[i++] = Serial.read();
      //As data trickles in from your serial port you are grabbing as much as you can, 
      //but then when it runs out (as it will after a few bytes because the processor 
      //is much faster than a 9600 baud device) you exit loop, which then restarts, 
      //and resets i to zero, and someChar to an empty array.So please be sure to keep this delay 
      delay(3);                  
 
    }while (Serial.available() > 0);

    lcd.clear();
    Serial.println(i);
    btSerial.println(someChar);
    Serial.println(someChar);
  }
 
  lcd.setCursor(0, 0);
  while(btSerial.available()) 
      {
        if (isDisplayingMode)
        {
          lcd.clear();
          isDisplayingMode = false;
        }
        char c = (char)btSerial.read();
        Serial.print(c);          
        if (c != 13 && c != 10)
          lcd.print(c);
        if (c == '\n')
          lcd.setCursor(0, 1);
      }
  isDisplayingMode = true;
}
