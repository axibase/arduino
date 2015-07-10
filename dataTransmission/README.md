
![connectedDevices](https://github.com/axibase/arduino/blob/master/dataTransmission/images/connectedDevices.png)
###Hardware & Software
All sketches were tested using [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software), [arduino uno](http://www.arduino.cc/en/Main/ArduinoBoardUno) with a [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield), and a [DHT11 sensor](http://www.micropik.com/PDF/dht11.pdf).

###Setup

###### Step 1: Download the Axibase Arduino project: 
```bash
git clone https://github.com/axibase/arduino
```

###### Step 2: Modify the sketch:

Depending on which protocol you are going to use, MQTT or TCP, modify one of the following files:

To get started we suggest you first try setting up your Arduino using the TCP protocol sketch, then you can try using the MQTT sketch with bidirectional messaging.

*NOTE: MQTT supports bidirectional messaging, which allows you to deliver messages to the microcontroller from the ATSD server.*

```
 ./arduino/dataTransmission/atsd_tcp_example/atsd_tcp_example.ino
 ./arduino/dataTransmission/atsd_mqtt_example/atsd_mqtt_example.ino
```

Replace ssid and pass parameters with the actual ssid and password, or modify the sketch to use the [ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield) instead of [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield).
```
char ssid[] = "ssid";           // your network SSID (name)
char pass[] = "ssidPass";       // your network password
```

######  Step 3: Generate a unique GUID for your microcontroller using the following service:

[GUID generator](https://www.guidgenerator.com/online-guid-generator.aspx)


######  Step 4: Setup your entityID:

Replace entityID parameter with the generated unique GUID in the chosen sketch.

*NOTE: Be sure to set a unique entityID (GUID) for each microcontroller that you are using.*

```
String entityID = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";
```

######  Step 5: Upload the sketch to your device using ArduinoIDE.


##MQTT Library
The sketches use [modified Gaehwiller's MQTT library v1.5](https://github.com/Sild/arduino-mqtt). 
This library was modified to support message length of greater than 90 characters. You can install the libraries using the following commands:
```
git clone https://github.com/Sild/arduino-mqtt
mv arduino-mqtt $ARDUINO_IDE_FOLDER/libraries/MQTT
```

There is some limitation to use message body - you can not publish message longer then 80 letter. It causing by buffer size limit. You can increase it by modifying library source code ( `MQTT/src/MQTTClient.h` ):
```
#define MQTT_BUFFER_SIZE 128 
```
Set ``MQTT_BUFFER_SIZE 256``, for example.

##DHT library & sensor
You can install the DHT library using Library Manager in your ArduinoIDE by typing the model name, for example "DHT11", in the search field and pressing `Install`, or by executing the following commands:
```
git clone https://github.com/adafruit/DHT-sensor-library
mv DHT-sensor-library $ARDUINO_IDE_FOLDER/libraries/DHT_sensor_library
```
If you are not using a DHT-sensor and a DHT-library, you should comment out the following lines in the MQTT sketch:
```
#include "DHT.h" //add DHT library
```
```
DHT dht(DHTPIN, DHTTYPE);
```
```
float getTemperature() {
  return dht.readTemperature();
}
float getHumidity() {
  return dht.readHumidity();
}
```

```
    String temperature = "series e:" + entityID + " m:temperature=" + (String)getTemperature();  
    Serial.println("sending row: '" + temperature + "' ...");
    client.publish(pubTopic,temperature);
    Serial.println("sended.");
    String humidity = "series e:" + entityID + " m:humidity=" + (String)getHumidity();  
    Serial.println("sending row: '" + humidity + "' ...");
    client.publish(pubTopic,humidity);
    Serial.println("sended.");
```

If you are not using a DHT-sensor and a DHT-library, you should comment out the following lines in the TCP protocol sketch:
```
    String temperature = "series e:" + entityID + " m:temperature=" + (String)getTemperature();  
    Serial.println("sending row: '" + temperature + "' ...");
    net.println(temperature);
    net.println();
    Serial.println("sended.");
    String humidity = "series e:" + entityID + " m:humidity=" + (String)getHumidity();  
    Serial.println("sending row: '" + humidity + "' ...");
    net.println(humidity);
    net.println();
    Serial.println("sended.");
```
If you have commented out the lines as intructed above, be sure to add the correct code for your sensor make and model in order for the device to function.

###Connecting the DHT11 Sensor
Connect your DHT11 sensor to your Arduino device as displayed on the following images:

![dht11](https://github.com/axibase/arduino/blob/master/dataTransmission/images/dht11.png)
![unoScheme](https://github.com/axibase/arduino/blob/master/dataTransmission/images/unoScheme.png)

In the sketch, be sure to specify the right data pin (2 by default):
```
//#define DHTPIN 2
```

##Test Arduino with your [ATSD](http://axibase.com/products/axibase-time-series-database/) instance

[Download](http://axibase.com/products/axibase-time-series-database/download-atsd/) and install [ATSD](http://axibase.com/products/axibase-time-series-database/). 

* To send data using MQTT, you need to install the [mosquitto](http://mosquitto.org/) MQTT-broker and MQTT-clients. 
The following commands will install mosquitto-broker and mosquitto-client on your server:
```
sudo apt-get update
sudo apt-get install mosquitto mosquitto-clients
```
Download start_mqtt.sh script and make it executable:
```
wget https://raw.githubusercontent.com/axibase/arduino/master/dataTransmission/start_mqtt.sh
chmod +x start_mqtt.sh
```
To start data transmission from mosquitto, run `start_mqtt.sh` script:
```
./start_mqtt.sh &
```
This script will start mosquitto-broker and will start sending data to ATSD. We expect that mqtt-broker, mqtt-clients and ATSD are running on one server. Otherwise you need to modify `start_mqtt.sh` script and set right value of `atsdServer` and `mqttServer`:
```
atsdServer="localhost"
mqttServer="localhost"
```

* Once ATSD is running, navigate to the Rules page located on the main menu of the ATSD UI to a create rule in the [ATSD Rule Engine](http://axibase.com/products/axibase-time-series-database/rule-engine/), that will send MQTT messages when events occur.

* Click the `Import` button and select the [mqttResponseRule.xml](https://github.com/axibase/arduino/blob/master/dataTransmission/mqttResponseRule.xml) file to create the rule.

[You can verify that all the fields of your Rule are completed correctly or setup the rule manually here](https://github.com/axibase/arduino/blob/master/dataTransmission/atsd_rules.md)

* Navigate to the Portals page located under Admin on the main menu in ATSD and create a visualization portal for your Arduino data.

Copy and paste the following code, substituting the `entity` values with your `entityID`, then click SAVE.
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

[View an example portal in Chart Lab](http://axibase.com/chartlab/fe9ebce1)

![chartLab](https://github.com/axibase/arduino/blob/master/dataTransmission/images/chartLab.png)

Once the portal is created, it can be found in the Portals dropdown list located on the main menu in ATSD.

* All done. Your arduino device will print an alert message to your serial monitor after each iteration. 

You can monitor the data using the visualization portal that you created. Which can be found in the Portals dropdown list located on the main menu in ATSD.

###Debugging MQTT 

If you will connect to your mosquitto server using a subscription client:
```
mosquitto_sub -t '#'
```
you will see the following messages:
```
series e:xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx m:millis=304.52
"TEST ALERT!"
```
