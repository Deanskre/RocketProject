
#include <Wire.h>
#include "MPL3115A2.h"
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
File dataFile;

MPL3115A2 myPressure;

int buzzer = 4;

float start;

void setup()
{
  Wire.begin();       
  Serial.begin(9600);  
  
  pinMode(buzzer, OUTPUT); 
  
  pinMode(10, OUTPUT);
  SD.begin(10);

  myPressure.begin(); // Get sensor online
  myPressure.setModeAltimeter(); 
  myPressure.setOversampleRate(7);
  myPressure.enableEventFlags(); 

  Serial.println("Calibration complete.");

  dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println("Time (s)\tAltitude (ft)");
  dataFile.close();
  digitalWrite(buzzer, HIGH);
  delay(600);
  digitalWrite(buzzer, LOW);
  start = millis();
}

void loop()
{
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  double T = (millis()-start)/1000;
  dataFile.print(T); 
  dataFile.print("\t");
  dataFile.println(myPressure.readAltitudeFt(), 2);
  dataFile.close();
}
