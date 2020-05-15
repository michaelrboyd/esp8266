/*
 *  Simple HTTP get webclient test
 */
 #include <Wire.h>
 #include <PolledTimeout.h>

 #define SDA_PIN 4
 #define SCL_PIN 5
 const int16_t I2C_MASTER = 0x42;
 const int16_t I2C_SLAVE = 0x08;

 void setup() {
   Serial.begin(115200);  // start serial for output
   Wire.begin(SDA_PIN, SCL_PIN, I2C_MASTER);        // join i2c bus (address optional for master)
 }

 void loop() {
   using periodic = esp8266::polledTimeout::periodicMs;
   static periodic nextPing(1000);

   if (nextPing) {
     Wire.requestFrom(I2C_SLAVE, 6);    // request 6 bytes from slave device #8

     while (Wire.available()) { // slave may send less than requested
       char c = Wire.read(); // receive a byte as character
       Serial.print(c);         // print the character
     }
   }
 }





/*
#include <Wire.h>                       // Wire Library for i2c Teensy(Master) to communicate with the ESP8266 (Slave)

#define I2C_SDA 4
#define I2C_SCL 5

byte x = 0;  //i2c counter



void setup() {
  // Setup serial debug port
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP8266 Serial Port");
  // Setup i2c
//  Wire.pins(4,5);
  Wire.begin(6);          //join i2c bus There is an issue where ESP only works as Master
}

int value = 0;

void loop() {
  byte error, address;
    int nDevices;
    Serial.println("Scanning...");
    nDevices = 0;
    for(address = 1; address < 127; address++ ) {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address<16) {
          Serial.print("0");
        }
        Serial.println(address,HEX);
        nDevices++;
      }
      else if (error==4) {
        Serial.print("Unknow error at address 0x");
        if (address<16) {
          Serial.print("0");
        }
        Serial.println(address,HEX);
      }
    }
    if (nDevices == 0) {
      Serial.println("No I2C devices found\n");
    }
    else {
      Serial.println("done\n");
    }
    delay(5000);

/*
  //Test sending i2c data
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  x++;
  delay(500);
  Serial.print("x is ");
  Serial.println(x);
*/


// } 
