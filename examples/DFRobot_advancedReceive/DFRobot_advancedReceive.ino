/*********************************************************************
* DFRobot_advancedReceive.
*
* Copyright (C)    2017   [DFRobot](http://www.dfrobot.com),
* This Library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Description:
* The IOT device receives the number 1, turns on the LED, otherwise turns off the LED
* 
* author  :  Jason
* version :  V1.0
* date    :  2017-03-06
**********************************************************************/


#include <SoftwareSerial.h>
#include "Iot.h"

void * eventCb(uint8_t type, const char *data, uint16_t len);
Iot iot(eventCb);                          //当物联网既需要接收数据有需要发送数据的时候，需要传入回调函数

int ledPin = 2;
int tempData = 0;                          //缓存接收的整型数据

SoftwareSerial mySerial(10, 11);         // RX, TX

#define WIFI_SSID       "DFSoftware"       //wifi名称
#define WIFI_PASSWD     "dfrobotsoftware"  //wifi密码
#define IOT_USERNAME    "test"             //物联网账号
#define IOT_PASSWD      "test"             //物联网账号密码

//连接状态回调函数
void * eventCb(uint8_t eventType, const char *data, uint16_t len)
{
}

void * myButton(const char *data, uint16_t len)
{
  tempData = atoi(data);                   //字符串转整型,浮点型，长整形...等atoi(),atof(),atol()...
  if(tempData == 1)
    digitalWrite(ledPin,HIGH);                 //打开LED小灯
  if(tempData == 0)
    digitalWrite(ledPin,LOW);                  //关闭LED小灯 
}

void setup(void)
{ 
  mySerial.begin(38400);
  pinMode(ledPin,OUTPUT);
  iot.setup(mySerial, WIFI_SSID, WIFI_PASSWD, IOT_USERNAME, IOT_PASSWD);
  iot.subscribe("Button", myButton);
  iot.start();
}

void loop(void)
{
	iot.loop();
}
