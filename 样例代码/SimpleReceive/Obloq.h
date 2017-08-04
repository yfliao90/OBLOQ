#pragma once
#define INTERVAL 1000

class HardwareSerial;
class SoftwareSerial;

typedef void(*HandleJson)(JsonObject& data);
typedef void(*HandleRaw)(const String& data);
class Obloq
{
private:
	HardwareSerial* _hardSerial = NULL;
	SoftwareSerial* _softSerial = NULL;
	HandleJson _handleJson = NULL;
	HandleRaw _handleRaw = NULL;
	int _wifiState = -1;
	int _interval = INTERVAL;
	bool _isSerialReady = false;
	unsigned long _time = 0;
	String _ssid = "";
	String _pwd = "";
	String _mac = "";
	String _url = "iot.dfrobot.com.cn";
	String _port= "1883";
	
public:
	Obloq(HardwareSerial & hardSerial, String ssid, String pwd = "", String mac = "");
	Obloq(SoftwareSerial & softSerial, String ssid, String pwd = "", String mac = "");

	~Obloq();

public:
	void setHandleJson(HandleJson);
	void setHandleRaw(HandleRaw);
	bool isSerialReady();
	int	 getWifiState();
	void ping();
	void update();
	void connect(const String& client_id,const String& iot_id,const String& iot_pwd);
	void connect(const String& client_id,const String& iot_id,const String& iot_pwd,const String& url,const String& port);
	void reconnect();
	void subscribe(const String& topic);
	void publish(const String& topic, const String &message);
	void disconnect();
private:
	void receiveData(String);
	bool sendMsg(const String&);
};

