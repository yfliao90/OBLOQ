/*
  DFRobot_simpleReceive

 * LED attached from pin 13 to ground
  
 * Note: 
 on most Arduinos there is already an LED on the board attached to pin 13.

 *function:
 The IOT device receives the number 1, turns on the LED,  receives the number 0, turns off the LED

 created 2017/3/6
 by Jason

 */

#include <SoftwareSerial.h>
#include "Iot.h"

Iot iot; 

SoftwareSerial mySerial(10, 11);         // RX, TX

#define WIFI_SSID       "DFSoftware"       //wifi名称
#define WIFI_PASSWD     "dfrobotsoftware"  //wifi密码
#define IOT_USERNAME    "test"             //物联网账号
#define IOT_PASSWD      "test"             //物联网账号密码


int tempData = 0;                          //缓存接收的整型数据

void * myKey(const char *data, uint16_t len)
{ 
  tempData = atoi(data);                  //将接收的字符串转整型,浮点型，长整形...等atoi(),atof(),atol()...
  if(tempData == 1)
    digitalWrite(13,HIGH); //打开LED小灯
  if(tempData == 0)
    digitalWrite(13,LOW); //关闭LED小灯 
}

void setup(void)
{ 
  mySerial.begin(38400);                  //波特率必须是38400
  pinMode(13,OUTPUT);
  iot.setup(mySerial, WIFI_SSID, WIFI_PASSWD, IOT_USERNAME, IOT_PASSWD);
  iot.subscribe("key1", myKey);           //监听物联网端设备名为key1发送过来的消息
  iot.start();
}

void loop(void)
{
	iot.loop();
}
