#include <Wire.h>
#include <AFMotor.h>
#include <Servo.h>

Servo myservo;

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

int pos = 114;

void receiveEvent() {
  char val = Wire.read();

  if (pos <= 30 || pos >= 210 ){
    pos = 114;
    myservo.write(pos);
    Serial.println("camera reset...");
  } else {
    if(val == 'd'){ // CROSS: camera down
      pos += 20;
      myservo.write(pos);
      Serial.println("camera down...");
     } else if (val == 'w'){ // TRIANGLE: camera up
      pos -= 20;
      myservo.write(pos);
      Serial.println("camera up...");
      }
     }

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
}

void setup() {
  Wire.begin(9);
  Wire.onReceive(receiveEvent); 
  Serial.begin(9600);
  
  myservo.attach(9);
  myservo.write(pos);
  
  motor1.setSpeed(255);
  motor2.setSpeed(255);
}

void loop() {

}
