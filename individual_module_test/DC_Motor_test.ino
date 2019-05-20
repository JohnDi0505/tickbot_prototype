/*
Test script for the motor shield

Wiring the module:
  VIN pin to VIN on the controller board;
  +5V pin to +5V on the controller board;
  GND pin to GND on the controller board;
  D4 pin to D4 on the controller board;
  D7 pin to D7 on the controller board;
  D8 pin to D8 on the controller board;
  D12 pin to D12 on the controller board;
  
Set serial output channel to 9600;
*/

#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

char val;

void setup() {
  
  Serial.begin(9600);
  
  motor1.setSpeed(255);
  motor2.setSpeed(255);
}                                     

void loop() {
  
  if(Serial.available()>0){
    val = Serial.read();
    // UP_ARROR: move forward for 1sec
    if(val == 'f'){
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        delay(100);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
    // DOWN_ARROW: move backward for 1sec
    if(val == 'b'){
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        delay(100);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
    // LEFT_ARROW: left turn for 0.1sec
    if(val == 'l'){
        motor1.run(BACKWARD);
        motor2.run(FORWARD);
        delay(100);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
    // RIGHT_ARROW: right turn for 0.1sec
    if(val == 'r'){
        motor1.run(FORWARD);
        motor2.run(BACKWARD);
        delay(100);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
    // TRIANGLE BUTTON: keep moving forward
    if(val == 'w'){
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        }
    // CROSS BUTTON: keep moving backward
    if(val == 'd'){
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        }
    // CIRCLE BUTTON: dancing
    if(val == 't'){
        motor1.run(BACKWARD);
        motor2.run(FORWARD);
        delay(3000);
        motor1.run(FORWARD);
        motor2.run(BACKWARD);
        delay(3000);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
    // SQUARE BUTTON: stop
    if(val == 's'){
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
  }
}
