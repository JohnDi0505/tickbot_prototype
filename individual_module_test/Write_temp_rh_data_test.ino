/*
Test script for SD card read/write module and the temperature & humidity sensor

Wiring the SD card module:
  VIN pin to +5V on the controller board;
  GND pin to GND on the controller board;
  MOSI pin to pin 11 on the controller board;
  MISO pin to pin 12 on the controller board;
  CLK pin to pin 13 on the controller board;
  CS pin to pin 10 on the controller board;

Wiring the temperature & humidity sensor:
  VIN pin to +5V on the controller board;
  GND pin to GND on the controller board;
  OUT pin to pin 4 on the controller board;

 Set serial output channel to 9600;
*/

#include <SimpleTimer.h>

#include <SPI.h>
#include <SD.h>

#include <DHT.h>
#include <DHT_U.h>

SimpleTimer timer;

File myFile;

#define DHT_datapin 4
#define DHT_type DHT22
DHT dht(DHT_datapin, DHT_type);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  timer.setInterval(10000, HT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  dht.begin();
}

void loop() {
  HT();
  delay(10000);
}

void HT(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(true);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print("temperature: ");
    myFile.print(t);
    myFile.print(" *F\trelative humidity: ");
    myFile.print(h);
    myFile.println(" %");
    
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print(" *F ");
  Serial.print("    Humidity = ");
  Serial.print(h);
  Serial.println(" % ");
}           
