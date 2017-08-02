#pragma once
class HardwareSerial;
class SoftwareSerial;

typedef void(*HandleJson)(JsonObject& data);
typedef void(*HandleRaw)(String& data);

class Obloq
{
private:
	HardwareSerial* _hardSerial = NULL;
	SoftwareSerial* _softSerial = NULL;
	HandleJson _handleJson = NULL;
	HandleRaw _handleRaw = NULL;
	int _wifiState = -1;
	int _interval = 500;
	bool _isSerialReady = false;
	unsigned long _time = 0;
	String _ssid = "";
	String _pwd = "";
	String _mac = "";
public:
	Obloq(HardwareSerial & hardSerial, String ssid, String pwd = "", String mac = "");
	Obloq(SoftwareSerial & softSerial, String ssid, String pwd = "", String mac = "");

	~Obloq();

public:
	void setHandleJson(HandleJson);
	void setHandleRaw(HandleRaw);
	bool isSerialReady();
	int  getWifiState();
	void ping();
	bool sendMsg(String);
	void update();
private:
	void receiveData(String&);
};

