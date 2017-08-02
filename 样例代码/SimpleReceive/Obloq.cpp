#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include "Obloq.h"

#define BufferSize 200

Obloq::Obloq(HardwareSerial & hardSerial, String ssid, String pwd, String mac)
{
	this->_hardSerial = &hardSerial;
	this->_ssid = ssid;
	this->_pwd = pwd;
	this->_mac = mac;
}

Obloq::Obloq(SoftwareSerial & softSerial, String ssid, String pwd, String mac)
{
	this->_softSerial = &softSerial;
	this->_ssid = ssid;
	this->_pwd = pwd;
	this->_mac = mac;
}

Obloq::~Obloq()
{
}

void Obloq::setHandleJson(HandleJson handle)
{
	this->_handleJson = handle;
}

void Obloq::setHandleRaw(HandleRaw handle)
{
	this->_handleRaw = handle;
}

bool Obloq::isSerialReady()
{
	return this->_isSerialReady;
}

int Obloq::getWifiState()
{
	return this->_wifiState;
}

void Obloq::ping()
{
	this->_isSerialReady = false;
	if (_hardSerial)
	{
		this->_hardSerial->print("{\"type\":\"system\",\"message\":\"PING!\"}\r");
	}
	else if (_softSerial)
	{
		this->_softSerial->print("{\"type\":\"system\",\"message\":\"PING!\"}\r");
	}
}

bool Obloq::sendMsg(String msg)
{
	if (this->_isSerialReady)
	{
		if (this->_hardSerial)
		{
			this->_hardSerial->print(msg + "\r");
		}
		else if (this->_softSerial)
		{
			this->_softSerial->print(msg + "\r");
		}
		else
		{
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

void Obloq::receiveData(String & data)
{
	if (this->_handleRaw)
	{ 
		this->_handleRaw(data);
	}
	if (data == "{\"type\":\"system\",\"message\":\"PONG!\"}")
	{
		this->_isSerialReady = true;
		if (this->_wifiState == 2)
		{
			return;
		}
		if (this->_mac == "")
		{
			this->sendMsg("{\"type\":\"system\",\"SSID\":\"" + this->_ssid + "\",\"PWD\":\"" + this->_pwd + "\"}\r");
		}
		else
		{
			this->sendMsg("{\"type\":\"system\",\"SSID\":\"" + this->_ssid + "\",\"PWD\":\"" + this->_pwd + "\",\"MAC\":\"" + this->_mac + "\"}\r");
		}
		return;
	}
	if (data == "{\"type\":\"system\",\"message\":\"Heartbeat!\"}")
	{
		return;
	}
	const char* jsonString = data.c_str();
	StaticJsonBuffer<BufferSize> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(jsonString);
	if (!root.success())
	{
		return;
	}

	String type = root["type"];
    if(type == "wifi")
        this->_wifiState = root["wifiState"];


	//switch (type)
	//{
        //case "wifi":
        //{
            //this->_wifiStat = root["wifiState"];
        //}
        //break;
        //default:break;
        ////case 2:break;
   
	//}
	if (this->_handleJson)
	{
		this->_handleJson(root);
	}
}

void Obloq::update()
{
	if ((millis() - this->_time) >= this->_interval)
	{
		this->_time = millis();
		if (!this->_isSerialReady)
		{
			this->ping();
		}
	}

	//从串口读数据
	if (_softSerial && _softSerial->available() > 0)
	{
		String data = _softSerial->readStringUntil('\r');
		while (true)
		{
			int length = data.length();
			int index = data.indexOf('\r');
			String subData = data.substring(0, index);
			this->receiveData(subData);
			if (index == -1 || length - index <= 1)
			{
				break;
			}
			else
			{
				subData = data.substring(index + 1, length);
				data = String(subData);
			}
		}
	}
	if (_hardSerial && _hardSerial->available() > 0)
	{
		String data = _hardSerial->readStringUntil('\r');
		while (true)
		{
			int length = data.length();
			int index = data.indexOf('\r');
			String subData = data.substring(0, index);
			this->receiveData(subData);
			if (index == -1 || length - index <= 1)
			{
				break;
			}
			else
			{
				subData = data.substring(index + 1, length);
				data = String(subData);
			}
		}
	}
}
