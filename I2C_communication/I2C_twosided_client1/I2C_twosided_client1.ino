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

char message[51];

void loop() {
  readGPS();
  getData();
}

void requestEvent() {
  Wire.write(message); // respond with message of 6 bytes
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
      String expression;
      // send time: UTC hour:minute:second (14 byte including ", ")
      if(GPS.fix==1){
        //String expression;
        if(GPS.hour<10){
          expression += "0" + String(GPS.hour);
          } else {
            expression += String(GPS.hour);
            }
        if(GPS.minute<10){
          expression += "0" + String(GPS.minute);
          } else {
            expression += String(GPS.minute);
            }
        if(GPS.seconds<10){
          expression += "0" + String(GPS.seconds);
          } else {
            expression += String(GPS.seconds);
            }

        // send date: month/day/year (12 byte including ", ")
        if(GPS.month<10){
          expression += "0" + String(GPS.month);
          } else {
            expression += String(GPS.month);
            }
        if(GPS.day<10){
          expression += "0" + String(GPS.day);
          } else {
            expression += String(GPS.day);
            }
        expression += String(GPS.year);

        // send GPS data: Latitude, Longitude, Altitude (10+2+10+2+6+2=32)
        expression += String(GPS.latitudeDegrees, 6);
        expression += String(GPS.longitudeDegrees, 6);
        expression += String(GPS.altitude, 2);
        expression += String((int)GPS.fix);
        expression += String((int)GPS.fixquality);

        // send temperature humidity data
        float hmd = dht.readHumidity();
        float tmp = dht.readTemperature(true);

        // send temperature humidity data
        expression += String(tmp);
        expression += String(hmd);

        int num;
        expression += "\n";
        expression.toCharArray(message, 51); // total characters 26 + 1
        num = strlen(message);
        Serial.println(num);
        Serial.println(message);
        
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
        Serial.print(tmp);
        Serial.print(" *F ");
        Serial.print("    Humidity = ");
        Serial.print(hmd);
        Serial.println(" % ");
        } else {
          Serial.println("Waiting for GPS to fix location...");
          }
  }

void HT(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(true);
  
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print(" *F ");
  Serial.print("    Humidity = ");
  Serial.print(h);
  Serial.println(" % ");
}
