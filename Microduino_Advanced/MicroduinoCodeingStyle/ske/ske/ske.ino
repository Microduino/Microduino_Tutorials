
void setup ()
{
unsigned char i;
for(i=9;i<=11;i++)//依次设置1~6个数字引脚为输出模式27.    pinMode(i,OUTPUT);//设置第i个引脚为输出模式28.}
pinMode(i,OUTPUT);
}
void loop()
{
Led();
Led1();
Led2();
}
