### OBLOQ Mqtt API说明文档



### 连接wifi

------------
设置wifi账号和密码，连接特定的wifi网络。

请求方式：
`{"type":"system","SSID":"test","PWD":"test"}`

参数：
- `"type":"system"` 发送消息类型：系统消息
- `"SSID":"test"`设置wifi账号为**test**,因为所处环境不同，wifi账号需要自行修改
- `"PWD":"test"`设置wifi密码为**test**,因为所处环境不同，wifi密码需要自行修改

返回值：
1. JSON格式
  - `{"type":"wifi","wifiState":1}` wifi连接状态：正在连接wifi
  - `{"type":"wifi","wifiState":-1}` wifi连接状态：wifi断开连接
  - `{"type":"wifi","wifiState":3} ` wifi连接状态：wifi连接失败，**检查SSID和PWD是否输入正确**
  - `{"type":"wifi","wifiState":2,"ip":"192.168.0.001"}` wifi连接状态：wifi连接成功，返回OBLOQ的ip地址
2. 普通"|"分割的字符串
  - `|type|wifi|wifiState|1|`
  - `|type|wifi|wifiState|-1|`
  - `|type|wifi|wifiState|3|`
  - `|type|wifi|wifiState|2|ip|192.168.0,001|`

Arduino样例：
- `softSerial.print("{\"type\":\"system\",\"SSID\":\"test\",\"PWD\":\"test\"}\r")`


### 建立串口通信
------------
设备通过串口RX，TX和OBLOQ进行通信，因为波特率的不同，发送的数据可能会乱码或者丢包，设备通过发送这条指令，检测设备串口和OBLOQ串口是否已经正常通信。建议设备通信之前先通过这条指令检查串口连接状态。

请求方式：
- `{"type":"system","message":"PING!"}`

参数：
- `"type":"system"` 发送消息类型：系统消息
- `"message":"PING!"` 发送消息字符串：PING!，表示建立串口通信

返回值：
1. JSON格式
  - `{"type":"system","message":"PONG!"}`返回系统消息PONG!,表示设备串口和OBLOQ串口已经正常通信
2. 普通"|"分割的字符串
  - `|type|system|message|PONG!|`

Arduino样例：
- `softSerial.print("{\"type\":\"system\",\"message\":\"PING!\"}\r")`


### 建立心跳连接
------------
设备串口和OBLOQ串口在长时间不进行数据通信后可能会断开连接，通过这条指令可以让设备和OBLOQ之间保持一定频率的数据交换。

请求方式：
- `{"type":"system","heartbeat":5000}`

参数：
- `"type":"system"` 发送消息类型：系统消息
- `"heartbeat":5000` 设置心跳数据发送频率，单位：毫秒，5000表示5秒钟OBLOQ向通信的的设备发送一次数据

返回值：
1. JSON格式
  - `{"type":"system","message":"Heartbeat!"}`一定频率返回系统消息Heartbeat!
2. 普通"|"分割的字符串
  - `|type|system|message|Heartbeat!|`

Arduino样例：
- `softSerial.print("{\"type\":\"system\",\"heartbeat\":-5000}\r")`

### 获取当前串口返回的数据类型
------------
OBLOQ串口接收到指令后，返回的数据类型有两种：JSON字符串和普通"|"分割的字符串

请求方式：
- `{"type":"system","message":"getReturnType"}`

参数：
- `"type":"system"` 发送消息类型：系统消息
- `"message":"getReturnType"` 获取当前OBLOQ串口返回的数据格式

返回值：
1. JSON格式
  - `{"type":"returnType","message":"json"}` 串口返回的是JSON字符串格式
2. 普通"|"分割的字符串
  - `|type|returnType|message|string|` 串口返回的是普通"|"分割的字符串格式

Arduino样例：
- `softSerial.print("{\"type\":\"system\",\"message\":\"getReturnType\"}\r")`

### 设置串口返回的数据类型
------------
OBLOQ串口接收到指令后，返回的数据类型有两种：JSON字符串和普通"|"分割的字符串

请求方式：
1. 设置返回JSON字符串格式
  - `{"type":"system","message":"setReturnType","returnType":"json"}`
2. 设置返回普通"|"分割的字符串格式
  - `{"type":"system","message":"setReturnType","returnType":"string"}`

参数：
- `"type":"system"` 发送消息类型：系统消息
- `"message":"setReturnType"` 设置当前OBLOQ串口返回的数据格式

返回值：
1. JSON格式
  - `{"type":"returnType","message":"json"}` 串口返回的是JSON字符串格式
2. 普通"|"分割的字符串
  - `|type|returnType|message|string|` 串口返回的是普通"|"分割的字符串格式

Arduino样例：
- ```cpp
  softSerial.print("{\"type\":\"system\",\"message\":\"setReturnType\",\"returnType\":\"string\"}\r")
  ```


### 重新连接wifi
------------
重新连接上一次连接的wifi

请求方式：
- `{"type":"system","message":"CONNECTWIFI"}`

参数：
- `"type":"system"` 发送消息类型：系统消息
- `"message":"CONNECTWIFI"` 发送消息字符串：CONNECTWIFI，表示重新连接wifi

返回值：
1. JSON格式
  - `{"type":"wifi","wifiState":1}` wifi连接状态：正在连接wifi
  - `{"type":"wifi","wifiState":-1}` wifi连接状态：wifi断开连接
  - `{"type":"wifi","wifiState":3} ` wifi连接状态：wifi连接失败，**检查SSID和PWD是否输入正确**
  - `{"type":"wifi","wifiState":2,"ip":"192.168.0.001"}` wifi连接状态：：wifi连接成功，返回OBLOQ的ip地址
2. 普通"|"分割的字符串
  - `|type|wifi|wifiState|1|`
  - `|type|wifi|wifiState|-1|`
  - `|type|wifi|wifiState|3|`
  - `|type|wifi|wifiState|2|ip|192.168.0,001|`

Arduino样例：
- `softSerial.print("{\"type\":\"system\",\"message\":\"CONNECTWIFI\"}\r")`

### 建立mqtt连接
------------
建立mqtt连接，默认连接到物联网host：iot.dfrobot.com.cn

请求方式：
1. 连接默认的物联网,默认连接到物联网host：iot.dfrobot.com.cn
  - `{"type":"mqtt","method":"connect","ClientId":"test","Iot_id":"test","Iot_pwd":"test"}`
2. 连接指定的物联网host
  - ```cpp
    {"type":"mqtt","method":"connect","ClientId":"SkxprkFyE-","Iot_id":"r1qHJFJ4Z","Iot_pwd":"SylqH1Y1VZ","url":"iot.dfrobot.com.cn","port":"1883"}
    ```

参数：

- `"type":"mqtt"` 发送消息类型：mqtt消息
- `"method":"connect"` 执行方法：连接请求
- `"ClientId":"test"` ClientId，随机输入，不能有中文
- `"Iot_id":"test"` 设置连接的物联网id,获取Iot_id可参考[教程](http://iot.dfrobot.com.cn/ "教程")
- `"Iot_pwd":"test"`设置连接的物联网pwd,获取Iot_pwd可参考[教程](http://iot.dfrobot.com.cn/ "教程")
- `"url":"iot.dfrobot.com.cn"`可选参数，指定连接特定的物联网host，默认连接：[iot.dfrobot.com.cn](http://iot.dfrobot.com.cn/ "iot.dfrobot.com.cn")
- `"port":"1883"` 可选参数，指定物联网host的端口

返回值：
1. JSON格式
  - `{"type":"mqtt","mqttState":1}` mqtt连接状态：连接成功
  - `{"type":"mqtt","mqttState":-1,"reason":"reason"}` mqtt连接状态：连接失败，返回连接失败的错误码
2. 普通"|"分割的字符串
  - `|type|mqtt|mqttState|1|`
  - `|type|mqtt|mqttState|-1|reason|reason|`

错误码描述：

| reason | 描述                                       |
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

Arduino样例：
- ```cpp
  softSerial.print("{\"type\":"mqtt",\"method\":\"connect\",\"ClientId\":\"test\",\"Iot_id\":\"test\",\"Iot_pwd\":\"test\"}\r")
  ```


### 订阅一个device
------------
物联网有很多设备，通过订阅设备的topic可以监听物联网设备接收到的消息。

请求方式：
- `{"type":"mqtt","method":"subscribe","topic":"BJpHJt1VW"}`

参数：
- `"type":"mqtt"` 发送消息类型：mqtt消息
- `"method":"subscribe"` 执行方法：订阅请求
- `"topic":"BJpHJt1VW"` 设置订阅的topic,获取设备topic可参考[教程](http://iot.dfrobot.com.cn/ "教程")

返回值：
1. JSON格式
  - `{"type":"mqtt","subscribe":1}` 订阅成功返回左侧字符串，订阅失败无返回。
  - `{"type":"mqtt","topic":"BJpHJt1VW","message":"test"}`返回某个订阅topic收到的消息
    - `"topic":"BJpHJt1VW"`接收到消息的topic
    - `"message":"test"`接收到的消息内容:test
2. 普通"|"分割的字符串
  - `|type|mqtt|subscribe|1|`
  - `|type|mqtt|topic|BJpHJt1VW|message|test|`

Arduino样例：
- `softSerial.print("{\"type\":\"mqtt\",\"method\":\"subscribe\",\"topic\":\"BJpHJt1VW\"}\r")`

### 发布消息
------------
物联网有很多设备，通过这条指令可以向某个特定的物联网设备发送特定的消息内容。

请求方式：
- `{"type":"mqtt","method":"publish","topic":"BJpHJt1VW","message":"test"}`

参数：
- `"type":"mqtt"` 发送消息类型：mqtt消息
- `"method":"publish"` 执行方法：发布消息
- `"topic":"BJpHJt1VW"` 接收消息的topic,获取设备topic可参考[教程](http://iot.dfrobot.com.cn/ "教程")
- `"message":"test"` 发布的消息内容：test

返回值：
1. JSON格式
  - `{"type":"mqtt","publish":1}` 发布成功返回左侧字符串，发布失败无返回。
2. 普通"|"分割的字符串
  - `|type|mqtt|publish|1|`

Arduino样例：
```cpp
- softSerial.print("{\"type\":\"mqtt\",\"method\":\"publish\",\"topic\":\"BJpHJt1VW\",\"message\":\"test\"}\r")
```



### 重新连接mqtt

------------
重新建立mqtt连接，前提是必须已经通过Iot_id，Iot_pwd建立过一次mqtt连接。

请求方式：
- `{"type":"mqtt","method":"reconnect"}`

参数：
- `"type":"mqtt"` 发送消息类型：mqtt消息
- `"method":"reconnect"` 执行方法：重新建立mqtt连接请求

返回值：
1. JSON格式
  - `{"type":"mqtt","mqttState":1}` 连接成功返回左侧字符串
2. 普通"|"分割的字符串
  - `|type|mqtt|mqttState|1|`

Arduino样例：
- `softSerial.print("{\"type\":\"mqtt\",\"method\":\"reconnect\"}\r")`

### 断开mqtt连接
------------
断开已建立的mqtt连接，前提是必须已经通过Iot_id，Iot_pwd建立过一次mqtt连接。

请求方式：
- `{"type":"mqtt","method":"disconnect"}`

参数：
- `"type":"mqtt"` 发送消息类型：mqtt消息
- `"method":"disconnect"` 执行方法：断开mqtt连接请求

返回值：
1. JSON格式
  - `{"type":"mqtt","mqttState":"-1"}` 成功断开mqtt连接返回左侧字符串，断开失败无返回
2. 普通"|"分割的字符串
  - `|type|mqtt|mqttState|-1|`

Arduino样例：
- `softSerial.print("{\"type\":\"mqtt\",\"mqttState\":\"-1\"}\r")`