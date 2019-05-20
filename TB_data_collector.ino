/* 
   This script defines the work of the client node (Arduino NANO) for data collection
   
   Role of the client NANO board:
    1. The client NANO board works to collect data from a temperature & humidity
    sensor and a GPS module.
    
    2. It packs data using a structure called "data" (a mutually identical
    communication protocol of the master node) and passively sends data to 
    the master node in binary format only when data transmission is requested.
    
   Electronic module connected to the master node:
    1. Adafruit GPS module
    2. temperature & humidity sensor
*/

// core library for I2C communication
#include <Wire.h>

// libraries & global variables for the GPS module
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
SoftwareSerial mySerial(3,2);
Adafruit_GPS GPS(&mySerial);

// libraries & global variables for the temperature & humidity sensor
#include <DHT.h>
#include <DHT_U.h>
#define DHT_datapin 4
#define DHT_type DHT22
DHT dht(DHT_datapin, DHT_type);

// define the structure called "data" for received data
struct data
  {
    unsigned char hour, minute, seconds, month, day, year, fix, fix_quality;
    float temp, rh, lat, lon, elv;
    unsigned char check_code;
  };
  
// sizeof() function helps to calculate the total length of data structure in byte
#define SIZE sizeof(data)

/* 
   define a union called "sensor_data" to represent received data in this case, the 
   union "sensor_data" represents a same chunck of memory (where the received data
   is stored in the master node) in two different ways:
    1). "data instance": readable and accessable as the elements in the structure "data";
    2). "unsigned char transfer_char[SIZE]": unreadable but arranged in a binary array;
*/

typedef union 
{
  data instance;
  unsigned char transfer_char[SIZE];
} sensor_data;

// declare an empty union
sensor_data s;

// void setup(): client board initialization
void setup() {
  Wire.begin(8); // join the I2C bus with an address = 8
  Wire.onRequest(requestEvent); // register an event once requested by the master

  Serial.begin(115200); // start serial output channel
  GPS.begin(9600); // GPS module starts at channel 9600
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  delay(1000);

  dht.begin(); // start temperature & humidity sensor

}

// void loop() function allows to keep running modules and receiving data
void loop() {
  readGPS();
  getData();
  Serial.println(SIZE);
}

// function to be executed as registered event
void requestEvent() {
    Wire.write(s.transfer_char, SIZE);
}

// function to run the GPS module
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

// retrieve and save data to the structure
void getData() {
  Serial.print("\nTime: "); Serial.print(GPS.hour); Serial.print(':');
  Serial.print(GPS.minute); Serial.print(':');
  Serial.print(GPS.seconds); Serial.print(',');Serial.print('\n');
  Serial.print("Date: "); Serial.print(GPS.month, DEC); Serial.print('/');
  Serial.print(GPS.day, DEC); Serial.print("/20");
  Serial.print(GPS.year, DEC); Serial.print(','); Serial.print('\n');
  Serial.print("Fix: "); Serial.print((int)GPS.fix);
  Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
  Serial.print("Temperature = ");
  Serial.print(dht.readTemperature(true));
  Serial.print(" *F ");
  Serial.print("    Humidity = ");
  Serial.print(dht.readHumidity());
  Serial.println(" % ");
  Serial.print("Latitude: "); Serial.print(GPS.latitudeDegrees, 6); Serial.print(','); Serial.print(' ');
    //Serial.print(GPS.lat); Serial.print(', ');
  Serial.print("Longitude: "); Serial.print(GPS.longitudeDegrees, 6); Serial.print(','); Serial.print(' ');
    //Serial.print(GPS.lon); Serial.print(', ');
  Serial.print("Altitude: "); Serial.println(GPS.altitude); //Serial.print('\n');
  
  s.instance.hour = GPS.hour;
  s.instance.minute = GPS.minute;
  s.instance.seconds = GPS.seconds;
  s.instance.month = GPS.month;
  s.instance.day = GPS.day;
  s.instance.year = GPS.year;
  s.instance.fix = (int)GPS.fix;
  s.instance.fix_quality = (int)GPS.fixquality;
  s.instance.temp = dht.readTemperature(true);
  s.instance.rh = dht.readHumidity();
  s.instance.lat = GPS.latitudeDegrees;
  s.instance.lon = GPS.longitudeDegrees;
  s.instance.elv = GPS.altitude;

  // calculate transaction verification code
  unsigned char n = 0;
  for (int i=0; i < SIZE - 1; i++){n += s.transfer_char[i];}
  s.instance.check_code = -n;
 }
