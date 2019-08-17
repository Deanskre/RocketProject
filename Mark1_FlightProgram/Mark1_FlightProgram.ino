
#include <Wire.h>
#include "MPL3115A2.h"
#include <MPU6050_tockn.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
File dataFile;

MPL3115A2 myPressure;
MPU6050 mpu6050(Wire);

int buzzer = 4;
int parachute = 9;
double start;
boolean freefall = false;
//double alt;
//double prevAlt;
//int apogeeDetectCount = 0;
int freefallTime = 0;


void setup() {
  Wire.begin();       
  Serial.begin(9600);  
  pinMode(buzzer, OUTPUT);
  pinMode(parachute, OUTPUT);
  pinMode(10, OUTPUT);
  SD.begin(10);
  
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  myPressure.begin(); // Get sensor online
  myPressure.setModeAltimeter(); 
  myPressure.setOversampleRate(7);
  myPressure.enableEventFlags(); 

  dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println("Time\tAltitude\taccX\taccy\taccZ\tgyroX\tgyroY\tgyroZ\tgyroAngleX\tgyroAngleY\tgyroAngleZ");
  dataFile.close();
  //alt = myPressure.readAltitudeFt();
  
  Serial.println("Calibration complete.");
  digitalWrite(buzzer, HIGH);
  delay(600);
  digitalWrite(buzzer, LOW);
  
  start = millis();
}


void dataLog() {
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  double T = (millis()-start)/1000;
  dataFile.print(T);
  
  dataFile.print("\t"); dataFile.print(myPressure.readAltitudeFt(), 2);

  mpu6050.update();
  dataFile.print("\t"); dataFile.print(mpu6050.getAccX());
  dataFile.print("\t"); dataFile.print(mpu6050.getAccY());
  dataFile.print("\t"); dataFile.print(mpu6050.getAccZ());
  
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroX());
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroY());
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroZ());

  dataFile.print("\t"); dataFile.print(mpu6050.getGyroAngleX());
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroAngleY());
  dataFile.print("\t"); dataFile.println(mpu6050.getGyroAngleZ());
  
  dataFile.close();
}

/*void apogeeDetect() {
  prevAlt = alt;
  alt = myPressure.readAltitudeFt();
  if (alt <= prevAlt - 3) { 
    apogeeDetectCount++; 
  }
  if (apogeeDetectCount >= 3) { 
    digitalWrite(parachute, HIGH);
  }
}*/

void parachuteControl() {
  if (mpu6050.getAccX() < 0.25 && mpu6050.getAccY() < 0.25 && mpu6050.getAccZ() < 0.25) {
    freefall = true;
  }
  if (freefallTime != 0 && freefall) {
    freefallTime = millis();
  }
  if (freefall && millis() >= freefallTime + 3500) {
    digitalWrite(parachute, HIGH);
  }
}


void loop(){
  parachuteControl();
  dataLog();
}
