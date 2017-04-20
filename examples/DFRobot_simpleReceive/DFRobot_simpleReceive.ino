/*********************************************************************
* DFRobot_simpleReceive.
*
* Copyright (C)    2017   [DFRobot](http://www.dfrobot.com),
* This Library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Description:
* The IOT device receives the number 1, turns on the LED,  receives the number 0, turns off the LED
* LED attached from pin 2 to ground
* 
* author  :  Jason
* version :  V1.0
* date    :  2017-03-06
**********************************************************************/

#include <SoftwareSerial.h>
#include "Iot.h"

Iot iot; 

SoftwareSerial mySerial(10, 11);         // RX, TX

#define WIFI_SSID       "DFSoftware"       //wifi名称
#define WIFI_PASSWD     "dfrobotsoftware"  //wifi密码
#define IOT_USERNAME    "test"             //物联网账号
#define IOT_PASSWD      "test"             //物联网账号密码

int ledPin =  2;
int tempData = 0;                          //缓存接收的整型数据

void * myButton(const char *data, uint16_t len)
{ 
  tempData = atoi(data);                  //将接收的字符串转整型,浮点型，长整形...等atoi(),atof(),atol()...
  if(tempData == 1)
    digitalWrite(ledPin,HIGH); //打开LED小灯
  if(tempData == 0)
    digitalWrite(ledPin,LOW); //关闭LED小灯 
}

void setup(void)
{ 
  mySerial.begin(38400);                  //波特率必须是38400
  pinMode(ledPin,OUTPUT);
  iot.setup(mySerial, WIFI_SSID, WIFI_PASSWD, IOT_USERNAME, IOT_PASSWD);
  iot.subscribe("Button", myButton);           //监听物联网端设备名为key1发送过来的消息
  iot.start();
}

void loop(void)
{
	iot.loop();
}
