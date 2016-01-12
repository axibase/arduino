#include <SPI.h>

#include "DHT.h" //add DHT library
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
    Serial.println(getTemperature());
    delay(15000);
}

float getTemperature() {
  return dht.readTemperature();
  //return random(100);
}


