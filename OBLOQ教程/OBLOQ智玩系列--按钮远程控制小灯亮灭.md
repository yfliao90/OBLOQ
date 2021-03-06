# OBLOQ智玩系列-按钮远程控制小灯亮灭

## 0. 项目简介

这次的项目将用OBLOQ实现一个按键远程无线控制一台台灯的亮灭。

在这里，按键是发送端，台灯是接收端。

发送端OBLOQ模块将按键的状态发送到IOT网站，IOT网站接着将接收到的按键状态转发到接收端OBLOQ模块，最后接收端主控接过信息，通过判断按键状态来控制台灯亮灭。

## 1. 在IOT网站上创建项目

第一步，在IOT网站里新建一个按键设备，用来接收发送端信息，并将信息转发到接收端。

**创建设备：新设备1**

登录OBLOQ官网，注册账号。进入工作间。

观察到左侧的用户信息，点击**眼睛**图标可以将隐藏的Iot_id和Iot_pwd显示出来。

<img src="./img/009.png" width="300px">


在右侧工作界面中，新建一个设备，默认名字“新设备1”。

观察到新设备自动生成一个Topic，这是设备唯一可识别码。

<img src="./img/003.png" width="300px">


为了稍后编程需要，记下这些关键信息，例如：

- 物联网Iot_id:  "r1qHJFJ4Z"
- 物联网Iot_pwd: "SylqH1Y1VZ"
- client_id: "ryHxUYFeW"
- topic: "rkX4LYFeZ"




## 2. 硬件搭建##

接着用杜邦线连接各硬件接口，搭建硬件设备，在wifi环境给主控板烧录相应的程序，就能直接使用了。

**硬件清单**

| 名称                         | 数量              |
| -------------------------- | --------------- |
| OBLOQ模块                    | $$1$$ ，推荐 $$2$$ |
| 主控板（如：Arduino UNO控制板*）     | $$1$$ ，推荐 $$2$$ |
| 按键模块（如选择使用虚拟按键，可省略此项）**    | $$0$$ ，推荐 $$1$$ |
| LED模块（或用一个台灯和一个继电器模块代替）*** | $$1$$           |
| 杜邦线                        | 若干              |

* 关于主控板，这里挑选Arduino UNO及其扩展板作为例子讲解，其他兼容的主控板也可以使用。
* 如果只有1套OBLOQ模块和主控，这套系统也是能实现的。此时用虚拟按键控制远端台灯的亮灭的方式，解决没有多余的主控和OBLOQ模块的问题，这时候我们需要在IOT网站网站中创建一个虚拟按键。
* 台灯和LED模块都可以作为被操控设备，任选一个即可。如果想适用于实际生活，推荐使用台灯。




**发送端**

<img src="./img/024.png" width="800px">

实际连线情况（主控板需供电）：

<img src="./img/023.jpg"  width="800px">



**接收端**（此例使用LED模块）

<img src="./img/017.png"  width="800px">

实际连线情况（主控板需供电）：

<img src="./img/022.jpg"  width="800px">

使用**LED模块**作为接收端的硬件搭建完成。

如果想使用**台灯**作为接收端，需要再加一个**继电器模块**，接线方式需要变动。正常台灯的输入电压是220V，需要直接接到家庭用电的插座上才能正常工作，而主控板一般只能输出5V低压，不能使台灯正常工作。继电器模块能让Arduino UNO的弱电（5V）控制强电（220V）。**我们需要改接电线，由于该过程可能接触220V电压（危险），请大家做好绝缘保护，没有改装经验的人请不要尝试。**下面是具体操作方法。



**(附录)使用台灯作为接收端**

**1.将继电器连接到台灯的输入端口**

台灯的插座有两根线，一根火线一根地线。将其中一根线剪断，串连接上继电器，一端接入NO（常开），一端接入COM（公共端）。如果不想破坏台灯的电线，也可以找根平时不用的带插头的线来改装，如下图：

<img src="./img/021.png"  width="800px">



实物连接图：

<img src="./img/020.jpg"  width="800px">

<img src="./img/019.jpg"  width="800px">



**2. 将烧录好接收端程序的Arduino UNO连接到继电器，连线的引脚和连接LED的示意图相同**

<img src="./img/018.jpg"  width="800px">

<img src="./img/017.png"  width="800px">

**3. 接通台灯电源，插入台灯插座即可。给Arduino UNO供电，可以USB供电，也可以电池供电。**

<img src="./img/016.jpg"  width="800px">



## 3. 在主控板上烧录控制程序

现在，我们要制作两个控制程序，分别控制按键和台灯（或LED模块），并烧录进对应主控板中。



**在写控制程序之前，你可能需要知道Arduino UNO程序烧录的基本操作步骤**

1. 查看 [Arduino中文手册](http://www.dfrobot.com.cn/images/upload/File/20150129112804mdoxsq.pdf) 第1-13页，下载安装Arduino IDE和驱动，配置板卡和串口.
2. 下载DFRobot的 OBLOQ库文件，并在Arduino内部加载。（[OBLOQ库文件下载地址](https://github.com/DFRobot/Obloq.git), [Arduino加载库文件的方法](https://jingyan.baidu.com/article/8065f87f904f4c2331249881.html)）
3. 在Arduino IDE内输入程序源码，进行验证（也就是编译），最后下载（也就是烧录）。




### 3.1 发送端程序——按键（Button）

这段内容适用于有两套OBLOQ模块和主控的用户，如果您只有一套，小节末尾段落另行说明实现方法。



**程序逻辑**

发送端的程序主要是循环检测事件是否触发，一旦事件触发，就向IOT网站发送信号，IOT网站则向相应联网设备转发此信号。

本例中，程序会反复检测按键是否按下，当检测到按键第一次按下的时候，OBLOQ系统向设备Lamp发送消息”1“，当检测到按键第二次按下的时候，向设备Lamp发送消息”0“，如此循环检测执行。



**特别注意！**

实际程序中必须修改程序中以下信息才能正常使用。

```c++
const String ssid = "testssid";          //wifi名称
const String password = "testpwd";       //wifi密码
const String client_id = "47dd01354ee3b0a7";   //物联网client_id
const String iot_id = "r1qHJFJ4Z";     	 //物联网iot_id
const String iot_pwd = "SylqH1Y1VZ";     //物联网iot_pwd
const String topic = "BJpHJt1VW";        //物联网设备topic

...
...
Obloq olq(softSerial, ssid, password);  //生成Obloq对象
olq.connect(client_id,iot_id,iot_pwd);  //连接MQTT
olq.subscribe(topic);                   //注册设备
olq.publish(topic, "1");                //发送消息
...
```



**具体代码**

```c++
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "Obloq.h"

//MQTT连接相关参数
const String ssid = "testssid";   
const String password = "testpwd";     
const String client_id = "47dd01354ee3b0a7";
const String iot_id = "r1qHJFJ4Z";
const String iot_pwd = "SylqH1Y1VZ";
const String topic = "BJpHJt1VW";

//按键连接引脚
int  buttonPin = 2;
int  sendPingFlag = true;

//按键防抖动的相关参数
int normalVoltage = 0;                     
bool isPress = false;
bool isClick = false;
bool hasPress = false;
bool currentState = false;
unsigned long currentTime = 0;
bool keyFlag  = true;

SoftwareSerial softSerial(10, 11);         // RX, TX
Obloq olq(softSerial, ssid, password);

void handleRaw(String& data)
{
    //Serial.println(data);   //串口打印返回的数据
}
void handleJson(JsonObject& data)
{
}

void setup()
{
    //Serial.begin(9600);
    softSerial.begin(9600);
    pinMode(buttonPin,INPUT);
    //olq.setHandleRaw(handleRaw);
    //olq.setHandleJson(handleJson);
}
void loop()
{
    olq.update();
    if(sendPingFlag && olq.getWifiState()==2)
    {
      sendPingFlag = false;
      olq.connect(client_id,iot_id,iot_pwd);
      delay(1000);
      olq.subscribe(topic);
    }
    keyScan();
    if(isClick)
    {
        if(keyFlag){
        olq.publish(topic, "1");    
        keyFlag = false;
        }
        else{
        olq.publish(topic, "0");             
        keyFlag = true;
        }    
    }
}

//按键防抖扫描
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



程序烧录完成后，为了确保系统能按照新固件的逻辑正确执行，按下主控板上的“Reset”按键重启主控板，完成固件重启。




**没有实物按键模块，使用虚拟按键替代**

如果只有一套OBLOQ模块和主控，按键功能依旧能够通过在IOT网站虚拟发送按键开关的信息实现。

如果你没有“按键”实物，可以按照如下方法操作，给出虚拟“按键”信号。

1. 登录IOT网站，在设备列表中找到按键设备：

<img src=".\img\003.png" width="300px">



2. 点击进入，可查看**设备详细信息**：

<img src=".\img\010.png" width="800px">



3. 发送消息。在“发送新消息”栏中输入消息内容，再点击后面的**发送**按钮即可。本例中，发送消息“0”关闭台灯，发送消息“1”打开台灯。

<img src=".\img\011.png" width="800px">

4. 查看消息是否发送成功。在点击发送消息按钮后，如果发送成功，消息列表里面将出现刚才输入的消息。完成接收端搭建后，此命令可让远方的LED灯（或台灯）打开或关闭。

<img src=".\img\012.png" width="800px">



### 3.2 接收端程序——台灯/LED模块（Lamp）



**程序逻辑**

接收端的程序主要是循环检测监听的设备是否发出信息，一旦该设备发出信息，将根据信息内容执行相应任务。

本例中，设备Lamp会反复判断是否接到来自“新设备1”的新消息。当接收到数字”1“，打开LED；若接收到数字”0“，则关闭LED。如此循环检测执行。



**特别注意！**

实际程序中必须修改程序中以下信息才能正常使用。

```c++
const String ssid = "testssid";          //wifi名称
const String password = "testpwd";       //wifi密码
const String client_id = "47dd01354ee3b0a7";   //物联网client_id
const String iot_id = "r1qHJFJ4Z";     	 //物联网iot_id
const String iot_pwd = "SylqH1Y1VZ";     //物联网iot_pwd
const String topic = "BJpHJt1VW";        //物联网设备topic

...
...
Obloq olq(softSerial, ssid, password);  //生成Obloq对象
olq.connect(client_id,iot_id,iot_pwd);  //连接MQTT
olq.subscribe(topic);                   //注册设备
...
...
```



**具体代码**

```c++
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "Obloq.h"

//MQTT连接相关参数
const String ssid = "testssid";     
const String password = "testpwd";  
const String client_id = "47dd01354ee3b0a7";
const String iot_id = "r1qHJFJ4Z";
const String iot_pwd = "SylqH1Y1VZ";
const String topic = "BJpHJt1VW";

 //led小灯引脚
int ledPin = 2;
int  sendPingFlag = true;                           
SoftwareSerial softSerial(10, 11);     // RX, TX
Obloq olq(softSerial, ssid, password);

void handleRaw(String& data)
{
    //Serial.println(data);   //串口打印返回的数据
}
void handleJson(JsonObject& data)
{
    int message = 0;
    char *deviceTopic = topic.c_str();
    if(strcmp(data["topic"],deviceTopic) == 0)
    {
        message = data["message"];
        switch(message)
        {
            case 0: digitalWrite(ledPin,LOW);break;
            case 1: digitalWrite(ledPin,HIGH) ;break;
            default:break;
        }
    }  
}

void setup()
{
    //Serial.begin(9600);
    softSerial.begin(9600);
    pinMode(ledPin,OUTPUT);
    //olq.setHandleRaw(handleRaw);
    olq.setHandleJson(handleJson);
}
void loop()
{
    olq.update();
    if(sendPingFlag && olq.getWifiState()==2)
    {
      sendPingFlag = false;
      olq.connect(client_id,iot_id,iot_pwd);
      delay(1000);
      olq.subscribe(topic);
    }
}
```



固件写好后，同发送端程序一样，向控制台灯/LED模块的主控板中烧入即可。程序烧录完成后，为了确保系统能按照新固件的逻辑正确执行，按下主控板上的“Reset”按键重启主控板。



## 4. 结果展示

现在看下这个项目的具体效果吧。

1. 保持硬件设备上电状态。OBLOQ模块正常工作状态下LED指示灯显示绿色常亮。
2. 现在，尝试按下按键（或虚拟按键），远方的台灯就能被无线操控亮灭了。




下图是安装好的按键和台灯。

<img src=".\img\013.jpg" width="800px">

按下按键，打开台灯。

<img src=".\img\014.jpg" width="800px">

再次按下按键，关闭台灯。

<img src=".\img\015.jpg" width="800px">



如果你并没有看到理想中的结果，检查一下是不是在下面的哪些步骤出了一些小问题：

* 程序中的wifi账号（SSID）、wifi密码，iot_user、iot_pwd、topic要保证书写的正确性；


- 硬件连线要正确，尤其注意实际使用的引脚和程序是否能对应；
- 当OBLOQ信号灯保持红色，或者黄色不变的时候，复位UNO开发板。




## 5. 总结和拓展
我们通过完成“按键联网控制小灯”这个简单的小项目，了解了如何利用OBLOQ模块制作联网设备，包括可以发送信息的“发送端设备”（按键），和接收信息并执行指令的“接送端设备”（LED/台灯）。

步骤总结：

1. 在IOT网站上创建项目
2. 搭设硬件
3. 分别在发送端和接收端的主控板上烧录对应的固件
4. 重启发送端和接收端主控板，并进行项目展示



如果顺利的话，项目到这里就算完成了。


然而，如果发生意外情况，而且不可判别问题原因，可以尝试下面这段代码“检测故障”。


下面是演示的样例代码，将这段代码编译后，烧入疑似故障的硬件相连的主控板里，重启运行固件，查看串口打印出的消息，我们能够查看OBLOQ模块连接wifi和物联网服务器的连接状态。具体返回消息的消息可以对比API接口文档查看。

**样例代码**

```c++
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "Obloq.h"

const String ssid = "wifissid";
const String password = "wifipwd";
const String client_id = "47dd01354ee3b0a7";
const String iot_id = "r1qHJFJ4Z";
const String iot_pwd = "SylqH1Y1VZ";
const String topic = "BJpHJt1VW";

SoftwareSerial softSerial(10, 11);         // RX, TX

bool sendFlag = true;
int actionMode = 0;
int disconnectCount = 0;

Obloq olq(softSerial, ssid, password);

void handleRaw(const String& data)
{
    Serial.println(data);   //串口打印返回的数据
}
void handleJson(JsonObject& data)
{
    if(strcmp(data["type"],"mqtt") == 0)
    {
       if(data["mqttState"] == 1)
        {      
            actionMode = 1;     
            return;
        }
        else if(data["mqttState"] == -1)
        {
            disconnectCount++;
            if(disconnectCount == 1)
                actionMode = 4; 
        }
        else if(data["subscribe"] == 1)
        {
            actionMode = 2;
            return;
        }
        else if(data["message"] != "")
        {
            actionMode = 3;
            return;
        }           
    }
    return;
}

void setup()
{
    softSerial.begin(9600);
    Serial.begin(9600);
    olq.setHandleRaw(handleRaw);
    olq.setHandleJson(handleJson);
}
void loop()
{
    olq.update();
    if(sendFlag && olq.getWifiState()==2)
    {
      sendFlag = false;
      olq.connect(client_id,iot_id,iot_pwd);
    }
    switch(actionMode)
    {
        case 1: olq.subscribe(topic); actionMode = 0; break;
        case 2: olq.publish(topic,"hello world"); actionMode = 0; break;
        case 3: olq.disconnect(); actionMode = 0; break;
        case 4: olq.reconnect(); actionMode = 0; break;
        default: actionMode = 0; break;
    }
}
```


**附：如何查看串口打印消息**

打开Arduino IDE 的串口工具：

<img src=".\img\025.png" width=600px>



查看调试信息：

<img src=".\img\026.png" width=600px>
