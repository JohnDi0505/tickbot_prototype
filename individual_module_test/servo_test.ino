/*
Test script for the Adafruit GPS module
Wiring the module:
  VIN pin to 6V ~ 7V power supply;
  GND pin to GND on the controller board;
          to GND on the power supply;
  signal pin to pin 9 on the controller board;

Set serial output channel to 115200;
*/

#include <Servo.h>

Servo myservo; 

int pos = 114;
char val;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(pos);
}

void loop() {
  
 if (pos <= 30 || pos >= 210 ){
    pos = 114;
    myservo.write(pos);
  } else {
    if (Serial.available()>0){
        val = Serial.read();
        if(val == 'd'){
          pos += 20;
          myservo.write(pos);
          } else if (val == 'u'){
            pos -= 20;
            myservo.write(pos);
            }
      }
  }
}
