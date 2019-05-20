/* 
   This script defines the work of the client node (Arduino UNO) for motor control
   
   Role of the client node:
    1. This client node is connected to a gyroscope and a motor driver which
    controls the movement of the platform via 2 DC motors and the angle of the
    camera via a servo motor.
    
    2. The client UNO board only talks to the master node via I2C communication.
    It passively receives one-character command from the master and responds with
    behavior by powering specific motors.
     
    3. Further development will integrate the gyroscope to the board allowing the
    it to recognize the orientation and angle of the platform. Such features will
    finally serve to identify the direction and develop anti-shaking camera.
    
   Electronic module connected to the master node:
    1. Motor shield - 2 DC motor & 1 servo motor
    2. Adafruit absolute orientation sensor
*/

// core library for I2C communication
#include <Wire.h>
// library for motor shield (DC motor)
#include <AFMotor.h>
// library for the servo motor
#include <Servo.h>

// declare servo motor object
Servo myservo;

// declare 2 DC motor objects
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

// declare initial angle for the servo
int pos = 114;

// declare a global variable for the one-character command sent by master
char val;

// function for while receiving commands
void receiveEvent() {
  char cmd = Wire.read();

  // reset the angle of camera when the servo reaches maxima in both directions
  if (pos <= 30 || pos >= 210 ){
    pos = 114;
    myservo.write(pos);
    Serial.println("camera reset...");
  } else {
    if(cmd == 'd'){ // CROSS botton: move camera down
      pos += 20;
      myservo.write(pos);
      Serial.println("camera down...");
     } else if (cmd == 'w'){ // TRIANGLE botton: move camera up
      pos -= 20;
      myservo.write(pos);
      Serial.println("camera up...");
      } else {
        val = cmd; // commands other than controlling camera are sent to motor shield
        }
     }
}

// void setup(): client board initialization
void setup() {
  Wire.begin(9);  // join the I2C bus with an address = 9
  Wire.onReceive(receiveEvent); // register an event once receiving data from master
  Serial.begin(9600); // start serial output channel
  
  myservo.attach(9); // start servo motor pin at 9
  myservo.write(pos); // reset camera to initial angle (vertical)
  
  motor1.setSpeed(255); // initialize DC motor speed
  motor2.setSpeed(255); // initialize DC motor speed
}


void loop() {
  // UP_ARROR botton: move forward
  if(val == 'f'){
      //Serial.println("moving forward...");
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      }
        
  // DOWN_ARROW botton: move backward
  if(val == 'b'){
      Serial.println("moving backward...");
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      }
        
  // LEFT_ARROW botton: left turn
  if(val == 'l'){
      Serial.println("turning left...");
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
      delay(100);
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      }
    
  // RIGHT_ARROW botton: right turn
  if(val == 'r'){
      Serial.println("turning right...");
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
      delay(100);
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      }
  // SQUARE button: stop
    if(val == 's'){
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        }
    // release command variable when a movement is complete
    val = 0;
}
