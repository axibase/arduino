#include <MQTTClient.h>
#include <SPI.h>
#include <WiFi.h>
#include <Bridge.h>

//UNCOMMENT TO USE DHT SENSORS:
//#include "DHT.h"
// //Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTPIN 2
//DHT dht(DHTPIN, DHTTYPE);

//wifi settings
char ssid[] = "ssid";          //  your network SSID (name)
char pass[] = "ssidPass";      // your network password
int status = WL_IDLE_STATUS;   // the Wifi radio's status

//mqtt connection settings
String uniqID = "xxxxxxxx-xxxx-xxxx-xxxx";
String mqttServer = "nur.axibase.com";
int mqttServerPort = 1883;
String pubTopic = "iot/" + uniqID + "/pub";
String subTopic = "iot/" + uniqID + "/sub";

WiFiClient net;
MQTTClient client(mqttServer.c_str(), 1883, net);

void setup() {
  Serial.begin(9600);
//  dht.begin(); //Uncomment to use sensors DHT sensors.
  
  while ( status != WL_CONNECTED) {
    Serial.println();
    Serial.print("Connecting by WPA to SSID: " + (String)ssid + " ...");
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("connected.");
  while (!client.connected()) {
    Serial.print("Connecting to mosquitto server: ");
    Serial.print(mqttServer);
    Serial.print(":");
    Serial.println(mqttServerPort);
    client.connect(uniqID.c_str());
  }
  Serial.println("Connection with mqtt server established.");
  Serial.print("subscribing to topic: " + subTopic + " ...");
  client.subscribe(subTopic);
  Serial.println("subscribed.");
  Serial.println();
  Serial.println("Publish to topic: " + pubTopic);
  Serial.println();
}

void loop() {
  String data = "debug series e:" + uniqID + " m:millis=" + (String)getData();
  Serial.println("sending row: '" + data + "' ...");
  client.publish(pubTopic,data);
  Serial.println("sended.");
//  Uncomment to use sensors:
//  delay(1000);
//  String sensorsData = "debug series e:" + uniqID + " m:millis=" + (String)getSensorsData();
//  Serial.println("sending row: '" + sensorsData + "' ...");
//  client.publish(pubTopic,sensorsData);
//  Serial.println("sended.");
  client.loop();
  delay(1000);
}

double getData() {
  return 200.0*sin((double)millis()/100000.0) + 300.0;
}

//UNCOMMENT TO USE DHT SENSORS:
//float getSensorsData() {
//  return dht.readTemperature();
//}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incomming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}
