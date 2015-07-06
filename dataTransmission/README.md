##Usage

###### Step 1: Download project: 
```bash
git clone https://github.com/axibase/arduino
```

###### Step 2: Modify sketch:

Depends on mqtt/tcp protocol you will use, modify one of the following file:

```
 ./arduino/dataTransmission/atsd_tcp_example/atsd_tcp_example.ino
 ./arduino/dataTransmission/atsd_mqtt_example/atsd_mqtt_example.ino
```

Replace ssid and pass templates with your real ssid and password, or modify the sketch to use [ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield) instead of [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield).
```bash
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
```

######  Step 3: Set uniqID:

Also replace uniqID template with your personal ID in selected sketch.
You can get personal ID using [GUID generator](https://www.guidgenerator.com/online-guid-generator.aspx). After you got an ID, remove last 13 letters ( last "-" and everything that comes after).

For example:

Original:
```
3388eac3-a9cd-4292-93e1-c7a894c78330
````
Result:
```
String uniqID = "3388eac3-a9cd-4292-93e1";
```

######  Step 4: Upload sketch to your device using ArduinoIDE.


##MQTT Library
The sketches use [Joel Gaehwiler's MQTT library v1.5](https://github.com/256dpi/arduino-mqtt/releases/tag/v1.5.0).

Install it:

1. Manually by downloading from [git](https://github.com/256dpi/arduino-mqtt).

2. By using [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software) or letter in *Sketch* -> *Include Library* -> *Manage Libraries...* .Input "MQTT library for Arduino" in filter, and press "Install" on appeared library. 



##Hardware & Software
All sketches were tested with [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software) and [arduino uno](http://www.arduino.cc/en/Main/ArduinoBoardUno) with [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield)

##Test with your [ATSD](http://axibase.com/products/axibase-time-series-database/) instance

[Download](http://axibase.com/products/axibase-time-series-database/download-atsd/) and install [ATSD](http://axibase.com/products/axibase-time-series-database/). To send data using MQTT, you also should install an MQTT-broker, like [mosquitto](http://mosquitto.org/). Installation guide is available on the [official mosquitto download page](http://mosquitto.org/download/).

Make sure ATSD is running. Create rules in [ATSD](http://axibase.com/products/axibase-time-series-database/) to send mqtt messages when events occur.

1. Go to ATSD web interface and open the `Rules` tab.
![ATSD Rules](https://github.com/axibase/arduino/blob/master/dataTransmission/images/rules.png)

2. Click the `Create` button at the bottom of page. Or, click the `Import` button and choose the [mqttResponseRule.xml](https://github.com/axibase/arduino/blob/master/dataTransmission/mqttResponseRule.xml) file to create the rule. If you will import rules from a file, you can skip steps 3-4.
![create rule](https://github.com/axibase/arduino/blob/master/dataTransmission/images/rules_bottom.png)

3. Complete the fields as shown on the following images. Ensure that `enabled` checkbox is marked.
![rule overview](https://github.com/axibase/arduino/blob/master/dataTransmission/images/overview.png)

4. Insert the row: `mosquitto_pub -t iot/${entity}/sub -m "TEST ALERT!"` in each action field on `Actions` tab and press the `Save` button.
![action](https://github.com/axibase/arduino/blob/master/dataTransmission/images/action.png)

5. All done. Your arduino device will print an alert message to your serial monitor after each publishing.

##Sensors
How to connect and use DHT sensors with DHT library you find on [this ](https://learn.adafruit.com/dht/using-a-dhtxx-sensor) page.







