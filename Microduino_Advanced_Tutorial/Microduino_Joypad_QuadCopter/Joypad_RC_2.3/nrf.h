#include "Arduino.h"

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);    //ce,cs
RF24Network network(radio);

const uint16_t this_node = 0;	//设置本机ID
const uint16_t other_node=1;

//--------------------------------
struct send_a	//发送
{
  uint32_t ms;
  uint32_t rf_STA;
  uint32_t rf_throttle;
  uint32_t rf_steering;
};

struct receive_a	//接收
{
  uint32_t node_ms;
};

//--------------------------------
unsigned long node_clock,node_clock_debug,node_clock_cache=0;		//节点运行时间、节点响应检查时间、节点时间缓存

//debug--------------------------
boolean node_clock_error=false;	//节点响应状态
unsigned long time_debug=0;		//定时器


//======================================
void vodebug()
{
  if(millis()-time_debug>interval_debug)
  {
    node_clock_error=boolean(node_clock==node_clock_debug);		//一定时间内，节点返回的运行时间若不变则有问题

    node_clock_debug=node_clock;

    time_debug=millis();
  }
}


void nrf_send()
{
#ifdef Serial_DEBUG
  Serial.print("Sending...");
#endif

  send_a sen = {
    millis(),node_STA,outBuf[1],outBuf[0]
  };		//把这些数据发送出去，对应前面的发送数组
  RF24NetworkHeader header(other_node);
  if (network.write(header,&sen,sizeof(sen)))
  {
#ifdef Serial_DEBUG
    Serial.print("Is ok.");
#endif

    delay(50);
    network.update();
    // If it's time to send a message, send it!
    while ( network.available() )
    {
      // If so, grab it and print it out
      RF24NetworkHeader header;
      receive_a rec;
      network.read(header,&rec,sizeof(rec));

      node_clock=rec.node_ms;		//运行时间赋值
    }
  }
#ifdef Serial_DEBUG
  else
    Serial.print("Is failed.");

  Serial.println("");
#endif
}
