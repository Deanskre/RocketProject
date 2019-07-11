
#include <Wire.h>
#include "MPL3115A2.h"
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
File dataFile;

MPL3115A2 myPressure;

float start;

void setup()
{
  Wire.begin();       
  Serial.begin(9600);  
  
  pinMode(9, OUTPUT); //LED
  digitalWrite(9, HIGH);
  
  pinMode(10, OUTPUT);
  SD.begin(10);

  myPressure.begin(); // Get sensor online
  myPressure.setModeAltimeter(); 
  myPressure.setOversampleRate(7);
  myPressure.enableEventFlags(); 

  Serial.println("Calibration complete.");
  digitalWrite(9, LOW);

  dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println("Time (s)\tAltitude (ft)");
  dataFile.close();
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
