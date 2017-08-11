### OBLOQ API document

OBLOQ  is a medium that connect the communication between iot website and controller. When wifi is available, the controller is able to connect to Internet via OBLOQ, pulling/pushing messages or control instructions to the remote end.

This document is OBLOQ module's API description.

### Connect wifi

------

Set WiFi SSID and password to connect to a specific WiFi network.

Request: 
`{"type":"system","SSID":"test","PWD":"test"}`

Parameters: 

- `"type":"system"` Message type: system
- `"SSID":"test"` Set wifi SSID as **test**, according to your own situation, please choose correct wifi SSID
- `"PWD":"test"` Set wifi password as **test**, according to your own situation, please choose correct wifi password

Returns: 

1. JSON format
   - `{"type":"wifi","wifiState":1}` wifi connect state: Connecting wifi
   - `{"type":"wifi","wifiState":-1}` wifi connect state: wifi disconnect
   - `{"type":"wifi","wifiState":3} ` wifi connect state: wifi connect error, **check if SSID and password are correct**
   - `{"type":"wifi","wifiState":2,"ip":"192.168.0.001"}` wifi connect state: wifi connect successful, returns ip address of OBLOQ
2. normal strings separated by "|"
   - `|type|wifi|wifiState|1|`
   - `|type|wifi|wifiState|-1|`
   - `|type|wifi|wifiState|3|`
   - `|type|wifi|wifiState|2|ip|192.168.0,001|`

Arduino Sample: 

- `softSerial.print("{\"type\":\"system\",\"SSID\":\"test\",\"PWD\":\"test\"}\r")`

### Establish serial communication

------

The device communicate with OBLOQ through serial RX and TX. Because of different Baud rate, the sending message could get lost or become messy code. By sending this request, the device detects whether the device serial port and the OBLOQ serial port have been properly communicated. It is suggested sending this request to establish serial communication before sending other important message.

Request: 

- `{"type":"system","message":"PING!"}`

Parameters: 

- `"type":"system"` Message type: system
- `"message":"PING!"` Send message (string): PING!, means to establish serial communication.

Returns: 

1. JSON format
   - `{"type":"system","message":"PONG!"}`Returns system message: PONG!, means serial communication between the device and OBLOQ has been established.
2. normal strings separated by "|"
   - `|type|system|message|PONG!|`

Arduino Sample: 

- `softSerial.print("{\"type\":\"system\",\"message\":\"PING!\"}\r")`



### Establish heartbeat connection

------

The device serial port and OBLOQ serial port may be disconnected after a long time without data communication. By this request, the data exchange between the device and the OBLOQ can be maintained at a certain frequency.

Request: 

- `{"type":"system","heartbeat":5000}`

Parameters: 

- `"type":"system"` Message type: system
- `"heartbeat":5000` Set sending frequency (millisecond) of Heartbeat message. 5000 means every 5 seconds OBLOQ will send a message to the device via serial. 

Returns: 

1. JSON format
   - `{"type":"system","message":"Heartbeat!"}`Returns system message at certain frequency: Heartbeat!
2. normal strings separated by "|"
   - `|type|system|message|Heartbeat!|`

Arduino Sample: 

- `softSerial.print("{\"type\":\"system\",\"heartbeat\":-5000}\r")`



### Get current return data type

------

After OBLOQ receives this request, it returns current return data type. there are two return data type: JSON strings or normal strings separated by "|"

Request: 

- `{"type":"system","message":"getReturnType"}`

Parameters: 

- `"type":"system"` Message type: system
- `"message":"getReturnType"` Get current OBLOQ return data type

Returns: 

1. JSON format
   - `{"type":"returnType","message":"json"}` Data returns from serial port: JSON strings
2. normal strings separated by "|"
   - `|type|returnType|message|string|` Data returns from serial port: normal strings separated by "|"

Arduino Sample: 

- `softSerial.print("{\"type\":\"system\",\"message\":\"getReturnType\"}\r")`



### Set return data type

------

After OBLOQ receives this request, there are two returned data type: JSON strings or normal strings separated by "|"

Request: 

1. Set return format: JSON strings
   - `{"type":"system","message":"setReturnType","returnType":"json"}`
2. Set return format: normal strings separated by "|"
   - `{"type":"system","message":"setReturnType","returnType":"string"}`

Parameters: 

- `"type":"system"` Message type: system
- `"message":"setReturnType"` Set current return format
- `"returnType":"string"` Set return type to normal strings separated by "|"
- `"returnType":"json"`Set return type to JSON strings

Returns: 

1. JSON format
   - `{"type":"returnType","message":"json"}` Data returns from serial port: JSON strings
2. normal strings separated by "|"
   - `|type|returnType|message|string|` Data returns from serial port: normal strings separated by "|"

Arduino Sample: 

- ```cpp
  softSerial.print("{\"type\":\"system\",\"message\":\"setReturnType\",\"returnType\":\"string\"}\r")
  ```

### Reconnect wifi

------

Reconnect wifi that connected last time

Request: 

- `{"type":"system","message":"CONNECTWIFI"}`

Parameters: 

- `"type":"system"` Message type: system
- `"message":"CONNECTWIFI"` Send message (string): CONNECTWIFI, means to reconnect wifi

Returns: 

1. JSON format
   - `{"type":"wifi","wifiState":1}` wifi connect state: Connecting wifi
   - `{"type":"wifi","wifiState":-1}` wifi connect state: wifi disconnect
   - `{"type":"wifi","wifiState":3} ` wifi connect state: wifi connect error, **check if SSID and password are correct**
   - `{"type":"wifi","wifiState":2,"ip":"192.168.0.001"}` wifi connect state: wifi connect successful, returns ip address of OBLOQ
2. normal strings separated by "|"
   - `|type|wifi|wifiState|1|`
   - `|type|wifi|wifiState|-1|`
   - `|type|wifi|wifiState|3|`
   - `|type|wifi|wifiState|2|ip|192.168.0,001|`

Arduino Sample: 

- `softSerial.print("{\"type\":\"system\",\"message\":\"CONNECTWIFI\"}\r")`

### Establish MQTT connection

------

Establish MQTT connection, default host: iot.dfrobot.com.cn

Request: 

1. Connect to default host: iot.dfrobot.com.cn

   - `{"type":"mqtt","method":"connect","ClientId":"test","Iot_id":"test","Iot_pwd":"test"}`

2. Connect to customized host

   - ```cpp
     {"type":"mqtt","method":"connect","ClientId":"SkxprkFyE-","Iot_id":"r1qHJFJ4Z","Iot_pwd":"SylqH1Y1VZ","url":"iot.dfrobot.com.cn","port":"1883"}
     ```

Parameters: 

- `"type":"mqtt"` Message type: MQTT
- `"method":"connect"` Request method: connect
- `"ClientId":"test"` ClientId, it can be arbitrary English words, it does not support Chinese
- `"Iot_id":"test"` Set IOT host id, [how to get Iot_id?](iot.dfrobot.com.cn)
- `"Iot_pwd":"test"`Set IOT host password,[how to get Iot_pwd?](iot.dfrobot.com.cn)
- `"url":"iot.dfrobot.com.cn"`Optional parameter, set customized IOT host, default host: [iot.dfrobot.com.cn](http://iot.dfrobot.com.cn/ "iot.dfrobot.com.cn")
- `"port":"1883"` Optional parameter, the port of the customized IOT host

Returns: 

1. JSON format
   - `{"type":"mqtt","mqttState":1}` MQTT connect state: connect successful
   - `{"type":"mqtt","mqttState":-1,"reason":"reason"}` MQTT connect state: connection failed, returns error code
2. normal strings separated by "|"
   - `|type|mqtt|mqttState|1|`
   - `|type|mqtt|mqttState|-1|reason|reason|`

错误码描述：

| reason | Description                              |
| ------ | ---------------------------------------- |
| -5     | There is no broker listening at the specified IP Address and Port |
| -4     | The response from the broker was not a CONNACK as required by the protocol |
| -3     | DNS Lookup failed                        |
| -2     | Timeout waiting for a CONNACK from the broker |
| -1     | Timeout trying to send the Connect message |
| 0      | No errors. Note: This will not trigger a failure callback. |
| 1      | The broker is not a 3.1.1 MQTT broker.   |
| 2      | The specified ClientID was rejected by the broker. (See mqtt.Client()) |
| 3      | The server is unavailable.               |
| 4      | The broker refused the specified username or password. |
| 5      | The username is not authorized.          |

Arduino Sample: 

- ```cpp
  softSerial.print("{\"type\":"mqtt",\"method\":\"connect\",\"ClientId\":\"test\",\"Iot_id\":\"test\",\"Iot_pwd\":\"test\"}\r")
  ```

### Subscribe a device

------

Among countless IOT devices, this request can receive messages from a specified device (device id: topic). 

Request: 

- `{"type":"mqtt","method":"subscribe","topic":"BJpHJt1VW"}`

Parameters: 

- `"type":"mqtt"` Message type: MQTT
- `"method":"subscribe"` Request method: subscribe
- `"topic":"BJpHJt1VW"` Set device to subscribe, [how to get device topic?](iot.dfrobot.com.cn)

Returns: 

1. JSON format
   - `{"type":"mqtt","subscribe":1}` only occurs after  successful subscribing
   - `{"type":"mqtt","topic":"BJpHJt1VW","message":"test"}` Return messages from the subscribed device
     - `"topic":"BJpHJt1VW"` Subscribed device topic
     - `"message":"test"` Message received content: test
2. normal strings separated by "|"
   - `|type|mqtt|subscribe|1|`
   - `|type|mqtt|topic|BJpHJt1VW|message|test|`

Arduino Sample: 

- `softSerial.print("{\"type\":\"mqtt\",\"method\":\"subscribe\",\"topic\":\"BJpHJt1VW\"}\r")`

### Publish message

------

Among countless IOT devices, this request can send messages to a specified device (device id: topic). 

Request: 

- `{"type":"mqtt","method":"publish","topic":"BJpHJt1VW","message":"test"}`

Parameters: 

- `"type":"mqtt"` Message type: MQTT
- `"method":"publish"` Request method: publish message
- `"topic":"BJpHJt1VW"` Set device to publish, [how to get device topic?](iot.dfrobot.com.cn)
- `"message":"test"` Message content: test

Returns: 

1. JSON format
   - `{"type":"mqtt","publish":1}` only occurs after successful publishing
2. normal strings separated by "|"
   - `|type|mqtt|publish|1|`

Arduino Sample: 

```cpp
- softSerial.print("{\"type\":\"mqtt\",\"method\":\"publish\",\"topic\":\"BJpHJt1VW\",\"message\":\"test\"}\r")
```



### Reconnect MQTT

------

Reconnect MQTT. The premise is that a mqtt connection must have been established through Iot_id and Iot_pwd.

Request: 

- `{"type":"mqtt","method":"reconnect"}`

Parameters: 

- `"type":"mqtt"` Message type: MQTT
- `"method":"reconnect"` Request method: reconnect MQTT

Returns: 

1. JSON format
   - `{"type":"mqtt","mqttState":1}` only occurs after MQTT connect successfully 
2. normal strings separated by "|"
   - `|type|mqtt|mqttState|1|`

Arduino Sample: 

- `softSerial.print("{\"type\":\"mqtt\",\"method\":\"reconnect\"}\r")`



### Disconnect MQTT

------

Disconnect MQTT. The premise is that a mqtt connection must have been established through Iot_id and Iot_pwd.

Request: 

- `{"type":"mqtt","method":"disconnect"}`

Parameters: 

- `"type":"mqtt"` Message type: MQTT
- `"method":"disconnect"` Request method: disconnect MQTT

Returns: 

1. JSON format
   - `{"type":"mqtt","mqttState":"-1"}` only occurs after MQTT disconnect successfully 
2. normal strings separated by "|"
   - `|type|mqtt|mqttState|-1|`

Arduino Sample: 

- `softSerial.print("{\"type\":\"mqtt\",\"mqttState\":\"-1\"}\r")`