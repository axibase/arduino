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
String entityID = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";
String mqttServer = "nur.axibase.com";
int mqttServerPort = 1883;
String pubTopic = "iot/" + entityID + "/pub";
String subTopic = "iot/" + entityID + "/sub";


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
    client.connect(entityID.c_str());
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
  if(client.connected()) {
    //the data will be insert with measurement server timestamp
    String data = "series e:" + entityID + " m:millis=" + (String)getData();
//UNCOMMENT TO USE DHT SENSORS:
//    String data = "series e:" + entityID + " m:millis=" + (String)getData() + " m:temperature=" + (String)getTemperature() + " m:humidity=" + (String)getHumidity();  
    Serial.println("sending row: '" + data + "' ...");
    client.publish(pubTopic,data);
    Serial.println("sended.");
    client.loop();
    delay(1000);
  } else {
    Serial.println("Disconnected from server. Require to restart device.");
    while(true){delay(5000);}
  }

}

double getData() {
  return 200.0*sin((double)millis()/100000.0) + 300.0;
}

//UNCOMMENT TO USE DHT SENSORS:
//float getTemperature() {
//  return dht.readTemperature();
//}
//float getHumidity() {
//  return dht.readHumidity();
//}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incomming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}
