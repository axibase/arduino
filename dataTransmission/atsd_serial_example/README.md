###Hardware & Software
All sketches were tested using [Arduino IDE v1.6.5](https://www.arduino.cc/en/Main/Software), [arduino uno](http://www.arduino.cc/en/Main/ArduinoBoardUno) with a [DHT11 sensor](http://www.micropik.com/PDF/dht11.pdf).

###Setup

###### Step 1: Download the Axibase Arduino project: 
```bash
git clone https://github.com/axibase/arduino
```


######  Step 2: Generate a unique GUID for your microcontroller using the following service:

[GUID generator](https://www.guidgenerator.com/online-guid-generator.aspx)


######  Step 3: Setup your entityID:

In the following sketch:
```
 ./arduino/dataTransmission/atsd_serial_example/atsd_serial_example.ino
```
Replace entityID parameter:
```
String entityID = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";
```
with the generated unique GUID 

*NOTE: Be sure to set a unique entityID (GUID) for each microcontroller that you are using.*

######  Step 4: Upload the sketch to your device using ArduinoIDE.

##DHT library & sensor
You can install the DHT library using Library Manager in your ArduinoIDE by typing the model name, for example "DHT11", in the search field and pressing `Install`, or by executing the following commands:
```
git clone https://github.com/adafruit/DHT-sensor-library
mv DHT-sensor-library $ARDUINO_IDE_FOLDER/libraries/DHT_sensor_library
```
If you are not using a DHT-sensor and a DHT-library, you should comment out the following lines in your respective sketch:
```
#include "DHT.h" //add DHT library
```
```
DHT dht(DHTPIN, DHTTYPE);
```
```
String temperature = "series e:" + entityID + " m:temperature=" + (String)getTemperature();  
Serial.println(temperature);
String humidity = "series e:" + entityID + " m:humidity=" + (String)getHumidity();  
Serial.println(humidity);
```
```
float getTemperature() {
  return dht.readTemperature();
}
float getHumidity() {
  return dht.readHumidity();
}
```
If you have commented out the lines as intructed above, be sure to add the correct code for your sensor's make and model in order for the device to function.

###Connecting the DHT11 Sensor
Connect your DHT11 sensor to your Arduino device as displayed on the following images:

![dht11](https://github.com/axibase/arduino/blob/master/dataTransmission/images/dht11.png)
![unoScheme](https://github.com/axibase/arduino/blob/master/dataTransmission/images/unoScheme.png)

In the sketch, be sure to specify the right data pin (2 by default):
```
#define DHTPIN 2
```

##Test Arduino with your [ATSD](http://axibase.com/products/axibase-time-series-database/) instance

####ATSD

[Download](http://axibase.com/products/axibase-time-series-database/download-atsd/) and install [ATSD](http://axibase.com/products/axibase-time-series-database/). 

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

You can monitor the data using the visualization portal that you created. Which can be found in the Portals dropdown list located on the main menu in ATSD.

####Configure java application
Modify the java [application](https://github.com/axibase/arduino/blob/master/dataTransmission/atsd_serial_example/SerialReader.java), specifying the correct ATSD hostname and port:
```
private static final String host = "localhost";
private static final int port = 8081;
```

####Compile and Run java application

Make sure ```rxtx-java``` library is installed. For example, in ubuntu:

```
sudo apt-get install librxtx-java
```


To compile, navigate to the project folder:
```
cd ./arduino/dataTransmission/atsd_serial_example
```
Then run the following command:
```
javac -cp ".:RXTXcomm.jar" SerialReader.java
```

Run the application:
```
java -cp ".:RXTXcomm.jar" SerialReader "/dev/ttyACMx"
```
Where ```/dev/ttyACMx``` is a path to the arduino serial port. ```/dev/ttyACM0``` is the default path.

You will see the following messages:
```
sended: series e:9aedc21d-d303-488d-9db4-78a1baed303f m:millis=284.56
sended: series e:9aedc21d-d303-488d-9db4-78a1baed303f m:millis=286.55
sended: series e:9aedc21d-d303-488d-9db4-78a1baed303f m:millis=289.09
```
This means that your data is being sent successfully.
