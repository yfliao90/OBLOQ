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
* Through the callback function to check the Internet of things connected to the various states, 
* used to debug and modify the program
* 
*  //EVENT_CODE_WIFI               //0:ssid或密码错误,1:已连接,2:已断开
*  //EVENT_CODE_SERVER             // 0:用户名或密码错误,1:已连接,2:已断开
*  //EVENT_CODE_NEW_VERSION        // x:最新版本字符串信息
*  //EVENT_CODE_UPGRADE_PERCENT    // 0~100 更新进度
*  //EVENT_CODE_UART               // 0:异常,1:正常
*  //EVENT_CODE_UNKNOWN            // 0:未定义错误
*  
* author  :  Jason
* version :  V1.0
* date    :  2017-03-06
**********************************************************************/

#include <SoftwareSerial.h>
#include "Iot.h"

#define IOTDBG(...) if(1){Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
void * eventCb(uint8_t type, const char *data, uint16_t len);
Iot iot(eventCb);

SoftwareSerial mySerial(10, 11);         // RX, TX

#define WIFI_SSID       "DFSoftware"       //wifi名称
#define WIFI_PASSWD     "dfrobotsoftware"  //wifi密码
#define IOT_USERNAME    "test"             //物联网账号
#define IOT_PASSWD      "test"             //物联网账号密码

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
  Serial.print("my Button Recv:");
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
  iot.subscribe("Button", myTest);          
  iot.start();
}

void loop(void)
{
  //两秒钟发送一次字符串，循环发送"down","up"
	if(millis() - sendTime > 2000){
		sendTime = millis();
		if(sendFlag){
      sendFlag = false;
			iot.publish("Button", "down");
		}else{
      sendFlag = true;
			iot.publish("Button", "up");
		}
	}
	iot.loop();
}
