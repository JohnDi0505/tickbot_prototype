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
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {
  DATA receive;
  Wire.requestFrom(8, sizeof(receive));    // request 6 bytes from slave device #8
  int i = 0;
  while (Wire.available()) { // slave may send less than requested
    receive.arr[i] = Wire.read(); // receive a byte as character
    i++;
  }
  
  Serial.print("hour = ");Serial.println(receive.bit.hour);
  Serial.print("min = ");Serial.println(receive.bit.min);
  Serial.print("second = ");Serial.println(receive.bit.second);
  Serial.print("month = ");Serial.println(receive.bit.month);
  Serial.print("date = ");Serial.println(receive.bit.date);
  Serial.print("year = ");Serial.println(receive.bit.year);
  Serial.print("latitude = ");Serial.println(receive.bit.lat, 6);
  Serial.print("longitude = ");Serial.println(receive.bit.lon, 6);
  Serial.print("altitude = ");Serial.println(receive.bit.elv, 2);
  Serial.print("temperature = ");Serial.println(receive.bit.temp, 2);
  Serial.print("humidity = ");Serial.println(receive.bit.humidity, 2);
  Serial.println("\n");
  
  delay(1000);
}
