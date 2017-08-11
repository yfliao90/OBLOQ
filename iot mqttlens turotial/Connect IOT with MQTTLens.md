# Connect IOT with MQTTLens 

iot.dfrobot.com.cn is based on MQTT protocol. Any MQTT client is able to connect and use our IOT website.

MQTTLens is a free MQTT client. We will then use it to connect to iot.dfrobot.com.cn, and subscribe and publish to a specific MQTT device. 

You can find this software in Chrome extension store. Before get start, let's add it to the Chrome extension and run.

### 1. Establish MQTT connection

Open MQTTLens, click "+" on the left to start MQTT connection configuration.

<img src=".\img\01.png">

Fill the information below:

--Connection Name: Arbitrary name

--Host Name: iot.dfrobot.com.cn

--Client ID: Arbitrary string in English. Connection may fail if other language are used.

--username: Your iot_id. Get it from the Workshop page at this site. 

--password: Your iot_pwd. Get it from the Workshop page at this site. 

<img src=".\img\02.png">

<img src=".\img\05.png">

Iot_id and Iot_pwd can be found on the left side of workshop page (iot.dfrobot.com.cn). Fill these two in the red box 1 and 2. Click little “eye” to show your iot_id, iot_pwd.

<img src=".\img\04.png">

Then click "create connection" to establish a MQTT connection.



### 2. Subscribe an IOT device, and publish a message to this device

Now we show how to use MQTTLens to subscribe an IOT device, and how to publish messages to a device.

First, create a new device in Workshop. Then you will have device's id, named "Topic" (Check the 3rd red box of the picture above). 

<img src=".\img\06.png">

In MQTTLens, fill the device "topic" in the Subscribe box. Click Subscribe.

In the bottom, it shows all your subscriptions. 

<img src=".\img\07.png">

In MQTTLens, fill the device "topic" in the Publish box, and fill in the message which will be sent to your device. Click Publish.

If published successful, the message will appear in the bottom, as you have already subscribed this device, so you can receive any message sent to it. 

<img src=".\img\08.png">

You can also login to the IOT website to check the messages sent from MQTTLens. 

<img src=".\img\09.png">

<img src=".\img\10.png">