/*
使用Microduino生成双音多频
Ansifa        2011/11/27

* 软件：需要Tone库，下载地址：http://rogue-code.googlecode.com/files/Arduino-Library-Tone.zip
* 材料：喇叭1个，100Ω电阻1个，1uF电容两个
* 硬件连接：在D11，D12引脚各接一个1uF电容，之后合并到100Ω电阻一端，
  100Ω电阻另一端接喇叭，然后喇叭另一端接地。

用法，将喇叭贴近电话机话筒，然后发送要拨的电话号码到串口，比如10000，即可拨通电话10000
*/

#include <Tone.h>

String Phone_Number = "";
int i = 0, mark = 0;

//定义freq1，freq2为Tone实例，并且定义双音多频的频率
//DTMF频率定义参见：[url]http://zh.wikipedia.org/zh/%E5%8F%8C%E9%9F%B3%E5%A4%9A%E9%A2%91[/url]
Tone freq1;
Tone freq2;
const int DTMF_freq1[] = {1336, 1209, 1336, 1477, 1209, 1336, 1477, 1209, 1336, 1477};
const int DTMF_freq2[] = {941,  697,  697,  697,  770,  770,  770,  852,  852,  852};

void setup()
{
	Serial.begin(9600);
    //定义声音产生引脚在Arduino的D11，D12
    freq1.begin(11);
    freq2.begin(12);
}

void loop()
{
    //读出串口数据，串接成Phone_Number字符串
    while (Serial.available() > 0)
    {
    	Phone_Number += char(Serial.read());
    	delay(2);
    	mark = 1;
    }
    //播放DTMF音频，电话号码来自Phone_Number，持续长度200ms，间隔长度300ms
    PlayDTMF(Phone_Number, 200, 300);
    
        //如果刚才接收到串口的电话号码，因为号码已经在喇叭输出了，清空电话号码并且重置mark
    if(mark == 1)
    {
       Phone_Number = "";
       Serial.println();
       mark = 0;
   }
}

/*
DTMF播放函数
调用格式：playDTMF(数字(0~9), 持续时间)。
*/
void PlayDTMF(String Number, long duration, long pause)
{
        //如果输入数字为空，或者持续时间不为正数，或者暂停时间不为正数
        //则视作错误，立即停止执行返回主程序。
    if(Number.length() == 0 || duration <= 0 || pause <= 0) return;
    //将Number逐个文字分离，
    for(i = 0; i < Number.length(); i++)
    {
        //如果Number属于0~9的数字字符，
        if(Number[i] >= '0' && Number[i] <= '9')
        {
            //则将它ASCII码减去‘0’，得到纯数字，
            Number[i] -= '0';
            //然后在串口输出一份，便于查看
            Serial.print(Number[i], DEC);
            //输出双音多频之一
            freq1.play(DTMF_freq1[Number[i]], duration);
            //输出双音多频之二
            freq2.play(DTMF_freq2[Number[i]], duration);
            delay(pause);
        }
    }

}
