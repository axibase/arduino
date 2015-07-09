#include <SPI.h>
#include <WiFi.h>


//UNCOMMENT TO USE DHT SENSORS:
//#include "DHT.h"
// //Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTPIN 2
//DHT dht(DHTPIN, DHTTYPE);


//wifi settings
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
int status = WL_IDLE_STATUS;    // the Wifi radio's status

//mqtt connection settings
String uniqID = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";
String atsdServer = "nur.axibase.com";
int atsdPort = 8081;


WiFiClient net;


void setup() {
  Serial.begin(9600);
//  dht.begin(); //Uncomment to use sensors DHT sensors.

  while ( status != WL_CONNECTED) {
    Serial.println();
    Serial.print("Connecting by WPA to SSID: " + (String)ssid + " ...");
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("connected.");
  while (!net.connected()) {
    Serial.print("Connecting to atsd server: ");
    Serial.print(atsdServer);
    Serial.print(":");
    Serial.println(atsdPort);
    net.connect(atsdServer.c_str(), atsdPort);
  }
}








void loop() {
  if(net.connected()) {
    //the data will be insert with measurement server timestamp
    String data = "series e:" + uniqID + " m:millis=" + (String)getData();
    //uncomment to use sensor
//    String data = "series e:" + uniqID + " m:millis=" + (String)getData() + " m:temperature=" + (String)getSensorsData();  
    Serial.print("sending row: '" + data + "' ...");
    net.println(data);
    net.println();
    Serial.println("sended.");
  } else {
    Serial.println("disconnected from server.");
    net.stop();
    net.connect(atsdServer.c_str(), atsdPort);
  }

  if (!net.connected()) {
    Serial.println("disconnected from server.");
    net.stop();
  } else {
    Serial.println("response: " + getResponse());
  }
  delay(1000);
}

float getData() {
  return 200.0*sin((double)millis()/100000.0) + 300.0;
}

//UNCOMMENT TO USE DHT SENSORS:
//float getTemperature() {
//  return dht.readTemperature();
//}
//float getHumidity() {
//  return dht.readHumidity();
//}

String getResponse() {
  String response = "";
  while (net.available()) {
    response += (char)net.read();
  }
  return response;
}





