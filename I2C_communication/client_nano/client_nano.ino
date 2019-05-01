// Wire Slave Sender
// Convert numeric values (an integer & a double) to binary and send data to master node

#include <Wire.h>

// union to convert short integer to binary (1 byte)
typedef union
{
  int all;
  struct
  {
  unsigned char i1:8;
  }bit;
} INT;

// union to convert double float to binary (8 byte)
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
  int a = 19;
  float b = -74.365489;
  
  INT send_int;
  DOUB send_double;

  send_int.all = a;
  send_double.all = b;

  unsigned char a1 = 0;
  a1 = send_int.bit.i1;
  
  unsigned char b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0, b7 = 0, b8 = 0;
  b1 = send_double.bit.f1;
  b2 = send_double.bit.f2;
  b3 = send_double.bit.f3;
  b4 = send_double.bit.f4;
  b5 = send_double.bit.f5;
  b6 = send_double.bit.f6;
  b7 = send_double.bit.f7;
  b8 = send_double.bit.f8;

  Wire.write(a1);
  Wire.write(b1);
  Wire.write(b2);
  Wire.write(b3);
  Wire.write(b4);
  Wire.write(b5);
  Wire.write(b6);
  Wire.write(b7);
  Wire.write(b8);
}
