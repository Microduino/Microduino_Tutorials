/*
Arduino单数码管骰子
Ansifa 2011-12-28
*/

//定义段码表，表中十个元素由LED段码生成器生成，选择了共阳极。
int a[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
void setup()
{
    DDRD = 0xFF;          //AVR定义PortD的低七位全部用作输出使用。即0xFF=B11111111对应D7~D0
    pinMode(12, INPUT);    //D12用来做骰子暂停的开关
}
void loop()
{
    for(int i = 0; i < 10; i++)
    {
        //将段码输出PortD的低7位，即Arduino的引脚D0~D6，这样需要取出PORTD最高位，即D7的状态，与段码相加，之后再输出。
        PORTD = a[i];
        delay(50);        //延时50ms
        while(digitalRead(12)) {}        //如果D12引脚高电平，则在此死循环,暂停LED跑动
    }
}