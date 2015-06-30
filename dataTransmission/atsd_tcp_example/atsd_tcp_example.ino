#include <SPI.h>
#include <WiFi.h>

//wifi settings
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
int status = WL_IDLE_STATUS;    // the Wifi radio's status

//mqtt connection settings
String uniqID = "xxxxxxxx-xxxx-xxxx-xxxx";
String atsdServer = "nur.axibase.com";
int atsdPort = 8081;


WiFiClient net;

void setup() {
  Serial.begin(9600);

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
    String data = "debug series e:" + uniqID + " m:millis=" + (String)getData();
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


String getResponse() {
  String response = "";
  while (net.available()) {
    response += (char)net.read();
  }
  return response;
}

double getData() {
  return 200.0*sin((double)millis()/100000.0) + 300.0;
}





