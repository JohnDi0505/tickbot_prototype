#include <SimpleTimer.h>

#include <Wire.h>

#include <SPI.h>
#include "SdFat.h"

SdFat SD;

SimpleTimer timer;

#define SD_CS_PIN 10
File myFile;

struct data
  {
    unsigned char hour, minute, seconds, month, day, year, fix, fix_quality;
    float temp, rh, lat, lon, elv;
    unsigned char check_code;
  };

#define SIZE sizeof(data)

typedef union 
{
  data instance;
  unsigned char transfer_char[SIZE];
} sensor_data;

sensor_data r;

char val;
char writing_permit = -1;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // initialize SD card
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (!SD.exists("data.txt")){
    // print file header
    myFile = SD.open("data.txt", FILE_WRITE);
    // if the file opened okay, write to it:
    if (myFile) {
      Serial.println("Writing File Header...");
      myFile.println("time,date,lat,lon,elv,fix,fix_quality,temp(*F),rh(%)");
      // close the file:
      myFile.close();
      }
    }
  
  timer.setInterval(5000, Datareceiver1);
  
}

void loop() {
  
  timer.run();
  
  if(Serial.available()>0){
    val = Serial.read();
    if (val == 't') { // circle to start/stop writing data
      writing_permit *= -1;
      } else {
        Datasender2();
        }
  }
  delay(100);
}

void Datasender2() {
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(val);              // sends one byte
  Wire.endTransmission();    // stop transmitting  
}

void Datareceiver1() {

  Wire.requestFrom(8, SIZE);
  int i = 0;
  unsigned char m = 0;
  while (Wire.available()) {
    r.transfer_char[i] = Wire.read();
    m += r.transfer_char[i];
    i++;
   }

  
  Serial.print("data transaction verification code: ");Serial.println(m);
  Serial.print("time: UTC ");Serial.print(r.instance.hour);Serial.print(":");
  Serial.print(r.instance.minute);Serial.print(":");
  Serial.println(r.instance.seconds);
  Serial.print("date: ");Serial.print(r.instance.month);Serial.print("/");
  Serial.print(r.instance.day);Serial.print("/20");
  Serial.println(r.instance.year);
  Serial.print("fix: "); Serial.print(r.instance.fix);Serial.print(", ");
  Serial.print("fix quality: "); Serial.println(r.instance.fix_quality);
  Serial.print("temperature: ");Serial.print(r.instance.temp, 2);Serial.print(" *F, ");
  Serial.print("relative humidity: ");Serial.print(r.instance.rh, 2);Serial.println(" %");

  if (r.instance.fix >= 1) {
    Serial.print("latitude: ");Serial.print(r.instance.lat, 6);Serial.print(", ");
    Serial.print("longitude: ");Serial.print(r.instance.lon, 6);Serial.print(", ");
    Serial.print("altitude: ");Serial.println(r.instance.elv, 2);

    if (writing_permit == 1) {
    Serial.println("Writing data to SD Card...");
    myFile = SD.open("data.txt", FILE_WRITE);
    
    myFile.print("UTC ");myFile.print(r.instance.hour);myFile.print(":");
    myFile.print(r.instance.minute);myFile.print(":");
    myFile.print(r.instance.seconds);myFile.print(",");
    myFile.print(r.instance.month);myFile.print("/");
    myFile.print(r.instance.day);myFile.print("/");
    myFile.print(r.instance.year);myFile.print(",");
    myFile.print(r.instance.lat, 6);myFile.print(",");
    myFile.print(r.instance.lon, 6);myFile.print(",");
    myFile.print(r.instance.elv, 2);myFile.print(",");
    myFile.print(r.instance.fix);myFile.print(",");
    myFile.print(r.instance.fix_quality);myFile.print(",");
    myFile.print(r.instance.temp, 2);myFile.print(",");
    myFile.println(r.instance.rh, 2);
      
    myFile.close();
    }
  } else {
    Serial.println("Waiting for GPS to fix location...");
    }
  Serial.print("\n");
 }
