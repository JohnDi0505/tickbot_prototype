#include <SimpleTimer.h>

#include <SD.h>
#include <SPI.h>

#include <DHT.h>
#include <DHT_U.h>

#include <AFMotor.h>

int CS_PIN = 10;

SimpleTimer timer;

File myfile;

#define DHT_datapin 2
#define DHT_type DHT22
DHT dht(DHT_datapin, DHT_type);

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

// define variables
char val;

void setup() {
  Serial.begin(9600);
  timer.setInterval(10000, HT);
  //pinMode(CS_PIN, OUTPUT);
  /*if (SD.begin()) {
     Serial.println("SD card is ready to use.");
    } else {
     Serial.println("SD card initialization failed.");
     return;
    }

  myfile = SD.open("test.txt", FILE_WRITE);
  if (myfile) {
    Serial.println("Writing to file...");
    Serial.println("Done.");
    } else{
      Serial.println("error opening test.txt");
    }
  myfile.close(); */

  // Read file
 /* myfile = SD.open("test.txt");
  if(myfile){
    Serial.println("Read: ");
    while (myfile.available()) {
      Serial.write(myfile.read());
      }
      myfile.close();
    }
  Serial.println(val); */
  motor1.setSpeed(255);
  motor2.setSpeed(245);
  dht.begin();
  //SD.begin();
}

void HT(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(true);
  /*myfile = SD.open("test.txt", FILE_WRITE);
  myfile.print(h);
  myfile.print("   ");
  myfile.println(t);
  myfile.close(); */
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print(" *F ");
  Serial.print("    Humidity = ");
  Serial.print(h);
  Serial.println(" % ");
}                                                 

void loop() {

  timer.run();
  
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
