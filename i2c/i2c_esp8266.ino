/*
 *  Simple HTTP get webclient test
 */


#include <Wire.h>                       // Wire Library for i2c Teensy(Master) to communicate with the ESP8266 (Slave)

byte x = 0;  //i2c counter
void setup() {
  // Setup serial debug port
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP8266 Serial Port");
  // Setup i2c
  Wire.pins(4,5);
  Wire.begin(6);          //join i2c bus There is an issue where ESP only works as Master
}

int value = 0;

void loop() {

  //Test sending i2c data
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  x++;
  delay(500);
  Serial.print("x is ");
  Serial.println(x);
}
