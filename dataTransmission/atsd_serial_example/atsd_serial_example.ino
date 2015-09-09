#include <SPI.h>


#include "DHT.h" //add DHT library
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);

//mqtt connection settings
String entityID = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";

void setup() {
  Serial.begin(9600);
  dht.begin(); //Uncomment to use sensors DHT sensors.
}

void loop() {
    String data = "series e:" + entityID + " m:millis=" + (String)getData();
    Serial.println(data);

    String temperature = "series e:" + entityID + " m:temperature=" + (String)getTemperature();  
//    Serial.println(temperature);
    
    String humidity = "series e:" + entityID + " m:humidity=" + (String)getHumidity();  
//    Serial.println(humidity);
    delay(1000);
}


float getData() {
  return 200.0*sin((double)millis()/100000.0) + 300.0;
}

//UNCOMMENT TO USE DHT SENSORS:
float getTemperature() {
  return dht.readTemperature();
}
float getHumidity() {
  return dht.readHumidity();
}
