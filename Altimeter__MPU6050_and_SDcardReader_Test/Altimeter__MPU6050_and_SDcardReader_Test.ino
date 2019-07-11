#include <SD.h>


#include <Wire.h>
#include "MPL3115A2.h"
#include <MPU6050_tockn.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
File dataFile;

MPL3115A2 myPressure;
MPU6050 mpu6050(Wire);

int led = 9;

void setup()
{
  Wire.begin();       
  //Serial.begin(9600);  
  
  pinMode(10, OUTPUT);
  SD.begin(10);
  
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  myPressure.begin(); // Get sensor online
  myPressure.setModeAltimeter(); 
  myPressure.setOversampleRate(7);
  myPressure.enableEventFlags(); 
}

void loop()
{
  String dataString = "";
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  dataFile.println(millis() + "milliseconds since program start.");
  dataFile.print(" Altitude(ft):");
  dataFile.println(myPressure.readAltitudeFt(), 2);

  mpu6050.update();
  dataFile.print("accX : "); dataFile.print(mpu6050.getAccX());
  dataFile.print("\taccY : "); dataFile.print(mpu6050.getAccY());
  dataFile.print("\taccZ : "); dataFile.println(mpu6050.getAccZ());
  
  dataFile.print("gyroX : "); dataFile.print(mpu6050.getGyroX());
  dataFile.print("\tgyroY : "); dataFile.print(mpu6050.getGyroY());
  dataFile.print("\tgyroZ : "); dataFile.println(mpu6050.getGyroZ());
  
  dataFile.print("accAngleX : "); dataFile.print(mpu6050.getAccAngleX());
  dataFile.print("\taccAngleY : "); dataFile.println(mpu6050.getAccAngleY());
  
  dataFile.print("gyroAngleX : "); dataFile.print(mpu6050.getGyroAngleX());
  dataFile.print("\tgyroAngleY : "); dataFile.print(mpu6050.getGyroAngleY());
  dataFile.print("\tgyroAngleZ : "); dataFile.println(mpu6050.getGyroAngleZ());
    
  dataFile.print("angleX : "); dataFile.print(mpu6050.getAngleX());
  dataFile.print("\tangleY : "); dataFile.print(mpu6050.getAngleY());
  dataFile.print("\tangleZ : "); dataFile.println(mpu6050.getAngleZ());
  dataFile.println("\n");

  dataFile.close();
}
