### Creating Rules in ATSD / How your Rule should look like

1. Navigate to the Rules page located on the main menu of the ATSD UI to a create rule in the [ATSD Rule Engine](http://axibase.com/products/axibase-time-series-database/rule-engine/), that will send mqtt messages when events occur.
![ATSD Rules](https://github.com/axibase/arduino/blob/master/dataTransmission/images/rules.png)

2. Click `CREATE` to create a new Rule.
![create rule](https://github.com/axibase/arduino/blob/master/dataTransmission/images/rules_bottom.png)

3. Complete the fields as shown on the following screenshot. Ensure that the `enabled` checkbox is marked.
![rule overview](https://github.com/axibase/arduino/blob/master/dataTransmission/images/overview.png)

4. Insert the following row: `mosquitto_pub -t iot/${entity}/sub -m "TEST ALERT!"` in each action field on the `Actions` tab and click the `Save` button.
![action](https://github.com/axibase/arduino/blob/master/dataTransmission/images/action.png)
