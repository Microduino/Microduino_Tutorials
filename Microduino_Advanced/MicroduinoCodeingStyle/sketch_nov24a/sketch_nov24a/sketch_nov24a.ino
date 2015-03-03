
void setup ()
{
unsigned char i;
for(i=9;i<=11;i++)//依次设置1~6个数字引脚为输出模式27.    pinMode(i,OUTPUT);//设置第i个引脚为输出模式28.}
pinMode(i,OUTPUT);
}
 
void Led(void)
{
  for (int a=0; a<=255;a++)                //循环语句，控制PWM亮度的增加
  {
    analogWrite(11,a);
    delay(8);                             //当前亮度级别维持的时间,单位毫秒            
  }
    for (int a=255; a>=0;a--)             //循环语句，控制PWM亮度减小
  {
    analogWrite(11,a);
    delay(8);                             //当前亮度的维持的时间,单位毫秒  
  }
  delay(300);                             //完成一个循环后等待的时间,单位毫秒
}
/************************************/
void Led1(void)
{
  for (int a=0; a<=255;a++)                //循环语句，控制PWM亮度的增加
  {
    analogWrite(10,a);
    delay(8);                             //当前亮度级别维持的时间,单位毫秒            
  }
    for (int a=255; a>=0;a--)             //循环语句，控制PWM亮度减小
  {
    analogWrite(10,a);
    delay(8);                             //当前亮度的维持的时间,单位毫秒  
  }
  delay(300);                             //完成一个循环后等待的时间,单位毫秒
}
/******************************************/
void Led2(void)
{
  for (int a=0; a<=255;a++)                //循环语句，控制PWM亮度的增加
  {
    analogWrite(9,a);
    delay(8);                             //当前亮度级别维持的时间,单位毫秒            
  }
    for (int a=255; a>=0;a--)             //循环语句，控制PWM亮度减小
  {
    analogWrite(9,a);
    delay(8);                             //当前亮度的维持的时间,单位毫秒  
  }
  delay(300);                             //完成一个循环后等待的时间,单位毫秒
}
/********************************************/
void loop()
{
Led();
Led1();
Led2();
}
