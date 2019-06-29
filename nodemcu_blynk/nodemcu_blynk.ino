/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <ArduinoJson.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "107505a5947842938e3f023638a4a987";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Leandro & Debora";
char pass[] = "miguel061321";

void setup()
{
  // Debug console
  Serial.begin(9600);

  Wire.begin(D1, D2);

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}

// Garra
BLYNK_WRITE(V1)
{
  Serial.print("Base: ");
  Serial.println(param.asInt());
  
  Wire.beginTransmission(8);
  Wire.write(param.asInt());
  Wire.endTransmission();
}

// Angulo
BLYNK_WRITE(V2)
{
  Serial.print("Angulo: ");
  Serial.println(param.asInt());

  Wire.beginTransmission(8);
  Wire.write("AN" + param[0].asInt());
  Wire.endTransmission();
}

// Altura
BLYNK_WRITE(V3)
{
  Serial.print("Altura: ");
  Serial.println(param.asInt());

  Wire.beginTransmission(8);
  Wire.write("AL" + param[0].asInt());
  Wire.endTransmission();
}

// Base
BLYNK_WRITE(V4)
{
  Serial.print("Base: ");
  Serial.println(param.asInt());

  Wire.beginTransmission(8);
  Wire.write("B" + param[0].asInt());
  Wire.endTransmission();
}

// Reset
BLYNK_WRITE(V5)
{
  Serial.print("Reset: ");
  Serial.println(param.asInt());

  Wire.beginTransmission(8);
  Wire.write("R" + param[0].asInt());
  Wire.endTransmission();
}

// Ligar
BLYNK_WRITE(V6)
{
  Serial.print("Ligar: ");
  Serial.println(param.asInt());

  Wire.beginTransmission(8);
  Wire.write("L" + param[0].asInt());
  Wire.endTransmission();
}

// Gravar
BLYNK_WRITE(V7)
{
  Serial.print("Gravar: ");
  Serial.println(param.asInt());

  Wire.beginTransmission(8);
  Wire.write("G" + param[0].asInt());
  Wire.endTransmission();
}

// Play
BLYNK_WRITE(V8)
{
  Serial.print("Play: ");
  Serial.println(param.asInt());

  Wire.beginTransmission(8);
  Wire.write("P" + param[0].asInt());
  Wire.endTransmission();
}
