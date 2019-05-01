// Wire Master Reader
// Receive binary values from slave node and translate values to numeric (an integer & a double float)

#include <Wire.h>

// union for short integer (1 byte)
typedef union
{
  int all;
  struct
  {
  unsigned char i1:8;
  }bit;
} INT;

// union for double float (8 byte)
typedef union
{
    double all;
    struct
    {
    unsigned char f1:8;
    unsigned char f2:8;
    unsigned char f3:8;
    unsigned char f4:8;
    unsigned char f5:8;
    unsigned char f6:8;
    unsigned char f7:8;
    unsigned char f8:8;
    }bit;
} DOUB;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {
  Wire.requestFrom(8, 9);    // request 6 bytes from slave device #8
  byte data[9];
  int i = 0;
  while (Wire.available()) { // slave may send less than requested
    data[i] = Wire.read(); // receive a byte as character
    i++;
  }
  
  INT a;
  DOUB b;
  a.bit.i1 = data[0];
  b.bit.f1 = data[1];
  b.bit.f2 = data[2];
  b.bit.f3 = data[3];
  b.bit.f4 = data[4];
  b.bit.f5 = data[5];
  b.bit.f6 = data[6];
  b.bit.f7 = data[7];
  b.bit.f8 = data[8];
  
  Serial.print("receive integer: 20");
  Serial.print(a.all);
  Serial.print("\t");
  Serial.print("receive float: ");
  Serial.println(b.all, 6);
  delay(500);
}
