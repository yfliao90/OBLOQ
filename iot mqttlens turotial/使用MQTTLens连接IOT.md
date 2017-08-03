# 使用MQTTLens连接IOT

MQTTLens是一个免费MQTT工具，使用它可以连接到MQTT代理Broker（如iot.dfrobot.com.cn），进行订阅和发布到MQTT topic等操作。在Chrome的应用商城里就能搜到MQTTLens，请将它加入Chrome的扩展程序，并开启。

### 1. 建立MQTT连接

打开MQTTLens，点击左侧＋，新建一个MQTT连接。

<img src=".\img\01.png">

填写以下信息：

Connection Name: 任意名字

Host Name: iot.dfrobot.com.cn

Client ID: 任意**英文**字符串，注意此处填中文字符可能导致建立连接失败

username: 你的iot_id，登录本网站，在工作间可获取

password: 你的iot_pwd，登录本网站，在工作间可获取

<img src=".\img\02.png">

<img src=".\img\05.png">

红框1、2处填写iot.dfrobot.com.cn登录后显示在工作间左侧的信息，点击小“眼睛”可让iot_id, iot_pwd可视。

<img src=".\img\04.png">

最后点击create connection建立MQTT连接。



### 2. 订阅IOT设备，并向设备发送消息

现在我们演示如何使用MQTTLens订阅iot设备，如何向iot设备发送消息。

首先，在工作间新建一个设备。记录此设备id（也就是Topic），见上图红框3。

<img src=".\img\06.png">

在MQTTLens页面，在订阅（Subscribe）填入topic，点击“订阅”（Subscribe）按钮。

下方显示成功订阅的信息。

<img src=".\img\07.png">

在MQTTLens页面，在发送（Publish）填入topic，填入需要发送的消息（message），点击“发送”（Publish）按钮。

下方显示信息成功发送。

<img src=".\img\08.png">

您也可以到网站设备页面查看从MQTTLens发出的消息。

<img src=".\img\09.png">

<img src=".\img\10.png">