/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

  #include <WiFi.h>
  #include <HTTPClient.h>
#include <HardwareSerial.h>
/*
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
*/

// Replace with your network credentials
const char* ssid     = "minhchi1";
const char* password = "01286297269Dung";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://datamerge.000webhostapp.com/data-run.php";


String apiKeyValue = "tPmAT5Ab3j7F9";
String text="";
//HardwareSerial Serial1(2);




void setup() {
  Serial.begin(115200);
  Serial1.begin(115200,SERIAL_8N1, 4 ,2); //Baud rate, parity mode, RX, TX for arduino
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

 
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
      if (Serial1.available()) {
      text = Serial1.readStringUntil('e');
        // Specify content-type header
          text.replace(" ","");
          
            if (isNumeric(text)==true) {
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            
            // Prepare your HTTP POST request data
            String httpRequestData = "api_key=tPmAT5Ab3j7F9&student_id=" + text + "&location=B111";
        
           
            Serial.print("httpRequestData: ");
            Serial.println(httpRequestData);
        
        
            // Send HTTP POST request
            int httpResponseCode = http.POST(httpRequestData);
             
                  
              if (httpResponseCode>0) {
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);
                Serial1.print(httpResponseCode);
                Serial1.print('f');
                Serial1.flush();
              }
              else {
                Serial.print("Error code: ");
                Serial.println(httpResponseCode);
                Serial1.print(httpResponseCode);
                Serial1.print('f');
                Serial1.flush();
              }
            // Free resources
            
            http.end();
            text="";
            
            }
      else 
            {
              Serial.print(text);
              Serial.println("end");
              text="";
              //Serial1.flush();
            }
      }
      
    }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  //delay(30000);  
}

boolean isNumeric(String str) {
    if (str.length() <=0) return false;
    for(char i = 0; i < str.length(); i++) {
        if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )) {
            return false;
        }
    }
    return true;
}
