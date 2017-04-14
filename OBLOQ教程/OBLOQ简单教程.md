# OBLOQ简单教程

## 0. 一分钟简介

如果想将小台灯、小喇叭或其他用电器接入网络，对它们进行简单的远程监控和操作，OBLOQ一定是你的首选。OBLOQ配置十分简单，无需复杂的编程，15分钟就足够让这些小物件在互联网上拥有生命。



OBLOQ包括一系列物联网产品：

* 可监控设备
* 主控板（如 Arduino UNO）
* OBLOQ模块
* OBLOQ-web服务器

经过适当组合，在wifi环境下，可监控设备就能连上Internet，按时发送设备状态，并且接收远端控制指令。

### OBLOQ产品逻辑图



形象点说，可以把OBLOQ看成一个“网卡”。

可监控设备（包含主控）插上这个“网卡”，在Wifi环境下就能和互联网互通数据了。

这些数据被储存在OBLOQ-web服务器上（以下简称OBLOQ-web），用户可以通过[设备页面][www.baidu.com]浏览所有相关数据。



不仅如此，OBLOQ-web又是一个数据转发和发布控制指令的中心。

* 用户能够直接通过OBLOQ-web发送控制指令，进行设备控制；
* 也可以将OBLOQ-web当作一个指令中转站，用“设备1”（sender）给远端“设备2”（receiver）发送控制指令。



具体怎么实现呢？下面进入正题，我们将用OBLOQ实现一个按键远程无线控制一台台灯的亮灭。



## 1. 在OBLOQ-web上创建项目

登录[OBLOQ][obloq.dfrobot.com.cn]官网，注册账号，记录账号和密码（例如：username: obloquser; password: 20170307）。



接下来的项目，需要用一个按键控制台灯的亮灭。

这里需要在OBLOQ数据库里建立两个新设备，

* 一个设备发送指令（sender），指令经OBLOQ-web到达另一个设备，如此操控第二个设备；
* 另一个设备接收指令（receiver），并按照指令操作。



进入设备界面，创建**设备1：Button**。

Button（sender）发送出“触发”/“未触发”的信息，操控台灯的开关。

### 图



创建**设备2：Lamp**。

Lamp（receiver）接收Button发出的信息，如果Button被“触发”，则打开；否则关闭。

### 图



**注：用户名、密码以及设备名需要记录，稍后编程时需要使用。**

这里我们且记下这些关键字：

* OBLOQ-web账号：obloquser
* OBLOQ-web密码：20170307
* 设备1名称：Button
* 设备2名称：Lamp



到此为止，OBLOQ网络端的操作已全部完成。为了看到最后的效果，我们接下来需要搭建硬件，然后给硬件安装程序（固件）。



## 2. 硬件搭建##

首先，需要用杜邦线连接好OBLOQ系列硬件。

包括：

| 名称                   | 数量                |
| -------------------- | ----------------- |
| OBLOQ模块              | 至少$$1$$ ，推荐 $$2$$ |
| 主控板（Arduino UNO控制板*） | 至少$$1$$ ，推荐 $$2$$ |
| 杜邦线                  | 若干                |
| 按键（可选项）**            | $$0$$ ，推荐 $$1$$   |
| 台灯 （或 LED模块）***      | $$1$$             |

*关于主控板，这里挑选Arduino UNO作为例子讲解，其他兼容的主控板也可以使用。

**如果只有1套OBLOQ模块和主控，这套系统也是能实现的。此时由于没有多余的主控和OBLOQ模块去支持“按键”，我们只能在OBLOQ-web网站中创建一个虚拟按键功能，并用虚拟按键控制远端台灯的亮灭。

***台灯和LED模块都可以作为被操控设备，任选一个即可。如果想适用于实际生活，推荐使用台灯。



整体硬件结构示意图如下：

### 图



可以这样理解，
$$
设备+主控板=智能设备
$$

$$
智能设备+OBLOCK模块=可联网的智能设备
$$

这样一套硬件设备搭建完成后，只要给予wifi环境，并给主控板输入适当的程序（固件），就能直接使用了。

硬件具体连线图如下：



**发送端**

<img src=".\img\OBLOQ发送设备.png" style="zoom:100%">

**接收端**（当使用LED模块代替台灯时）

<img src=".\img\接收设备&led.png" style="zoom:100%">



如果仅使用LED模块，按照上图连接，就能完成全部硬件搭建。

如果使用的是小灯，我们还需要对小灯做一些改装，才能代替LED模块。

### 改装方案



#### 附：硬件结构简介

##### OBLOQ模块

功能：无线上网模块。能使设备能通过wifi连接上网，收发数据。

结构图：

<img src="./img/OBLOQ引脚说明图.jpg" width="400px">



各个接口的含义

|  名称  | 功能描述  |
| :--: | :---: |
|  TX  | 串口发送端 |
|  RX  | 串口接收端 |
| GND  | 电源负极  |
| VCC  | 电源正极  |



信号灯含义

|  颜色  |        指示状态         |
| :--: | :-----------------: |
|  红色  |       没有正常运行        |
|  白色  |                     |
|  蓝色  |        有新版固件        |
|  黄色  |      连接服务器的过程中      |
|  绿色  | 正常工作 server connect |

##### Arduino UNO

功能：主控板。它是Arduino系列的一款开源电子平台，主要基于AVR单片机的微控制器和相应的开发软件，为非电子专业和业余爱好者使用而设计。（[详细使用说明](http://www.dfrobot.com.cn/images/upload/File/20150129112804mdoxsq.pdf)）

结构图：

<img src=".\img\UNO R3.png" width="600px">

各个接口的含义：

<img src=".\img\UNO.png" style="zoom:100%">



## 3. 在主控板上烧录控制程序

### 硬件搭好的实物图

完成硬件搭建后，为了能让这些模块和板子运作起来，我们不但要给它们供电，还要给主控板写入运作逻辑的程序。

这次，控制程序是直接写入主控板EEPROM的，除非再次写入，否则硬件运作的逻辑不会改变。所以这类程序又叫固件，固件是担任着一个系统最基础最底层工作的软件。而往硬件的EPROM或EEPROM里写入程序的过程叫程序烧录。

现在，我们要制作两个固件程序，分别控制按键和台灯（或LED模块）。



### 3.1 发送端程序——按键（Button）

这段程序适用于有两套OBLOQ模块和主控的用户，如果您只有一套，小节末尾段落另行说明实现方法。



**程序逻辑说明**

发送端的程序主要是循环检测事件是否触发，一旦事件触发，就向OBLOQ-web发送信号，OBLOQ-web则向相应联网设备转发此信号。

本例中，程序会反复检测按键是否按下，当检测到按键第一次按下的时候，OBLOQ系统向设备Lamp发送消息”1“，当检测到按键第二次按下的时候，向设备Lamp发送消息”0“，如此循环检测执行。

代码已经写好了。使用时注意修改关于用户的一些信息，就可以烧录到控制按键的主控板中使用。

```c++
#define WIFI_SSID       "DFSoftware"       //wifi名称
#define WIFI_PASSWD     "dfrobotsoftware"  //wifi密码
#define IOT_USERNAME    "obloquser"             //物联网账号
#define IOT_PASSWD      "20170307"             //物联网账号密码
```



**具体代码**

```c++
/*
  DFRobot_advancedSend

 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 *function:
 The IOT device sends a message when the key is pressed
 We can convert the integer number to a string and send it out through the IOT device

 created 2017/3/6
 by Jason
 */
 
#include <SoftwareSerial.h>
#include "Iot.h"

void * eventCb(uint8_t type, const char *data, uint16_t len);
Iot iot(eventCb);                          //当物联网既需要接收数据有需要发送数据的时候，需要传入回调函数

SoftwareSerial mySerial(10, 11);           // RX, TX

#define WIFI_SSID       "DFSoftware"       //wifi名称
#define WIFI_PASSWD     "dfrobotsoftware"  //wifi密码
#define IOT_USERNAME    "obloquser"             //物联网账号
#define IOT_PASSWD      "20170307"             //物联网账号密码

char *tempString =      "string";
int normalVoltage = 0;                     //按键防抖动的相关参数
bool isPress = false;
bool isClick = false;;
bool hasPress = false;
int  buttonPin = 2;
bool currentState = false;
unsigned long currentTime = 0;
bool sendFlag  = true;

//连接状态回调函数
void * eventCb(uint8_t eventType, const char *data, uint16_t len)
{
}

void setup(void)
{
  mySerial.begin(38400);                   //打开软串口，波特率必须是38400
  pinMode(buttonPin,INPUT);
  iot.setup(mySerial, WIFI_SSID, WIFI_PASSWD, IOT_USERNAME, IOT_PASSWD);
  iot.start();
}
 
void loop(void)
{
  keyScan();
  if(isClick)
  {
    if(sendFlag){
      itoa(1,tempString,10);                //将整型转换成字符串，然后再发送
      iot.publish("Button", tempString);
      sendFlag = false;
    }
    else{
      iot.publish("Button", "0");             //直接发送字符串0也可以
      sendFlag = true;
    }    
  }
 iot.loop();
}

//按键扫描
void keyScan()
{
   if((digitalRead(buttonPin)!=normalVoltage)&&!isPress)
    {
        isPress = true;
        currentTime = millis();
    }
    else if (isPress)
    {
        if (millis()-currentTime>=10)
        {
            if(digitalRead(buttonPin)!=normalVoltage)
            {
                currentState = true;
            }
            else
            {
                currentState = false;
                isPress = false;
            }
        }
    }
    isClick = false;
    if (currentState) {
        hasPress = true;
    }
    else if(hasPress) {
        isClick = true;
        hasPress = false;
    }
}
```



**如何将程序烧录到Arduino UNO**

查看 [Arduino中文手册](http://www.dfrobot.com.cn/images/upload/File/20150129112804mdoxsq.pdf) 第1-13页，下载安装Arduino IDE和驱动，配置板卡和串口，输入程序源码，进行校验（也就是编译），最后下载（也就是烧录）。



**没有实物按键模块，我们可以用虚拟按键**

如果只有一套OBLOQ模块和主控，按键功能只能通过在OBLOQ-web网站虚拟发送按键开关的信息实现。

### 实现方案。



### 3.2 接收端程序——台灯/LED模块（Lamp）



**程序逻辑说明**

接收端的程序主要是循环检测监听的设备是否发出信息，一旦该设备发出信息，将根据信息内容执行相应任务。

本例中，设备Lamp会反复判断是否接到来自设备Button的新消息。当接收到数字”1“，打开LED；若接收到数字”0“，则关闭LED。如此循环检测执行。

同上，使用代码时注意修改关于用户的一些信息，就可以烧录到控制LED模块/台灯的主控板中使用。

```c++
#define WIFI_SSID       "DFSoftware"       //wifi名称
#define WIFI_PASSWD     "dfrobotsoftware"  //wifi密码
#define IOT_USERNAME    "obloquser"             //物联网账号
#define IOT_PASSWD      "20170307"             //物联网账号密码
```



**具体代码**

```c++
/*
  DFRobot_advancedReceive

 * LED attached from pin 13 to ground
  
 * Note: 
 on most Arduinos there is already an LED on the board attached to pin 13.

 *function:
 The IOT device receives the number 1, turns on the LED, otherwise turns off the LED

 created 2017/3/6
 by Jason

 */

#include <SoftwareSerial.h>
#include "Iot.h"

void * eventCb(uint8_t type, const char *data, uint16_t len);
Iot iot(eventCb);                          //当物联网既需要接收数据有需要发送数据的时候，需要传入回调函数

int tempData = 0;                          //缓存接收的整型数据

SoftwareSerial mySerial(10, 11);         // RX, TX

#define WIFI_SSID       "DFSoftware"       //wifi名称
#define WIFI_PASSWD     "dfrobotsoftware"  //wifi密码
#define IOT_USERNAME    "obloquser"             //物联网账号
#define IOT_PASSWD      "20170307"             //物联网账号密码

//连接状态回调函数
void * eventCb(uint8_t eventType, const char *data, uint16_t len)
{
}

void * myKey(const char *data, uint16_t len)
{
  tempData = atoi(data);                   //字符串转整型,浮点型，长整形...等atoi(),atof(),atol()...
  if(tempData == 1)
    digitalWrite(13,HIGH);                 //打开LED小灯
  if(tempData == 0)
    digitalWrite(13,LOW);                  //关闭LED小灯 
}

void setup(void)
{ 
  mySerial.begin(38400);
  pinMode(13,OUTPUT);
  iot.setup(mySerial, WIFI_SSID, WIFI_PASSWD, IOT_USERNAME, IOT_PASSWD);
  iot.subscribe("Button", myKey);
  iot.start();
}

void loop(void)
{
	iot.loop();
}
```



固件写好后，同发送端程序一样，向控制台灯/LED模块的主控板中烧入即可。



## 4. 结果展示

完成了网站注册、硬件搭建、固件烧录，现在一起来看这个项目的具体效果吧，准备好了吗？

1. 给所有硬件设备上电，如果之前因为烧录程序时已经保持上电状态，可以跳过这一步。
2. 为了确保系统能按照新固件的逻辑正确执行，按下主控板上的“Reset”按键重启主控板，完成固件重启。
3. 现在，尝试按下按键，是不是发现它可以无线操控远方的台灯了？



如果你没有“按键”实物，可以按照如下方法操作，给出虚拟“按键”信号。

### 在OBLOQ-web操作按键开关的方法



如果你并没有看到理想中的结果，检查一下是不是在下面的哪些步骤出了一些小问题：

* 程序中的wifi账号（SSID）、密码，物联网账号、密码、设备名称要保证书写的正确性
* 硬件连线要正确



## 5. 总结和故障处理

我们通过完成“按键联网控制小灯”这个简单的小项目，了解了如何利用OBLOQ模块制作联网设备，包括可以发送信息的“发送端设备”（按键），和接收信息并执行指令的“接送端设备”（台灯）。

步骤：

1. 在OBLOQ-web上创建项目
2. 搭设硬件
3. 分别在发送端和接收端的主控板上烧录对应的固件
4. 重启发送端和接收端主控板，并进行项目展示



如果顺利的话，项目到这里就算完成了。



然而，意外情况也是可能发生的。如果你遇到了不可判别原因的问题，可以尝试下面这段代码“检测故障”。

将这段代码编译后，烧入疑似故障的硬件相连的主控板里，运行固件，从串口打印出的消息中，我们能够查看OBLOQ模块连接wifi和物联网服务器的连接状态，比如OBLOQ模块有没有连接成功。



**样例代码**

```c++
/*
 DFRobot_callback

 *function:
 Through the callback function to check the Internet of things connected to the various states, 
 used to debug and modify the program

  //EVENT_CODE_WIFI               //0:ssid或密码错误,1:已连接,2:已断开
  //EVENT_CODE_SERVER             // 0:用户名或密码错误,1:已连接,2:已断开
  //EVENT_CODE_NEW_VERSION        // x:最新版本字符串信息
  //EVENT_CODE_UPGRADE_PERCENT    // 0~100 更新进度
  //EVENT_CODE_UART               // 0:异常,1:正常
  //EVENT_CODE_UNKNOWN            // 0:未定义错误

 created 2017/3/6
 by Jason
 */

#include <SoftwareSerial.h>
#include "Iot.h"

#define IOTDBG(...) if(1){Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
void * eventCb(uint8_t type, const char *data, uint16_t len);
Iot iot(eventCb);

SoftwareSerial mySerial(10, 11);           // RX, TX

#define WIFI_SSID       "DFSoftware"       //wifi名称
#define WIFI_PASSWD     "dfrobotsoftware"  //wifi密码
#define IOT_USERNAME    "obloquser"             //物联网账号
#define IOT_PASSWD      "20170307"             //物联网账号密码

unsigned long long sendTime = 0;
bool sendFlag = true;

void * eventCb(uint8_t eventType, const char *data, uint16_t len)
{
 
  IOTDBG(eventType,HEX);
  IOTDBG(data[0],HEX);
  switch(eventType){
    case EVENT_CODE_WIFI:
      if(data[0] == 0){
        IOTDBG("EVENT_CODE_WIFI: ssid or passwd error");
      }else if(data[0] == 1){
        IOTDBG("EVENT_CODE_WIFI: wifi connect");
      }else if(data[0] == 2){
        IOTDBG("EVENT_CODE_WIFI: wifi disconnect");
      }else{
        IOTDBG("EVENT_CODE_WIFI: wifi error arg");
      }
      break;
    case EVENT_CODE_SERVER:
      if(data[0] == 1){
        IOTDBG("EVENT_CODE_SERVER: server connect");
      }else if(data[0] == 2){
        IOTDBG("EVENT_CODE_SERVER: server disconnect");
      }else if(data[0] == 0){
        IOTDBG("EVENT_CODE_SERVER: username or passwd error");
      }else{
        IOTDBG("EVENT_CODE_SERVER: server error arg");
      }
      break;
    case EVENT_CODE_NEW_VERSION:
      IOTDBG("eventType: has new Version");
      IOTDBG(data);
      //iot.update();//默认升级到当前平台最新版//iot.update(PLATFORM_CURRENT,"0");
      break;
    case EVENT_CODE_UPGRADE_PERCENT:
      //uint8_t percent = (uint8_t)data[0];
      IOTDBG("eventType: upgrade percent:");
      IOTDBG((uint8_t)data[0]);
    case EVENT_CODE_UART:
      break;
    case EVENT_CODE_UNKNOWN:
      break;
    default:
      break;
  }
}

void * myTest(const char *data, uint16_t len)
{
  Serial.print("my Relay1 Recv:");
  Serial.println(data);
  Serial.print("len=");
  Serial.println(len);
  return NULL;
}

void setup(void)
{
  Serial.begin(115200);
  mySerial.begin(38400);
  while(!Serial);
  iot.setDbgSerial(Serial);
  iot.setup(mySerial, WIFI_SSID, WIFI_PASSWD, IOT_USERNAME, IOT_PASSWD);
  iot.subscribe("Relay1", myTest);          
  iot.start();
}

void loop(void)
{
  //两秒钟发送一次字符串，循环发送"down","up"
	if(millis() - sendTime > 2000){
		sendTime = millis();
		if(sendFlag){
            sendFlag = false;
			iot.publish("key1", "down");
		}else{
            sendFlag = true;
			iot.publish("key1", "up");
		}
	}
	iot.loop();
}
```



有经验的用户可能已经看出来了，这是回调函数的一种用法。

下表是这段代码打印出来的日志信息的具体含义。请大家核对。



| log消息                                    | 消息含义         |
| :--------------------------------------- | ------------ |
| **EVENT_CODE_WIFI: ssid or passwd error** | wifi账号或者密码错误 |
| **EVENT_CODE_WIFI: wifi connect **       | wifi连接成功     |
| **EVENT_CODE_WIFI: wifi disconnect**     | wifi断开连接     |
| **EVENT_CODE_SERVER: server connect**    | 服务器连接成功      |
| **EVENT_CODE_SERVER: server disconnect** | 服务器断开连接      |
| **EVENT_CODE_SERVER: username or passwd error** | 物联网账号或密码错误   |
| **eventType: has new Version**           | 有新固件         |
| **EVENT_CODE_WIFI: wifi connect： **      | wifi连接成功     |



**常见问题解决**



**Q：模块通电后，板子的信号灯不亮**

A：电源正负极接线有问题，检查接线



**Q：模块信号灯保持黄色常亮**

A：模块联网过程中卡在连接服务器的过程中，重启主控板。



**Q：物联网Web端没有就收到发送过来的消息**

A： 检查程序中的设备名称和物联网Web端的设备名称是否保持一致



**Q：为什么刚更新固件还会提示有新版固件**

A：







