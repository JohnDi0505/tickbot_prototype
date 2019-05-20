/*
Test script for the Adafruit GPS module

Wiring the module:
  VIN pin to +5V on the controller board;
  GND pin to GND on the controller board;
  RX pin to D2 on the controller board;
  TX pin to D3 on the controller board;

Set serial output channel to 115200;
*/

#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
SoftwareSerial mySerial(3,2);
Adafruit_GPS GPS(&mySerial);

void setup() {
  
  Serial.begin(115200);
  GPS.begin(9600);
  //GPS.sendCommand("$PGCMD,33,0*6D");
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  delay(1000);

}

uint32_t timer = millis();

void loop() {

  readGPS();
  getGPS();

}

void readGPS() {
  String NMEA1;
  String NMEA2;
  char c;

  while(!GPS.newNMEAreceived()){
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
  NMEA1=GPS.lastNMEA();

  while(!GPS.newNMEAreceived()){
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
  NMEA2=GPS.lastNMEA(); 
}

void getGPS() {
      if(GPS.fix==1){
        Serial.print("\nTime: "); Serial.print(GPS.hour); Serial.print(':');
        Serial.print(GPS.minute); Serial.print(':');
        Serial.print(GPS.seconds); Serial.print(',');Serial.print('\n');
        Serial.print("Date: "); Serial.print(GPS.month, DEC); Serial.print('/');
        Serial.print(GPS.day, DEC); Serial.print("/20");
        Serial.print(GPS.year, DEC); Serial.print(','); Serial.print('\n');
        Serial.print("Fix: "); Serial.print((int)GPS.fix);
        Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
        Serial.print("Latitude: "); Serial.print(GPS.latitudeDegrees, 6); Serial.print(','); Serial.print(' ');
        //Serial.print(GPS.lat); Serial.print(', ');
        Serial.print("Longitude: "); Serial.print(GPS.longitudeDegrees, 6); Serial.print(','); Serial.print(' ');
        //Serial.print(GPS.lon); Serial.print(', ');
        Serial.print("Altitude: "); Serial.println(GPS.altitude); //Serial.print('\n');
        } else {
          Serial.println("Waiting for GPS to fix location...");
          }
  }
