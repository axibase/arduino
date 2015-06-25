##Datacube

![datacube](https://github.com/axibase/arduino/blob/master/datacube/images/datacube.png)
 
Designed to inform you of system status unintrusively.

The sketch was tested on [arduino uno](http://www.arduino.cc/en/Main/ArduinoBoardUno) with [ethernet shield](http://www.arduino.cc/en/Main/ArduinoEthernetShield) and [arduino ethernet](http://www.arduino.cc/en/Main/ArduinoBoardEthernet) board. Also it will run on any board which use [ATmega328](http://www.atmel.com/ru/ru/devices/ATMEGA328.aspx) microcontroller.

The cover of cube was made using a [LED light box](http://www.ikea.com/us/en/catalog/products/60241544/)

## Installation

To learn how to upload and run sketches you can use the official arduino [guide](http://www.arduino.cc/en/Main/Howto)

## API

The sketch is able to recive the ip address using DHCP or if ip is not recived in one minute after start it will take it manually from the sketch.

To use the web-interface, find the ip of your arduino board and in your browser navigate to `http://boardIpAddr:8088`.

All api functions are based on a get-request to this host using the method `set`.

Possible parameters:

 - id                                   - Specify which LED you controll. Should be the first parameter. By default, the settings will apply to all LEDs.
 - color=[red,green,blue,...]           - set color.
 - level=[normal,bad,warning,...]       - set color witch applies to specified critical level.
 - light=[on/off]                       - tun on\off the LED light.
 - delay=[seconds]                      - set the delay. After this time, if no command are recived, the LED will start to blink.

## Example

 - set color of LED0 to blue
```bash
http://datacubeIp:8080/set?id=0&color=blue
```

 - set color of LED2 to green
```bash
http://datacubeIp:8080/set?id=2&level=good
```

 - turn off LED lights
```bash
http://datacubeIp:8080/set?light=off
```

 - set color of all LEDs to red, the LEDs will start to blink in will not recive any command in following 15 seconds
```bash
http://datacubeIp:8080/set?delay=15&color=red
```

## Usage

You can create rules in [ATSD](http://axibase.com/products/axibase-time-series-database/) to send web notifications when events occur.
For example, to show the state of your system.

The follow instruction will show how to monitor your cpu.

1. Go to ATSD web interface and open "Rules" tab.
![ATSD Rules](https://github.com/axibase/arduino/blob/master/datacube/images/rules.png)

2. Click the "Create" button at the bottom of page.
![create rule](https://github.com/axibase/arduino/blob/master/datacube/images/rules_bottom.png)

3. Complete the fields as shown on the following images. Do not forget to change datacubeIp to real ip that you use. Ensure that "enabled" checkbox is marked.
![rule overview](https://github.com/axibase/arduino/blob/master/datacube/images/overview.png)

4. Create a web notification as shown on the following image and click "Save". Ensure that "enabled" checkbox is marked.
![rule notify](https://github.com/axibase/arduino/blob/master/datacube/images/web_notify.png)

5. All done.
 - when the cpu load on your target machine will exceed 70, the datacube will light up red.
 - If the value of cpu will maintain a load over 70, the datacube will change color to purple. 
 - When the cpu load will drop below 70, the light will turn off.



