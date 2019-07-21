
#include <Wire.h>
#include "MPL3115A2.h"
#include <MPU6050_tockn.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
File dataFile;

MPL3115A2 myPressure;
MPU6050 mpu6050(Wire);

float start;

void setup()
{
  Wire.begin();       
  Serial.begin(9600);  
  
  pinMode(9, OUTPUT); //LED
  digitalWrite(9, HIGH);
  
  pinMode(10, OUTPUT);
  SD.begin(10);
  
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  myPressure.begin(); // Get sensor online
  myPressure.setModeAltimeter(); 
  myPressure.setOversampleRate(7);
  myPressure.enableEventFlags(); 

  dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println("Time\tAltitude\taccX\taccy\taccZ\taccAngleX\taccAnglyeY\tgyroAngleX\tgyroAngleY\tgyroAngleZ");
  dataFile.close();
  
  Serial.println("Calibration complete.");
  digitalWrite(9, LOW);
  start = millis();
}

void loop()
{
  dataFile = SD.open("datalog.txt", FILE_WRITE);

  double T = (millis()-start)/1000;
  dataFile.print(T);
  
  dataFile.print("\t"); dataFile.print(myPressure.readAltitudeFt(), 2);

  mpu6050.update();
  dataFile.print("\t"); dataFile.print(mpu6050.getAccX());
  dataFile.print("\t"); dataFile.print(mpu6050.getAccY());
  dataFile.print("\t"); dataFile.print(mpu6050.getAccZ());

  dataFile.print("\t"); dataFile.print(mpu6050.getAccAngleX());
  dataFile.print("\t"); dataFile.print(mpu6050.getAccAngleY());
  
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroAngleX());
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroAngleY());
  dataFile.print("\t"); dataFile.println(mpu6050.getGyroAngleZ());

  dataFile.close();
}
