#include <TinyGPSPlus.h>
//#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/


//#define ENABLE_RSSI true

#include "Arduino.h"
#include "LoRa_E220.h"
#include <SoftwareSerial.h>
#include <WiFi.h>

#include "esp_sleep.h"





static const int RXPin = 0, TXPin = 1;
static const int LoraRXPin = 4, LoraTXPin = 3;
static const uint32_t GPSBaud = 9600;
HardwareSerial GPS_Serial(1);
HardwareSerial LORA_Serial(0);
// The TinyGPSPlus object
TinyGPSPlus gps;


LoRa_E220 e220ttl(&LORA_Serial, 2, 6, 7); // AUX M0 M1
//LoRa_E220 e220ttl(&LORA_Serial); // AUX M0 M1

void SendInfo();
void ReadGPS();
void bedtime();

void setup()
{
  Serial.begin(9600);
  GPS_Serial.begin(9600, SERIAL_8N1, RXPin, TXPin);
  LORA_Serial.begin(9600, SERIAL_8N1, LoraRXPin, LoraTXPin);
  e220ttl.begin();
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  Serial.println("Setup Complete");
 // delay(50);
  //e220ttl.setMode(MODE_0_NORMAL);
  delay(100);
  //WiFi.mode(WIFI_OFF);
  //btStop();  
}

void loop()
{
  ReadGPS();
  //e220ttl.sendBroadcastFixedMessage(23, "Test Message");
  //delay(1000);
  //SendInfo();

  //bedtime();
}

void SendInfo()
{
  
}






void ReadGPS(){

while (GPS_Serial.available() > 0)
    if (gps.encode(GPS_Serial.read()))
    if (gps.location.isValid() && gps.satellites.isValid()){
      digitalWrite(8, LOW);
      e220ttl.sendBroadcastFixedMessage(23, String(gps.location.lat(), 6));
      //delay(50);
      e220ttl.sendBroadcastFixedMessage(23, String(gps.location.lng(), 6));
     // delay(50);
      e220ttl.sendBroadcastFixedMessage(23, String(gps.satellites.value(), 6));
      //delay(50);
      bedtime();
    }
      
     // delay(2000);
  
 if (millis() > 10000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void bedtime(){
  //e220ttl.setMode(MODE_3_CONFIGURATION);
  //delay(200);
  digitalWrite(8, HIGH);
  esp_deep_sleep(2 * 1000000);
}