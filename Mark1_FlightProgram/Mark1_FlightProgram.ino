
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
double alt;
double maxAlt;
boolean apogee = false;

void setup() {
  // Arduino crap
  Wire.begin();       
  Serial.begin(9600);  
  pinMode(buzzer, OUTPUT);
  pinMode(parachute, OUTPUT);
  pinMode(10, OUTPUT);
  SD.begin(10);

  // mpu 6050 crap
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // altimeter crap
  myPressure.begin(); // Get sensor online
  myPressure.setModeAltimeter(); 
  myPressure.setOversampleRate(7);
  myPressure.enableEventFlags(); 

  // sd card crap
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println("Time\tAltitude\taccX\taccy\taccZ\tgyroX\tgyroY\tgyroZ\tgyroAngleX\tgyroAngleY\tgyroAngleZ"); // writing table header
  dataFile.close();
  
  alt = myPressure.readAltitudeFt();
  maxAlt = alt;

  // sound buzzer to indicate calibration completion
  Serial.println("Calibration complete.");
  digitalWrite(buzzer, HIGH);
  delay(600);
  digitalWrite(buzzer, LOW);
  
  start = millis();
}

// logs time and sensor values to SD card when called
void dataLog() {
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  double T = (millis()-start)/1000; // record T, the current time
  dataFile.print(T);
  
  dataFile.print("\t"); dataFile.print(alt, 2); // record altitude in feet

  dataFile.print("\t"); dataFile.print(mpu6050.getAccX()); // record accelerometer values (g m/s^2)
  dataFile.print("\t"); dataFile.print(mpu6050.getAccY());
  dataFile.print("\t"); dataFile.print(mpu6050.getAccZ());
  
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroX()); // record gyroscope values (deg/s)
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroY());
  dataFile.print("\t"); dataFile.print(mpu6050.getGyroZ());
  
  dataFile.close();
}

// checks for if the rocket has reached apogee
void apogeeCheck() {
  if (alt >= maxAlt) { 
    maxAlt = alt; 
  }
  if (alt < maxAlt - 5.5) { // if the current altitude is significantly less than the max altitude
    digitalWrite(parachute, HIGH);  // deploy parachute
    apogee = true;
  }
}


void loop(){
  alt = myPressure.readAltitudeFt();
  mpu6050.update();
  dataLog();
  if (!apogee) {
    apogeeCheck();
  }
}
