/*
  DFRobot_send

 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 *function:
 The IOT device sends a message when the key is pressed
 We can convert the integer number to a string and send it out through the IOT device

 created 2017/2/8
 by Jason
 */
 
#include <SoftwareSerial.h>
#include "Iot.h"

void * eventCb(uint8_t type, const char *data, uint16_t len);
Iot iot(eventCb);

#define WIFI_SSID       "DFSoftware"
#define WIFI_PASSWD     "dfrobotsoftware"
#define IOT_USERNAME    "test"
#define IOT_PASSWD      "test"

char *tempString = "test";
int  buttonState = 0;         // variable for reading the pushbutton status
const int buttonPin = 2;      // the number of the pushbutton pin
bool sendFlag = true;

SoftwareSerial mySerial(10, 11); // RX, TX

//连接状态回调函数
void * eventCb(uint8_t eventType, const char *data, uint16_t len)
{
}

void setup(void)
{
  Serial.begin(115200);
  mySerial.begin(38400);     //打开软串口，波特率必须是38400
  pinMode(buttonPin,INPUT);
  while(!Serial);
  iot.setup(mySerial, WIFI_SSID, WIFI_PASSWD, IOT_USERNAME, IOT_PASSWD);
  iot.start();
}

void loop(void)
{
  buttonState = digitalRead(buttonPin);
  //按键按下发送字符串
  if (buttonState == HIGH) {
    delay(10);
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) 
    {
      if(sendFlag){
         //第一种，将整型1转换成字符串发送
        itoa(1,tempString,10);
        iot.publish("key1", tempString); 
        sendFlag = false;
        return;
      }
      else
      {
        //第二种，直接发送字符串0
        iot.publish("key1", "0");
        sendFlag = true;
        return;
      }
       
    }  
  }
  iot.loop();
}
