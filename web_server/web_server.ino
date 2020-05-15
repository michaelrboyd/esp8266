/*
 *  Simple HTTP get webclient test
 */

#include <ESP8266WiFi.h>
#include <Wire.h>                       // Wire Library for i2c Teensy(Master) to communicate with the ESP8266 (Slave)

#include "credentials.h"

const char* hostname = WIFI_HOSTNAME;
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WiFiServer server(80);
byte x = 0;  //i2c counter

// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "Analog input:  " + String(analogRead(A0)) +
            "</html>" +
            "\r\n";
  return htmlPage;
}

void setup() {
  // Setup serial debug port
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP8266 Serial Port");
  // Setup i2c
  Wire.pins(4,5);
  Wire.begin(6);          //join i2c bus There is an issue where ESP only works as Master


  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.hostname(hostname);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

int value = 0;

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients


  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
          break;
        }
      }
    }
    delay(1000); // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("[Client disonnected]");
  }
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
