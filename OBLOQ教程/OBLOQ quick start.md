# OBLOQ quick start

## 0. One minute introduction

OBLOQ module is a medium that connects devices to internet.

To monitor or even control your lamp, audio or any other electrical equipment, you need at first connect them to the internet. OBLOQ must be an easy and powerful tool at your hand. It is easy to configure OBLOQ module. There is no need to program complicated codes. Only 15 minutes, these little electrical devices will get their lives via internet.



Tool list

* devices that will be connected to internet
* controller ( i.e. Arduino UNO)
* OBLOQ module
* IOT website (iot.dfrobot.com.cn)

Combining all these together, devices will be able to exchange data via wifi. 

<img src="./img/oblog banner-2-02.jpg" style="zoom:100%">

Or you can say, OBLOQ module is something like a "network card".

This "network card" enable devices (which contains microcontroller) to push/pull data from internet.

It can be put in this way,
$$
device+micro Controller=smart Device
$$

$$
smart Device+OBLOQmodule=IOTSmartDevice
$$

All the data is transmited in the IOT website server (iot.dfrobot.com.cn). This website also gives a visualized graph of data which is further screened.



##### (addendum) OBLOQ module brief introduction

Function:  Wireless connect devices to Internet. The device can get or post data from/to IOT website. OBLOQ's signal light always keeps green when its operate normally. 

<img src="./img/OBLOQ引脚说明图.jpg" width="300px">



Interface: 

| Name |         Description         |
| :--: | :-------------------------: |
|  TX  |  Sending end (serial port)  |
|  RX  | Receiving end (serial port) |
| GND  |             GND             |
| VCC  |             VCC             |



Signal light:

| Color  | State             |
| :----: | :---------------- |
|  Red   | Error             |
| Yellow | Connect to server |
| Green  | Normal            |



## 1. Establish an project on IOT website

We will learn how to use OBLOQ by a simple example.



**Add a new device: device 1**

Login to iot.dfrobot.com.cn, enter the workshop. Add a new device. Its name is by default “new device 1”.

![创建设备Button](.\img\设备1.png)

Each device will have an unique id (Topic).

Iot_id and Iot_pwd are on the left of workshop page. 

![用户信息](.\img\IDToken.png)

keep in mind the information below for further use:

* iot_id: Skv3zKyNb
* iot_pwd: r1lD3ztJ4b
* Topic: BJTS0iaU-




## 2. Hardware##

Connect hardware using Dupont wire. Prepare wifi environment for OBLOQ. Upload control code to the microcontroller (Arduino UNO). The IOT device is ready to run.



**Hardware checklist**

| Name                                     | Quantity |
| ---------------------------------------- | -------- |
| OBLOQ module                             | 1        |
| Controller( i.e. Arduino UNO microcontroller*） | 1        |
| LED module                               | 1        |
| Dupont wire                              | Some     |

*We choose Arduino UNO as the controller, you can also use other compatible controller as you wish.



**schematic wiring diagram**

![最简单接收设备](./img/最简单接收设备.png)



## 3. Upload program to microcontroller

The microcontroller follows rules to control OBLOQ and device. These rules are coded in the controller program which is then uploaded to the controller board. After building the hardware, the next step is uploading required program to Arduinio UNO.



Here is sample code:

- ssid: testssid
- pwd: testpwd


- iot_id: Skv3zKyNb
- iot_pwd: r1lD3ztJ4b
- topic: BJTS0iaU-

Attention: Please verify below wifi configuration and iot account information according to your own needs.

```c++
Obloq olq(Serial, "ssid", "pwd");
olq.connect(client_id,iot_id,iot_pwd);
olq.subscribe(topic);
```

**Source code**

```c++
#include <ArduinoJson.h>
#include "Obloq.h"

bool sendFlag = true;
Obloq olq(Serial, "testssid", "testpwd");

void handleRaw(String& data)
{
    //Serial.println(data);   //串口打印返回的数据
}
void handleJson(JsonObject& data)
{
    int message = 0;
    if(strcmp(data["topic"],"BJTS0iaU-") == 0)
    {
        message = data["message"];
        switch(message)
        {
            case 1: digitalWrite(13,HIGH);break;
            case 2: digitalWrite(13,LOW) ;break;
            default:break;
        }
    }  
}

void setup()
{
    Serial.begin(9600);
    pinMode(13,OUTPUT);
    olq.setHandleRaw(handleRaw);
    olq.setHandleJson(handleJson);
}
void loop()
{
    olq.update();
    if(sendFlag && olq.getWifiState()==2)
    {
      sendFlag = false;
      olq.connect("SkxprkFyE-","Skv3zKyNb","r1lD3ztJ4b");
      olq.subscribe("BJTS0iaU-");
    }
}
```



**Attention: Before uploading program, make sure Rx(0) and Tx(1) pins of Arduino UNO are not connected to OBLOQ module.** Serial ports are occupied during program uploading. In our future tutorial, we will show how to use soft serial ports to avoid this problem.

After program is uploaded, connect pin Rx, Tx of Arduino UNO to pin Tx, Rx of OBLOQ module respectively. Press "Reset" to restart Arduino UNO. Then wait for OBLOQ module connect to internet automatically .



## 4. Demonstration

Now let's take a look at the result of this project. 

1. Keep all hardware and devices power on. When OBLOQ module operates normally, its LED (signal light) shows green and is always on. 
2. Login to iot.dfrobot.com.cn, enter Workshop, check if the operating device has been registered on IOT website.
3. Send number "1" to this device. The LED (pin 13) on Arduino UNO will be turned on. Send number "2", it will be turned off.


<img src="./img/switch1_2_开关小灯设备页面.png" width="800px">

Before sending any message, the LED (pin 13) on Arduino UNO is off.

<img src="./img/最简单教程结果展示_小灯灭.jpg" width="800px">

Sending number "1", the LED will be turned on.

<img src="./img/最简单教程结果展示_小灯亮_指示.jpg" width="800px">

Let's see on IOT website, in the middle of "My device" webpage, there is a screening tool -- Check message. Here if we select "Date range -- 1 Day" and click "check", we can get the screening result of the exchanged messages of today. System will automatically generate a visualized data diagram.

<img src="./img/switch1_2_开关小灯数据筛选页面.png" width="800px">



If you do not get the above results, check if something goes wrong in below steps.

* The wifi SSID, password, iot_user, iot_pwd, topic are correct.
* Connections between devices are correct. You may check twice if the pins using in hardware matches those in arduino UNO program.
* When obloq signal light keeps showing red, or yellow, please restart Arduino UNO.





## 5. Summary
This is a start up tutorial of  how to use iot website and OBLOQ to connect a LED device in arduino UNO to internet, then control this LED on/off.

Steps summary:

1. Create the project device on iot website
2. Connect all devices using Dupont wire
3. Upload required program to Arduino UNO
4. Sending "1" or "2" message in the iot workshop, check if LED (pin 13) on Arduino is turned on or off.


