#include <Wire.h>

#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
SoftwareSerial mySerial(3,2);
Adafruit_GPS GPS(&mySerial);

#include <DHT.h>
#include <DHT_U.h>

#define DHT_datapin 4
#define DHT_type DHT22
DHT dht(DHT_datapin, DHT_type);

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

sensor_data s;

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  delay(1000);

  dht.begin();

}

void loop() {
  readGPS();
  getData();
  Serial.println(SIZE);
}

void requestEvent() {
    Wire.write(s.transfer_char, SIZE);
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

void getData() {
  
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
  
  unsigned char n = 0;
  for (int i=0; i < SIZE - 1; i++){n += s.transfer_char[i];}
  s.instance.check_code = -n;
 }
