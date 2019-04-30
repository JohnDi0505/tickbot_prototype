// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

char val;

void setup() {
  Wire.begin(9);                // join i2c bus with address #9
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  
  motor1.setSpeed(255);
  motor2.setSpeed(245);
}

void loop() {
  
    // UP_ARROR: move forward for 1sec
    if(val == 'f'){
        Serial.println("moving forward...");
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        delay(100);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
        
    // DOWN_ARROW: move backward for 1sec
    if(val == 'b'){
        Serial.println("moving backward...");
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        delay(100);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
        
    // LEFT_ARROW: left turn for 0.1sec
    if(val == 'l'){
        Serial.println("turning left...");
        motor1.run(BACKWARD);
        motor2.run(FORWARD);
        delay(100);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
    
    // RIGHT_ARROW: right turn for 0.1sec
    if(val == 'r'){
        Serial.println("turning right...");
        motor1.run(FORWARD);
        motor2.run(BACKWARD);
        delay(100);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
     
    // TRIANGLE BUTTON: keep moving forward
    if(val == 'w'){
        Serial.println("keep moving forward...");
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        }
    
    // CROSS BUTTON: keep moving backward
    if(val == 'd'){
        Serial.println("keep moving backward...");
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        }
    
    // SQUARE BUTTON: stop
    if(val == 's'){
        Serial.println("stop...");
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
  }

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  char c = Wire.read();    // receive byte as an integer
  val = c;
}
