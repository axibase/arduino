##MQTT Protocol

Download project: 
```bash
git clone https://github.com/axibase/arduino
```

Replace ssid and pass templates with your real ssid and password, or modify the sketch to use [ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield) instead of [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield).
```bash
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
```

Replace uniqID template by your personal ID. You can get it by [GUID generator](https://www.guidgenerator.com/online-guid-generator.aspx). After you get ID, remove last 13 letters ( last "-" and all after it ).

```bash
String uniqID = "xxxxxxxx-xxxx-xxxx-xxxx";
```

Replace ssid and pass templates with your real ssid and password, or modify the sketch to use [ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield) instead of [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield)
```bash
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
```

Install mqtt-server and mqtt-subscription client, [mosquitto](http://mosquitto.org/) for example. You can find installation instruction on [official project page](http://mosquitto.org/download/).

Go to projectDirectory/dataTransmission/ and run start_mqtt.sh script:
```bash
./start_mqtt.sh > mqtt.log 2>&1 &
```
Ensure that script is placed on the same machine where atsd is installed.

Upload sketch to your device by ArduinoIDE.


####MQTT Library
The sketches use [Joel Gaehwiler's MQTT library v1.5](https://github.com/256dpi/arduino-mqtt/releases/tag/v1.5.0).

You can install it:

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

Upload sketch to your device by ArduinoIDE.

##Hardware & Software
All sketches was tested with [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software) and [arduino uno](http://www.arduino.cc/en/Main/ArduinoBoardUno) with [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield)




