long  duan[]={
  0x0f,0x17,0x1b,0x1d,0x1e};
long  data[]=
{
  0x44,0x42,0x7F,0x40,0x40,
  0x79,0x49,0x49,0x49,0x4F,
};

void setup()
{
  for(int x=2;x<14;x++) 
  {
    pinMode(x,OUTPUT);//设置输出引脚
  }  

}
void loop()
{
  for(int j=0;j<100;j++)
  {
    for(int i=0;i<5;i++)
    {
      ledduan(duan[i]);
      leddisplay(data[i]);
    delayMicroseconds(800);
    }
  }  
  for(int j=0;j<100;j++)
  {
    for(int i=0;i<5;i++)
    {
      ledduan(duan[i]);
      leddisplay(data[i+5]);
    delayMicroseconds(800);
    }
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

