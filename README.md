# ESP8266 SOC 方案连接云平台实现远程家居控制

## 实现功能
* 连接机智云物联网平台
* 实时获取温度和湿度
* 控制RGB灯珠，实现颜色调节
* 控制2个继电器
* 按键S1短按控制继电器A的开关（同时上报状态），长按实现SoftAP模式
* 按键S2短按控制继电器B份开关（同时上报状态），长按实现AirLink模式
* 串口打印log，波特率 115200bps

## 硬件环境
* ESP12F（32Mbit）WIFI模组  1个
* DTH11温湿度传感器  1个
* RGB-LED  1个

## GPIO管脚分配
|管脚|功能|
|---|---|
|GPIO 13|RGB-LED Red
|GPIO 14|RGB-LED Green
|GPIO 15|RGB-LED Blue
|GPIO 2|DHT11
|GPIO 4|继电器 A
|GPIO 5|继电器 B
|GPIO 0|按键 S1
|GPIO 12|按键 S2
