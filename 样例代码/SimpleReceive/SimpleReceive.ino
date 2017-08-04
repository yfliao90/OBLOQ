#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "Obloq.h"

bool sendFlag = true;
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
        case 1: digitalWrite(13,HIGH);break;
        case 2: digitalWrite(13,LOW) ;break;
        default:break;
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
      olq.connect("SkxprkFyE-","r1qHJFJ4Z","SylqH1Y1VZ");
      olq.subscribe("BJpHJt1VW");
    }
}
