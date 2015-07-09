##Usage

###### Step 1: Download the Axibase Arduino project: 
```bash
git clone https://github.com/axibase/arduino
```

###### Step 2: Modify the sketch:

Depending on which protocol you will use, mqtt or tcp, modify one of the following file:

```
 ./arduino/dataTransmission/atsd_tcp_example/atsd_tcp_example.ino
 ./arduino/dataTransmission/atsd_mqtt_example/atsd_mqtt_example.ino
```

Replace ssid and pass templates with your real ssid and password, or modify the sketch to use the [ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield) instead of [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield).
```
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
```

######  Step 3: Setup your entityID:

Replace entityID template with your personal ID in the sketch that you chose.
You can get your personal ID using the [GUID generator](https://www.guidgenerator.com/online-guid-generator.aspx).
```
String entityID = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";
```

######  Step 4: Upload sketch to your device using ArduinoIDE.


##MQTT Library
The sketches use [modified Gaehwiller's MQTT library v1.5](https://github.com/Sild/arduino-mqtt). 
You can install the libraries using the following commands:
```
git clone https://github.com/Sild/arduino-mqtt
mv arduino-mqtt $ARDUINO_IDE_FOLDER/libraries/MQTT
```

##DHT11 sensor
If you are using a DHT11 sensor, you can uncomment a few lines in the code to enable data transmission from the sensors. Read comments in the sketch to see which line should be uncommented. For example:
```
//UNCOMMENT TO USE DHT SENSORS:
//#include "DHT.h"
```

##Hardware & Software
All sketches were tested with [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software) and [arduino uno](http://www.arduino.cc/en/Main/ArduinoBoardUno) with a [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield)

##Test Arduino with your [ATSD](http://axibase.com/products/axibase-time-series-database/) instance

[Download](http://axibase.com/products/axibase-time-series-database/download-atsd/) and install [ATSD](http://axibase.com/products/axibase-time-series-database/). To send data using MQTT, you also should install an MQTT-broker, like [mosquitto](http://mosquitto.org/). Installation guide is available on the [official mosquitto download page](http://mosquitto.org/download/).

Once ATSD is running. Go the the Rules page located on the main menu of the ATSD UI to create rules in the [ATSD Rule Engine](http://axibase.com/products/axibase-time-series-database/rule-engine/), that will send mqtt messages when events occur.

1. Go to ATSD web interface and open the `Rules` page located on the main menu.
![ATSD Rules](https://github.com/axibase/arduino/blob/master/dataTransmission/images/rules.png)

2. Click the `Create` button at the bottom of page. Or, click the `Import` button and select the [mqttResponseRule.xml](https://github.com/axibase/arduino/blob/master/dataTransmission/mqttResponseRule.xml) file to create the rule. If you will import rules from the file, you can skip steps 3 and 4.
![create rule](https://github.com/axibase/arduino/blob/master/dataTransmission/images/rules_bottom.png)

3. Complete the fields as shown on the following screenshot. Ensure that the `enabled` checkbox is marked.
![rule overview](https://github.com/axibase/arduino/blob/master/dataTransmission/images/overview.png)

4. Insert the following row: `mosquitto_pub -t iot/${entity}/sub -m "TEST ALERT!"` in each action field on the `Actions` tab and press the `Save` button.
![action](https://github.com/axibase/arduino/blob/master/dataTransmission/images/action.png)

5. Navigate to the Portals page located under Admin and create a visualization portal for your data.

[Use ATSD Visualization guides to create the portal](http://axibase.com/products/axibase-time-series-database/visualization/)

[View the Chart Lab example](http://axibase.com/chartlab/fe9ebce1)

Basic Arduino portal configuration example:

*You can copy paste the following example code, substituting the `entity` value with your `entityID`.*
```
[configuration]
  height-units = 1
  width-units = 1

[group]

[widget]
  type = chart
  time-span = 1 hour
  step-line = false

  [series]
    entity = 9aedc21d-d303-488d-9db4-78a1baed303f
    metric = temperature

  [series]
    entity = 9aedc21d-d303-488d-9db4-78a1baed303f
    metric = humidity
```
Resulting Portal:

![chartLab](https://github.com/axibase/arduino/blob/master/dataTransmission/images/chartLab.png)

6. All done. Your arduino device will print an alert message to your serial monitor after each publishing. If you will connect to your mosquitto server using a subscription client:
```
mosquitto_sub -t '#'
```
you will see the following messages:
```
series e:xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx m:millis=304.52
"TEST ALERT!"
```
You can monitor the data using the created portal. Which can be found in the Portals dropdown list located on the main menu in ATSD.

##Sensors
Connect your DHT11 sensor to your Arduino device as displayed on the following image:

![dht11](https://github.com/axibase/arduino/blob/master/dataTransmission/images/dht11.png)

In sketch, be sure to specify the right data pin (2 by default):
```
//#define DHTPIN 2
```








