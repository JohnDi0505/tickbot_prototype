/* 
   This script defines the work of the master node (Arduino Nano)
   
   Role of the master node:
    1. Master node is connected to 2 client nodes via I2C communication:
    a NANO board works to collect data; a UNO board controls 2 DC motors
    and a servo motor.
    
    2. Master node actively requests data from the client NANO board
    every 5 seconds, and restores binary transmission to readable texts
    which are sent as serial output to mobile device via bluetooth and
    can be write to an SD card when the master node receives command.
     
    3. Master node sends one-character command to the client UNO board
    without any specified delays so that all motors are indirectly
    controlled by the master node seamlessly.

   Electronic module connected to the master node:
    1. Bluetooth module
    2. SD card read/write module
*/

// SimpleTimer.h library allows data I2C transmission to be independent from motor control
#include <SimpleTimer.h>

// core library for I2C communication
#include <Wire.h>

// library for SD card module
#include <SPI.h>
#include "SdFat.h"

// define global variable & objects
SdFat SD;

SimpleTimer timer;

// configure SD card output pin to pin #10
#define SD_CS_PIN 10
File myFile;

// define the structure called "data" for received data
struct data
  {
    unsigned char hour, minute, seconds, month, day, year, fix, fix_quality;
    float temp, rh, lat, lon, elv;
    unsigned char check_code;
  };

// sizeof() function helps to calculate the total length of data structure in byte
#define SIZE sizeof(data)

/* define a union called "sensor_data" to represent received data in this case, the 
   union "sensor_data" represents a same chunck of memory (where the received data
   is stored in the master node) in two different ways:
    1). "data instance": readable and accessable as the elements in the structure "data";
    2). "unsigned char transfer_char[SIZE]": unreadable but arranged in a binary array;*/

typedef union 
{
  data instance;
  unsigned char transfer_char[SIZE];
} sensor_data;

// declare an empty union
sensor_data r;

// global variable holds the value sent by the bluetooth module
char val;
// start writing data to SD card when writing_permit is 1; not writing when it is -1;
char writing_permit = -1;

// void setup(): master board initialization
void setup() {

  Wire.begin(); // start I2C bus
  Serial.begin(9600); // start serial output channel

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

  // create a file "data.txt" with a header to the SD card if it doesn't not exist
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

  // define the data refresh rate as every 5 seconds for serial output
  timer.setInterval(5000, Datareceiver1);
  
}

void loop() {
  // run SimpleTimer
  timer.run();

  // void loop() function keeps detecting serial input (bluetooth input)
  if(Serial.available()>0){
    val = Serial.read();
    /* CIRCLE button would send a single-character "t" to master board telling whether
       to start or stop writing data */
    if (val == 't') {
      writing_permit *= -1;
      } else {
        Datasender2();
        }
  }
}

// function to actively send data to the client UNO board
void Datasender2() {
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(val);              // sends one byte
  Wire.endTransmission();    // stop transmitting  
}

// function to request data from the client NANO board
void Datareceiver1() {

  Wire.requestFrom(8, SIZE);
  int i = 0; // initialize index for binary transmission
  unsigned char m = 0; // initialize binary variable for data verification
  while (Wire.available()) {
    r.transfer_char[i] = Wire.read();
    m += r.transfer_char[i];
    i++;
   }

  // print transmitted data to serial port
  // data transaction is complete when verification code = 0
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

  // output GPS data when the GPS module fix the location
  if (r.instance.fix >= 1) {
    Serial.print("latitude: ");Serial.print(r.instance.lat, 6);Serial.print(", ");
    Serial.print("longitude: ");Serial.print(r.instance.lon, 6);Serial.print(", ");
    Serial.print("altitude: ");Serial.println(r.instance.elv, 2);

    // write data to SD card when the state of writing_permit changes to 1
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
