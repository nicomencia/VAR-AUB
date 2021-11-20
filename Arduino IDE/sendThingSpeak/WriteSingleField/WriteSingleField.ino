/*
  WriteSingleField
  
  Description: Writes a value to a channel on ThingSpeak every 20 seconds.
  
  Hardware: Arduino MKR WiFi 1010
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires WiFiNINA library
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
    
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/
#include <Arduino.h>
#include <OneWire.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#define DIGITAL_PIN_RAIN 6
#define ANALOG_PIN_RAIN A2

char ssid[] = SECRET_SSID;    //  your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int lightPin = A1;
int temperaturePin = A2;
OneWire onewire(2);

uint16_t rainVal;
boolean isRaining = false;
String raining;
int highRaining;

void setup() {
  Serial.begin(115200);  // Initialize serial

  pinMode(DIGITAL_PIN_RAIN, INPUT);
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
    
  ThingSpeak.begin(client);  //Initialize ThingSpeak
}

bool sensor_read(float *result)
{
  uint8_t data[12];
  int i;

  // retornar una lectura invalida si falla la comunicacion con el sensor
  *result = -100.0;

  // enviar comando de iniciar la conversion de temperatura
  // primero generamos pulso de reset
  onewire.reset();
  // enviar el comando skip ROM que selecciona todos los dispositivos en el bus
  onewire.skip();
  // enviar el comando de comienzo de conversion A/D
  onewire.write(0x44);

  // esperar el termino de conversion AD en el sensor
  delay(1000);

  // prestamos atención al pulso de presencia al generar el pulso de reset
  if (!onewire.reset())
    return false;
  // enviar el comando skip ROM que selecciona todos los dispositivos en el bus
  onewire.skip();
  // enviar comando de lectura de scratchpad
  onewire.write(0xBE);

  // comenzar lectura de datos
  for (i = 0; i < 9; i++)
    data[i] = onewire.read();

  // alinear los datos recibidos
  int16_t temp = (((int16_t)data[1]) << 11) | (((int16_t)data[0]) << 3);

  // convertir a grados centigrados
  *result = (float)temp * 0.0078125;

  // lectura satisfactoria
  return true;
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  // Get data from sensors

  // Soil moisture
  int soil_moisture = analogRead(A0);
  int moisture = map(soil_moisture, 675, 1023, 100, 0);
  Serial.println(moisture);

  // Light
  int light_reading = analogRead(lightPin);
  int light = map(light_reading, 1023, 50, 100, 0);
  Serial.println(light);

  // Temperature
  float temperature;
  if (sensor_read(&temperature))
  {
    Serial.print(F("OK, Temperatura: "));
    Serial.println(temperature);
  }
  else
  {
    Serial.println(F("Fallo de comunicacion con DS18B20"));
  }

  //Rain sensor
  rainVal = analogRead(ANALOG_PIN_RAIN);
  isRaining = digitalRead(DIGITAL_PIN_RAIN);
  if (isRaining) {
    raining = "No";
    highRaining = 0;
  }
  else {
    raining = "Yes";
    highRaining = 1;
  }
  rainVal = map(rainVal, 0, 1023, 100, 0);
  Serial.print("Raining: ");
  Serial.println(raining);
  Serial.print("Moisture: ");
  Serial.print(rainVal);
  Serial.println("%\n");
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  ThingSpeak.setField(1, moisture);
  ThingSpeak.setField(2, light);
  ThingSpeak.setField(3, temperature);
  ThingSpeak.setField(4, rainVal);
  ThingSpeak.setField(5, highRaining);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(20000); // Wait 20 seconds to update the channel again
}
