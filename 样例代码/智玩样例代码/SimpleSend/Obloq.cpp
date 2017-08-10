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
		this->_hardSerial->print(F("{\"type\":\"system\",\"message\":\"PING!\"}\r"));
	}
	else if (_softSerial)
	{
		this->_softSerial->print(F("{\"type\":\"system\",\"message\":\"PING!\"}\r"));
	}
}

bool Obloq::sendMsg(const String & msg)
{
	//Serial.print(F("Arduino send - > "));
	//Serial.println(msg);
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

void Obloq::receiveData(String data)
{
	if (this->_handleRaw)
	{ 
		this->_handleRaw(data);
	}
	if (data == F("{\"type\":\"system\",\"message\":\"PONG!\"}"))
	{
		this->_isSerialReady = true;
		if (this->_wifiState == 2)
		{
			return;
		}
		if (this->_mac == "")
		{
			this->sendMsg("{\"type\":\"system\",\"SSID\":\"" + this->_ssid + "\",\"PWD\":\"" + this->_pwd + "\"}");
		}
		else
		{
			this->sendMsg("{\"type\":\"system\",\"SSID\":\"" + this->_ssid + "\",\"PWD\":\"" + this->_pwd + "\",\"MAC\":\"" + this->_mac + "\"}");
		}
		return;
	}

	if (data == F("{\"type\":\"system\",\"message\":\"Heartbeat!\"}"))
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

void Obloq::connect(const String& client_id,const String& iot_id,const String& iot_pwd)
{
	this->connect(client_id,iot_id,iot_pwd,this->_url,this->_port);
}

void Obloq::connect(const String& client_id,const String& iot_id,const String& iot_pwd,const String& url,const String& port)
{
	String connect_msg = "{\"type\":\"mqtt\",\"method\":\"connect\",\"ClientId\":\"" + client_id + "\",\"Iot_id\":\"" + iot_id + "\",\"Iot_pwd\":\""+ iot_pwd + "\",\"url\":\"" + url + "\",\"port\":\"" + port + "\"}";
	this->sendMsg(connect_msg);
}

void Obloq::reconnect()
{
	String reconnect_msg = F("{\"type\":\"mqtt\",\"method\":\"reconnect\"}");
	this->sendMsg(reconnect_msg);
}

void Obloq::subscribe(const String& topic)
{
	String subscribe_msg = "{\"type\":\"mqtt\",\"method\":\"subscribe\",\"topic\":\"" + topic + "\"}";
	this->sendMsg(subscribe_msg);
}

void Obloq::publish(const String& topic, const String &message)
{
	String publish_msg = "{\"type\":\"mqtt\",\"method\":\"publish\",\"topic\":\"" + topic + "\",\"message\":\"" + message + "\"}";
	this->sendMsg(publish_msg);
}

void Obloq::disconnect()
{	
	String disconnect_msg = F("{\"type\":\"mqtt\",\"method\":\"disconnect\"}");
	this->sendMsg(disconnect_msg);	
}
