#include <Wire.h>
#include <DHT.h>

#define DHT_datapin 2
#define DHT_type DHT22
DHT dht(DHT_datapin, DHT_type);

float j = 23.876324;

float t;
float h;

void setup() {
  Serial.begin(9600);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}            

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature(true);
  String tmp = String(t,2);
  String hum = String(h,2);
  String f = tmp + "\t" + hum + "\n";
  Serial.println(f);
  delay(1000);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  char th[13];
  String tmp = String(t,2);
  String hum = String(h,2);
  String f = tmp + "\t" + hum + "\n";
  f.toCharArray(th, 13);
  Wire.write(th);
}
