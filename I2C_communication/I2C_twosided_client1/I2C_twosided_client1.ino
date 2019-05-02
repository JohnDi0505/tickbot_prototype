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

bool GPS_fix = false;

typedef union
{
  struct
  {
    int hour, minute, seconds, month, day, year, fix, fix_quality;
    double lat, lon, elv, temp, rh;
  } params;
  unsigned char arr[48];
} sensor_data;

sensor_data s;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent);
  
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  delay(1000);

  dht.begin();          // start serial for output

}

void loop() {
  readGPS();
  getData();
}

void requestEvent() {
  if (GPS_fix) { // send data when location is fixed
    Wire.write(s.arr, sizeof(s));
    } else { // send 1 byte character to master to suggest GPS is working to fix location
      Wire.write("n");
      }
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
  if(GPS.fix==1){

    // send GPS data once the location is fixed
    GPS_fix = true;

    // add GPS, temperature, and humidity data to union
    s.params.hour = GPS.hour;
    s.params.minute = GPS.minute;
    s.params.seconds = GPS.seconds;
    s.params.month = GPS.month;
    s.params.day = GPS.day;
    s.params.year = GPS.year;
    s.params.fix = (int)GPS.fix;
    s.params.fix_quality = (int)GPS.fixquality;
    s.params.lat = GPS.latitudeDegrees;
    s.params.lon = GPS.longitudeDegrees;
    s.params.elv = GPS.altitude;
    s.params.temp = dht.readTemperature(true);
    s.params.rh = dht.readHumidity();
        
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
    Serial.print("Temperature = ");
    Serial.print(s.params.temp);
    Serial.print(" *F ");
    Serial.print("    Humidity = ");
    Serial.print(s.params.rh);
    Serial.println(" % ");
    } else {
      Serial.println("Waiting for GPS to fix location...");
      }
 }
