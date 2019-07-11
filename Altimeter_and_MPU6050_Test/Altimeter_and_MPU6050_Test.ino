#include <SD.h>


#include <Wire.h>
#include "MPL3115A2.h"
#include <MPU6050_tockn.h>


MPL3115A2 myPressure;
MPU6050 mpu6050(Wire);

int led = 9;

void setup()
{
 
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output
  
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  myPressure.begin(); // Get sensor online
  myPressure.setModeAltimeter(); 
  myPressure.setOversampleRate(7);
  myPressure.enableEventFlags(); 
}

void loop()
{

  float altitude = myPressure.readAltitudeFt();
  Serial.print(" Altitude(ft):");
  Serial.println(altitude, 2);

  mpu6050.update();
  /*Serial.print("accX : ");Serial.print(mpu6050.getAccX());
  Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
  Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
  
  Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
  Serial.print("\tgyroY : ");Serial.print(mpu6050.getGyroY());
  Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());
  
  Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
  Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
  
  Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
  Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
  Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());*/
    
  Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");Serial.println(mpu6050.getAngleZ());
}
