#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int led_gpio1 = 26;
int led_gpio2 = 27;

#define Authorization_key "LfOgY4mJ6n4X66PO_sS865aUBGKd-5jI"                    
#define SSID "xxxxxx"       // replace with your SSID
#define Password "xxxxx"           //replace with your password

void setup() {  
  pinMode(led_gpio1, OUTPUT); 
  pinMode(led_gpio2, OUTPUT); 
  Serial.begin(115200);
  delay(10);
  WiFi.begin(SSID, Password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  } 
  Blynk.begin(Authorization_key,SSID,Password);
}
void loop(){
    Blynk.run();
}
