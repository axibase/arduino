##MQTT Protocol

Download project: 
```bash
git clone https://github.com/axibase/arduino
```

Modify arduino/dataTransmission/atsd_mqtt_example/atsd_mqtt_example.ino:

Replace ssid and pass templates with your real ssid and password, or modify the sketch to use [ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield) instead of [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield).
```bash
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
```

Replace uniqID template with your personal ID. You can get it using [GUID generator](https://www.guidgenerator.com/online-guid-generator.aspx). After you got an ID, remove last 13 letters ( last "-" and everything that comes after).

```bash
String uniqID = "xxxxxxxx-xxxx-xxxx-xxxx";
```


Install mqtt-server and mqtt-subscription client, for example [mosquitto](http://mosquitto.org/). You can find installation instruction on [official project page](http://mosquitto.org/download/).

Go to arduino/dataTransmission/ and run start_mqtt.sh script:
```bash
./start_mqtt.sh > mqtt.log 2>&1 &
```
Ensure that script is placed on the same machine where atsd is installed.

Upload sketch to your device using ArduinoIDE.


####MQTT Library
The sketches use [Joel Gaehwiler's MQTT library v1.5](https://github.com/256dpi/arduino-mqtt/releases/tag/v1.5.0).

Install it:

1. Manually by downloading from [git](https://github.com/256dpi/arduino-mqtt).

2. By using [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software) or letter in *Sketch* -> *Include Library* -> *Manage Libraries...* .Input "MQTT library for Arduino" in filter, and press "Install" on appeared library. 


##TCP Protocol

Download project: 
```bash
git clone https://github.com/axibase/arduino
```

Replace ssid and pass templates with your real ssid and password, or modify the sketch to use [ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield) instead of [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield)
```bash
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
```

Upload sketch to your device using ArduinoIDE.

##Hardware & Software
All sketches were tested with [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software) and [arduino uno](http://www.arduino.cc/en/Main/ArduinoBoardUno) with [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield)




