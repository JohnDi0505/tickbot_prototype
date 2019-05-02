#include <Wire.h>

#include <SPI.h>
#include "SdFat.h"
SdFat SD;

#define SD_CS_PIN 10
File myFile;

typedef union
{
  struct
  {
    int hour, minute, seconds, month, day, year, fix, fix_quality;
    double lat, lon, elv, temp, rh;
  } params;
  unsigned char arr[48];
} sensor_data;

// global variable for bluetooth module
char val;
char movement;
int writing_permit = -1;

// declare an empty union for sensor_data
sensor_data r;

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
    if (val == 't') { // circle to start/stop writing data
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
  Wire.requestFrom(8, sizeof(r));    // request 6 bytes from slave device #8
  int i = 0;
  while (Wire.available()) { // slave may send less than requested
    r.arr[i] = Wire.read(); // receive a byte as character
    i++;
  }
  
  Serial.print("time: UTC ");Serial.print(r.params.hour);Serial.print(":");
  Serial.print(r.params.minute);Serial.print(":");
  Serial.println(r.params.seconds);
  Serial.print("date: ");Serial.print(r.params.month);Serial.print("/");
  Serial.print(r.params.day);Serial.print("/");
  Serial.println(r.params.year);
  Serial.print("latitude: ");Serial.print(r.params.lat, 6);Serial.print(", ");
  Serial.print("longitude: ");Serial.print(r.params.lon, 6);Serial.print(", ");
  Serial.print("altitude: ");Serial.println(r.params.elv, 2);
  Serial.print("fix: ");Serial.print(r.params.fix);Serial.print(", ");
  Serial.print("fix quality: ");Serial.println(r.params.fix_quality);
  Serial.print("temperature: ");Serial.print(r.params.temp, 2);Serial.print(" *F, ");
  Serial.print("relative humidity: ");Serial.print(r.params.rh, 2);Serial.println(" %\n");

  if (writing_permit == 1) {
    Serial.println("Writing data to SD Card...");
    myFile = SD.open("tick_trip_data.txt", FILE_WRITE);
    
    myFile.print("UTC ");myFile.print(r.params.hour);myFile.print(":");
    myFile.print(r.params.minute);myFile.print(":");
    myFile.print(r.params.seconds);myFile.print(",");
    myFile.print(r.params.month);myFile.print("/");
    myFile.print(r.params.day);myFile.print("/");
    myFile.print(r.params.year);myFile.print(",");
    myFile.print(r.params.lat, 6);myFile.print(",");
    myFile.print(r.params.lon, 6);myFile.print(",");
    myFile.print(r.params.elv, 2);myFile.print(",");
    myFile.print(r.params.fix);myFile.print(",");
    myFile.print(r.params.fix_quality);myFile.print(",");
    myFile.print(r.params.temp, 2);myFile.print(",");
    myFile.println(r.params.rh, 2);
      
    myFile.close();
  }
}

void Datasender2() {
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(movement);              // sends one byte
  Wire.endTransmission();    // stop transmitting  
}
