##MQTT Protocol

###### Step 1: Download project: 
```bash
git clone https://github.com/axibase/arduino
```

###### Step 2: Modify arduino/dataTransmission/atsd_mqtt_example/atsd_mqtt_example.ino:

Replace ssid and pass templates with your real ssid and password, or modify the sketch to use [ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield) instead of [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield).
```bash
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
```

######  Step 3: Replace uniqID template with your personal ID:

You can get personal ID using [GUID generator](https://www.guidgenerator.com/online-guid-generator.aspx). After you got an ID, remove last 13 letters ( last "-" and everything that comes after).

```bash
String uniqID = "xxxxxxxx-xxxx-xxxx-xxxx";
```

######  Step 4: Upload sketch to your device using ArduinoIDE.


####MQTT Library
The sketches use [Joel Gaehwiler's MQTT library v1.5](https://github.com/256dpi/arduino-mqtt/releases/tag/v1.5.0).

Install it:

1. Manually by downloading from [git](https://github.com/256dpi/arduino-mqtt).

2. By using [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software) or letter in *Sketch* -> *Include Library* -> *Manage Libraries...* .Input "MQTT library for Arduino" in filter, and press "Install" on appeared library. 


##TCP Protocol

######  Step 1: Download project: 
```bash
git clone https://github.com/axibase/arduino
```

######  Step 2: Modify arduino/dataTransmission/atsd_tcp_example/atsd_tcp_example.ino:

Replace ssid and pass templates with your real ssid and password, or modify the sketch to use [ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield) instead of [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield)
```bash
char ssid[] = "ssid";           //  your network SSID (name)
char pass[] = "ssidPass";       // your network password
```

######  Step 3: Upload sketch to your device using ArduinoIDE.

##Hardware & Software
All sketches were tested with [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software) and [arduino uno](http://www.arduino.cc/en/Main/ArduinoBoardUno) with [wifi shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield)




