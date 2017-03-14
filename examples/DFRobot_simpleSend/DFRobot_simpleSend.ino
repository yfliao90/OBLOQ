/*
 DFRobot_simpleSend

 *Function:
 Send data once every two seconds to the Internet of Things

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
char *tempString   =    "string";
unsigned long sendTime = 0;
unsigned long number = 0;

void setup(void)
{
  mySerial.begin(38400);                  //波特率必须是38400
  iot.setup(mySerial, WIFI_SSID, WIFI_PASSWD, IOT_USERNAME, IOT_PASSWD);
  iot.start();
}

void loop(void)
{
  //每两秒钟发送一次字符串到物联网
  if(millis() - sendTime >= 2000){
    sendTime = millis();
    ultoa(number++,tempString,10);         //将无符号长整型数据转换成字符串
    iot.publish("key1", tempString);       //发送字符串到物联网,key1是物联网设备名
    //iot.publish("key1", "down");         //直接发送字符串
  }
  iot.loop();
}
