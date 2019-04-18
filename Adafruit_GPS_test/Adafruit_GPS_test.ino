#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
SoftwareSerial mySerial(3,2);
Adafruit_GPS GPS(&mySerial);

String NMEA1;
String NMEA2;
char c;

void setup() {
  
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand("$PGCMD,33,0*6D");
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  delay(1000);
  
}

void loop() {

  readGPS();
  if(GPS.fix==1){
    Serial.print("Time: "); Serial.print(GPS.hour); Serial.print(':');
    Serial.print(GPS.minute); Serial.print(':');
    Serial.print(GPS.seconds); Serial.print(',');Serial.print('\n');
    Serial.print("Date: "); Serial.print(GPS.month); Serial.print('/');
    Serial.print(GPS.day); Serial.print('/');
    Serial.print(GPS.year); Serial.print(','); Serial.print('\n');
    Serial.print("Latitude: "); Serial.print(GPS.latitudeDegrees, 4); Serial.print(','); Serial.print(' ');
    //Serial.print(GPS.lat); Serial.print(', ');
    Serial.print("Longitude: "); Serial.print(GPS.longitudeDegrees, 4); Serial.print(','); Serial.print(' ');
    //Serial.print(GPS.lon); Serial.print(', ');
    Serial.print("Altitude: "); Serial.println(GPS.altitude); Serial.print('\n');
    } else {
      Serial.println("Waiting for GPS to fix location...");
      }
}

void readGPS() {
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
  
