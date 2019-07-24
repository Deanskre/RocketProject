
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
File dataFile;

MPU6050 mpu6050(Wire);

int buzzer = 4;

float start;

void setup()
{
  Wire.begin();       
  Serial.begin(9600);  
  
  pinMode(buzzer, OUTPUT); //buzzer
  
  
  pinMode(10, OUTPUT);
  SD.begin(10);
  
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  Serial.println("Calibration complete.");
  digitalWrite(9, LOW);

  dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println("Time\tgyroX\tgyroY\tgyroZ\tgyroAngleX\tgyroAngleY\tgyroAngleZ\tangleX\tangleY\tangleZ");
  dataFile.close();
  
  digitalWrite(buzzer, HIGH);
  delay(600);
  digitalWrite(buzzer, LOW);
  start = millis();
}

void loop()
{
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  mpu6050.update();
  double T = (millis()-start)/1000;
  
  dataFile.print(T);
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroX());
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroY());
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroZ());
  
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroAngleX());
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroAngleY());
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroAngleZ());

  dataFile.print("\t"); dataFile.print(mpu6050.getAngleX());
  dataFile.print("\t"); dataFile.print(mpu6050.getAngleY());
  dataFile.print("\t"); dataFile.println(mpu6050.getAngleZ());
  
  dataFile.close();
}
