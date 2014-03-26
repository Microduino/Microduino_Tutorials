/* 四位数码管的可控显示
** 注意，使用的5461AS是共阴极的，如果您是共阳极的需要注意某些部分的HIGH与LOW的替换
*/

//设置阳极接口
//D1口用来做串口数据传输（TX）
int a = 2;//D0,D1口在本程序中用来数据传出传入，不能当作数字针脚使用。
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int p = 9;//小数点
//设置阴极接口（控制1、2、3、4数码管的亮与灭）
int d1 = 13;//千位
int d2 = 12;//百位
int d3 = 11;//十位
int d4 = 10;//个位
//设置变量
int del = 5000;  //此数值可用于对时钟进行微调
int changepin = A0;//从A0口输入电位计数据
int val=0;//接收从A0口获得的电位计数值
int val4=0;  //千位上的数字，即DIG1上的数字，对应针脚为d1，Arduino上的针脚为13
int val3=0;  //百位上的数字，即DIG2上的数字，对应针脚为d2，Arduino上的针脚为12
int val2=0;  //十位上的数字，即DIG3上的数字，对应针脚为d3，Arduino上的针脚为11
int val1=0;  //个位上的数字，即DIG4上的数字，对应针脚为d4，Arduino上的针脚为10

void setup()
{
  Serial.begin(9600);//设置串口通信速率为9600
 
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);

  pinMode(p, OUTPUT);
}

void loop()
{
  val=analogRead(changepin);//读取电位计数值并赋给val
  Serial.println(val);//从串口中输出val的值
  for(int i=0;i<25;i++)//为了让数字不要那么灵敏的变来变去，给他循环25次恰好满足我的需求
  {
    //条件判断，根据val的位数分别显示
    if(val>=1000)//四位数
    {
      //注意，这里用使用了两种取某一位上的数字的算法，我只是想试试两种算法是否都好用
      val4=(val/1000)%10;                         /*比如1023除以1000等于1.023，
       取整数得到1，此即为千位上的数字，此为第一种算法，简单.*/
      val3=( val-((val/1000)%10*1000) )/100%10;   /*比如523减去523除以100等于5.23，取整后乘以100得到500，
       用523减去500得到23，再除以10得到2.3，取整后得到2，
       此为百位上的数字，此为第二章算法，稍显复杂，不过用起来也可以*/
      val2=(val-val/100%10*100)/10%10;
      val1= val-val/10%10*10-val/1000%10*1000;

      clearLEDs();
      pickDigit(1);
      pickNumber(val1);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(2);
      pickNumber(val2);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(3);
      pickNumber(val3);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(4);
      pickNumber(val4);
      delayMicroseconds(del);
    }
    else if(val>=100 && val<1000)//三位数
    {
      val3=(val/100)%10;
      val2=((val-(((val/100)%10)*100))/10)%10;
      val1=val-((val/100)%10)*100-((((val-((val/100)%10)*100)/10)%10)*10);

      clearLEDs();
      pickDigit(1);
      pickNumber(val1);
      delayMicroseconds(2*del);

      clearLEDs();
      pickDigit(2);
      pickNumber(val2);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(3);
      pickNumber(val3);
      delayMicroseconds(del);  
    }
    else if(val>=10 && val<100)//两位数
    {
      val2=(val/10)%10;
      val1=val-(((val/10)%10)*10);

      clearLEDs();
      pickDigit(1);
      pickNumber(val1);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(2);
      pickNumber(val2);
      delayMicroseconds(del);
    }
    else if(val>=0 && val<10)//一位数
    {
      val1=val;
      clearLEDs();
      pickDigit(1);
      pickNumber(val1);
      delayMicroseconds(del);
    }
  }
}

void pickDigit(int x)  //定义pickDigit(x),其作用是开启dx端口
{
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

  if(x==1)
  {
    digitalWrite(d4, LOW);
  }
  else if(x==2)
  {
    digitalWrite(d3, LOW);
  }
  else if(x==3)
  {
    digitalWrite(d2, LOW);
  }
  else if(x==4)
  {
    digitalWrite(d1, LOW);
  }
}

void pickNumber(int x)   //定义pickNumber(x),其作用是显示数字x
{
  switch(x)
  {
  default:
    zero();
    break;
  case 1:
    one();
    break;
  case 2:
    two();
    break;
  case 3:
    three();
    break;
  case 4:
    four();
    break;
  case 5:
    five();
    break;
  case 6:
    six();
    break;
  case 7:
    seven();
    break;
  case 8:
    eight();
    break;
  case 9:
    nine();
    break;
  }
}

void clearLEDs()  //清屏
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);

  digitalWrite(p, LOW);
}


/*
下面实现数字0到9以及小数点dp的显示，你也可以使用二维数组来实现这一部分，
 详情请看博文：http://tahoroom.sinaapp.com/archives/5790.html
 */
void zero()  //定义数字0时各阳极针脚的开关
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void one()  //定义数字1时各阳极针脚的开关
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void two()  //定义数字2时各阳极针脚的开关
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void three()  //定义数字3时各阳极针脚的开关
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void four()  //定义数字4时各阳极针脚的开关
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void five()  //定义数字5时各阳极针脚的开关
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void six()  //定义数字6时各阳极针脚的开关
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void seven()  //定义数字7时各阳极针脚的开关
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void eight()  //定义数字8时各阳极针脚的开关
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void nine()  //定义数字9时各阳极针脚的开关
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void dpoint() //点亮小数点
{
  digitalWrite(p, HIGH);
}
