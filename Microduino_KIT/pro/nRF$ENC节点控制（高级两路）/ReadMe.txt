手机通过路由器操控以太网关与主机通信，主机通过nRF控制节点开关以及监控温度

主机通过以太网连接路由器
在以下两行设置好IP以及路由器网关后即可使用。
static byte myip[] = { 
  192,168,169,111 };
// gateway ip address
static byte gwip[] = { 
  192,168,169,1 };

灯泡节点通过市电板直接连接一个灯泡即可
饮水机除了需要改装内部电路，并且把一个DS18B20温度传感器贴在饮水机桶壁以检测温度（占用I/O口为A4）

主机：
l 核心：Microduino Core+
l 2.4G：Microduino nRF
l 以太网：Microduino Ethernet（可选ENC28J60或W5500）
l 供电：Microduino DUO H2

节点：
l 核心：Microduino Core
l 2.4G：Microduino nRF
l 220V市电控制：Microduino 电器控制板
l 温度传感器：DS18B20（仅饮水机需要）


......From PKJ : 2014.06.30