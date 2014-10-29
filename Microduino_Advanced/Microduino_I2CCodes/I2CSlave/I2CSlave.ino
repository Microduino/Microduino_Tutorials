/*循环接收主机发送来的数据包，同时显示在串口监视器上
 
把数据包的最后一个字节，再上传回主机
*/
#include "Wire.h"//声明I2C库文件
int x;//变量x值决定主机的LED是否点亮
//初始化
void setup()
{
  Wire.begin(4);                // 加入 i2c 总线，设置从机地址为 #4
  Wire.onReceive(receiveEvent); //注册接收到主机字符的事件
  Wire.onRequest(requestEvent); // 注册主机通知从机上传数据的事件
  Serial.begin(9600);           //设置串口波特率
}
//主程序
void loop()
{
  delay(100);//延时
}

// 当从机接收到主机字符，执行该事件
void receiveEvent(int howMany)
{
  while( Wire.available()>1) // 循环执行，直到数据包只剩下最后一个字符
  {
    char c = Wire.read(); // 作为字符接收字节
    Serial.print(c);         // 把字符打印到串口监视器中
  }
   //接收主机发送的数据包中的最后一个字节
  x = Wire.read();    // 作为整数接收字节
  Serial.println(x);    //把整数打印到串口监视器中，并回车 
}

//当主机通知从机上传数据，执行该事件
void requestEvent()
{
  //把接收主机发送的数据包中的最后一个字节再上传给主机
  Wire.write(x); // 响应主机的通知，向主机发送一个字节数据
}
