#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "Obloq.h"

StaticJsonBuffer<200> jsonBuffer;

bool sendFlag = true;
String MQTTCONNECT = "{\"type\":\"mqtt\",\"method\":\"connect\",\"ClientId\":\"SkxprkFyE-\",\"Iot_id\":\"r1qHJFJ4Z\",\"Iot_pwd\":\"SylqH1Y1VZ\"}";
String SUBSCRIBE   = "{\"type\":\"mqtt\",\"method\":\"subscribe\",\"topic\":\"BJpHJt1VW\"}";

Obloq olq(Serial, "DFSoftware", "dfrobotsoftware");

void handleRaw(String& data)
{
    //Serial.println(data);   //串口打印返回的数据
}
void handleJson(JsonObject& data)
{
    static int message = 0;
    if(strcmp(data["topic"],"BJpHJt1VW") == 0)
    {
        message = data["message"];
    }
    switch(message)
    {
        case 0: digitalWrite(13,LOW);break;
        case 1: digitalWrite(13,HIGH) ;break;
        default:break;
    }
}

void setup()
{
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
      olq.sendMsg(MQTTCONNECT);
      olq.sendMsg(SUBSCRIBE);
    }
}
