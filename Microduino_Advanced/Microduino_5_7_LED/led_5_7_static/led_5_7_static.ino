//共阴数码管

long  duan[]={
  0x0f,0x17,0x1b,0x1d,0x1e};
long  data[]=
{
0x4D,0x1E,0x3C,0x1E,0x4D,
0x7F,0x41,0x5D,0x41,0x7F
};//阴码、逐列式、逆向

void setup()
{
  for(int x=2;x<14;x++) 
  {
    pinMode(x,OUTPUT);//设置输出引脚
  }  

}
void loop()
{
  for(int i=0;i<5;i++)
  {
    ledduan(duan[i]);
    leddisplay(data[i+5]);
    delayMicroseconds(800);
  }
}

void leddisplay(int num)    // 将花样字模对应到端口显示
{
  digitalWrite(7, ((num>>0)&0x01));
  digitalWrite(8, ((num>>1)&0x01));
  digitalWrite(9, ((num>>2)&0x01));
  digitalWrite(10, ((num>>3)&0x01));
  digitalWrite(11, ((num>>4)&0x01));
  digitalWrite(12, ((num>>5)&0x01));
  digitalWrite(13, ((num>>6)&0x01));
}

void ledduan(int num)   
{
  digitalWrite(6, ((num>>0)&0x01));
  digitalWrite(5, ((num>>1)&0x01));
  digitalWrite(4, ((num>>2)&0x01));
  digitalWrite(3, ((num>>3)&0x01));
  digitalWrite(2, ((num>>4)&0x01));
}


