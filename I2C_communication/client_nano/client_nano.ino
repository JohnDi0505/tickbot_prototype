#include <Wire.h>

typedef union
{
  struct
  {
    int hour, min, second, month, date, year;
    double lat, lon, elv, temp, humidity;
  }bit;
  unsigned char arr[46];
} DATA;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);
}

void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  DATA b;
  b.bit.hour = 9;
  b.bit.min = 33;
  b.bit.second = 47;
  b.bit.month = 5;
  b.bit.date = 1;
  b.bit.year = 19;
  b.bit.lat = 47.198213;
  b.bit.lon = -71.128334;
  b.bit.elv = 19.8133452;
  b.bit.temp = 76.192342;
  b.bit.humidity = 21.4823422;
  
  Wire.write(b.arr, sizeof(b));
  
}
