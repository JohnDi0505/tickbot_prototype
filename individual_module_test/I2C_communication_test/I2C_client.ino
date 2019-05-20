/*
This script is to demostrate how data is packed, received, verified, and sent to a client
Arduino in binary format by a master Arduino controller board on I2C bus.

Principle of wiring I2C bus:
  1). All nodes must share a GND line
  2). All nodes must share SDA(or Analog 4), SCL(or Analog 5) lines
  3). All nodes can have common or their individul power supplies

Wiring in practice:
  GND pin on Board A to GND pin on Board B;
  SDA(or A4) pin on Board A to SDA(or A4) pin on Board B;
  SCL(or A5) pin on Board A to SCL(or A5) pin on Board B;

Important Notes:
  1). All client nodes passively respond to the master: client nodes will send data only when
  they are requested by the master; inversely, client nodes passively receive data only when
  the master node initiates a sending. Therefore, the frequency of data transaction only depends
  on how frequently the master node is requesting and sending data.
  2). Each I2C transaction will be temporarily stored in a small chunck of pre-allocated memory
  on the destination Arduino board. This chunck of memory, also called the "I2C buffer", is
  defined by a static global variable in two "head"(ends with .h) files located in the "Wire.h"
  library and essentially limits the size of the data per transaction (32 bytes in defaut). The
  size of I2C buffer can be modified by changing the buffer variable in both "Wire.h" & "twi.h"
  located in the "Wire" library to the same value simultaneously. Increasing I2C buffer size 
  is not suggested since it would occupy a considerable amount of dynamic memory. 
  3). Part of data that exceeds the buffer limit will not be sent to the receiver via I2C bus.
  It is critical to control the size based on data type and transmitting binary data is highly
  efficient.
*/

#include <Wire.h>

// define the structure called "data" for received data
struct data
  {
    unsigned char A, B, C;
    int D, E, F;
    float G, H, I;
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
} data_communication;

// declare an empty union "r" to receive data from client
data_communication r;

// declare a union "s" to send data to client
data_communication s;
s.instance.A = 'h';
s.instance.B = 8;
s.instance.C = 34;
s.instance.D = 25;
s.instance.E = 57;
s.instance.F = 125;
s.instance.G = 8.14;
s.instance.H = 0.234;
s.instance.I = 285.192;
// calculate data transmission verification code
unsigned char n = 0;
  for (int i=0; i < SIZE - 1; i++){n += s.transfer_char[i];}
  s.instance.check_code = -n;

void setup() {
  Wire.begin(8);
  Wire.onRequest(Datasender);
  Wire.onReceive(Datareceiver);
}

void loop() {}

void Datareceiver() {
  Wire.requestFrom(8, SIZE);
  int i = 0; // initialize index for binary transmission
  unsigned char m = 0; // initialize binary variable for data verification
  while (Wire.available()) {
    r.transfer_char[i] = Wire.read();
    m += r.transfer_char[i];
    i++;
   }
  
  Serial.print("receiving data from client: ");
  Serial.print("verification code = ");Serial.println(m);
  Serial.print("char A = ");Serial.println(r.instance.A);
  Serial.print("char B = ");Serial.println(r.instance.B);
  Serial.print("char C = ");Serial.println(r.instance.C);
  Serial.print("int D = ");Serial.println(r.instance.D);
  Serial.print("int E = ");Serial.println(r.instance.E);
  Serial.print("int F = ");Serial.println(r.instance.F);
  Serial.print("float G = ");Serial.println(r.instance.G);
  Serial.print("float H = ");Serial.println(r.instance.H);
  Serial.print("float I = ");Serial.println(r.instance.I);
  Serial.print("\n");
}

void Datasender() {
  Wire.beginTransmission(8);
  Wire.write(s.transfer_char, SIZE);
  Wire.endTransmission();
}
