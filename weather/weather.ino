/*
 *  Simple HTTP get webclient test
 */

// #include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
// #include <WiFiClient.h>

#include "credentials.h"

const char* hostname = WIFI_HOSTNAME;
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;


// api call to openweathermap.com
const String endpoint = OPEN_WEATHER_FORECAST_ENDPOINT;
const String key = OPEN_WEATHER_API_KEY;

const size_t capacity = JSON_ARRAY_SIZE(3) + 2*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 340;
DynamicJsonDocument doc(capacity);

// WiFiServer server(80);

// prepare a web page to be send to a client (web browser)
/*
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
*/

void setup() {
  // Setup serial debug port
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP8266 Serial Port");

  // We start by connecting to a WiFi network

//  Serial.println();
//  Serial.println();
  Serial.print("\n\nConnecting to ");
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
//   server.begin();
//  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

int value = 0;

void loop() {
  // WiFiClient client = server.available();   // Listen for incoming clients
if ((WiFi.status() == WL_CONNECTED)) {

     WiFiClient client;

     HTTPClient http;

//     Serial.println(endpoint + key);

//     Serial.print("[HTTP] begin...\n");
     if (http.begin(client, endpoint + key)) {

//       Serial.print("[HTTP] GET...\n");
       // start connection and send HTTP header
       int httpCode = http.GET();

       // httpCode will be negative on error
       if (httpCode > 0) {
         // HTTP header has been send and Server response header has been handled
  //       Serial.printf("[HTTP] GET... code: %d\n", httpCode);

         // file found at server
         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
           String payload = http.getString();
           Serial.println(payload);
           //start json parsing
           deserializeJson(doc, payload);
           JsonObject main = doc["main"];
           int main_temp = main["temp"];    //default value is float, but all I care about is integer
           int main_humidity = main["humidity"];
           Serial.print("temperature is = ");
           Serial.println(main_temp);
           Serial.print("Humidity = ");
           Serial.println(main_humidity);
         }
       } else {
         Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
       }

       http.end();
     } else {
       Serial.printf("[HTTP} Unable to connect\n");
     }
   }

 delay(30000);
}



/*
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

      HTTPClient http;
      Serial.println(endpoint + key);
      http.begin(endpoint + key); //Specify the URL
      int httpCode = http.GET();  //Make the request

      if (httpCode > 0) { //Check for the returning code

          String payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload);
        }

      else {
        Serial.println("Error on HTTP request");
      }

      http.end(); //Free the resources
    }

    delay(30000);


/*
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
*/
