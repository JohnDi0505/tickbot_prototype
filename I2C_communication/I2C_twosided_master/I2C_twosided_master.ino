#include <Wire.h>

#include <SPI.h>
#include "SdFat.h"
SdFat SD;

#define SD_CS_PIN 10
File myFile;

// global variable for bluetooth module
char val;
char movement;
int writing_permit = -1;

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);

  // initialize SD card
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // print file header
  myFile = SD.open("tick_trip_data.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing File Header...");
    myFile.println("time,date,lat,lon,elv,fix,fix_quality,temp(*F),rh(%)");
    // close the file:
    myFile.close();
  }
}

void loop() {

  if(Serial.available()>0){
    val = Serial.read();
    if (val == 't') { // circle to start/stop write data
      writing_permit *= -1;
      } else {
        movement = val;
        }
  }
  
  Datasender2();
  Datareceiver1();
  delay(1000);
}

void Datareceiver1() {
  Wire.requestFrom(8, 50);    // request 6 bytes from slave device #8
  String translation = "UTC ";
  while (Wire.available()) { // slave may send less than requested
    for(int i=1; i<51; i++) {
    char c = Wire.read(); // receive a byte as character
    if (i == 3) {
      translation += ":";
      }
    if (i == 5) {
      translation += ":";
      }
    if (i == 7) {
      translation += ",";
      }
    if (i == 9) {
      translation += "/";
      }
    if (i == 11) {
      translation += "/";
      }
    if (i == 13) {
      translation += ",";
      }
    if (i == 22) {
      translation += ",";
      }
    if (i == 32) {
      translation += ",";
      }
    if (i == 38) {
      translation += ",";
      }
    if (i == 39) {
      translation += ",";
      }
    if (i == 40) {
      translation += ",";
      }
    if (i == 45) {
      translation += ",";
      }
    translation += c;
    }
    Serial.print(translation);         // print the character
  }

  if (writing_permit == 1) {
    Serial.println("Writing data to SD Card...");
    myFile = SD.open("tick_trip_data.txt", FILE_WRITE);
    myFile.println(translation);
    myFile.close();
  }
}

void Datasender2() {
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(movement);              // sends one byte
  Wire.endTransmission();    // stop transmitting  
}
