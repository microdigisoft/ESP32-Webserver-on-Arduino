//copyright @ https://microdigisoft.com  //

#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#include <stdio.h>
#include <stdint.h>
//Create a static buffer for framing BME280 sensor data with ESP32 chip ID.
StaticJsonBuffer<200> jsonBuffer;
JsonObject& BME280DATA = jsonBuffer.createObject();
char JSONmessageBuffer[200];
//Device Chip ID in unsigned 64bit interger
uint64_t chipid;
// Initilize BME280 Sensor Module
Adafruit_BME280 bme; // Connected with I2C pin with ESP32
float temperature = 0;//to record temperature reading
float humidity = 0; //to record humidity reding

void setup() {
  Serial.begin(115200); //Start Serial port
  // Chip ID extraction to String
  chipid = ESP.getEfuseMac(); //Get the CHIP MAC ADRESS
  Serial.printf("ESP32 ID = %04X", (uint16_t)(chipid >> 32));//print High2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
   char chipIDChar1[10]; //print High2 bytes
  sprintf(chipIDChar1, "BME280-STATION-%04X", (uint16_t)(chipid >> 32));
  char chipIDChar2[10];//print Low 4bytes.
  sprintf(chipIDChar2, "%08X", (uint32_t)chipid);
  Serial.println(chipIDChar1); // holding chip ID characters
  Serial.println(chipIDChar2); //
  BME280DATA["ESP32 ID"] = chipIDChar1; // Adding CHIPID
  // Start BME280 Sensor
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin();
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  // Temperature in Celsius
  temperature = bme.readTemperature();
  // Uncomment the next line to set temperature in Fahrenheit
  // (and comment the previous temperature line)
  //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
  // Convert the value to a char array
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString); //convesion to string
  Serial.print("Temperature: ");
  Serial.println(tempString);
  humidity = bme.readHumidity();
  // Convert the value to a char array
  char humString[8];
  dtostrf(humidity, 1, 2, humString); //convesion to string
  Serial.print("Humidity: ");
  Serial.println(humString);
  Serial.print("Pressure: ");
  Serial.println(bme.readPressure() / 100.0F);
  BME280DATA["TEMPERATURE"] = tempString;
  BME280DATA["HUMIDITY"] = humString;
  BME280DATA["PRESSURE"] = bme.readPressure() / 100.0F;
  BME280DATA.printTo(Serial);
  BME280DATA.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("");
  //Serial.print("BME280 Data Reading =");
  //Serial.println(JSONmessageBuffer);
  delay(5000);
}
