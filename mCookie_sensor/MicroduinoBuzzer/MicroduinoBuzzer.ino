#define buzzer_pin 6
void setup()
{
  pinMode(buzzer_pin,OUTPUT);
}
 
void loop()
{
  for(int i=200;i<=800;i++)  //用循环的方式将频率从200HZ 增加到800HZ
  {
    tone(buzzer_pin,i);    //在端口输出频率
    delay(5);      //该频率维持5毫秒   
  }
  delay(1000);     //最高频率下维持1秒钟
  for(int i=800;i>=200;i--)
  {
    tone(buzzer_pin,i);
    delay(10); //该频率维持10毫秒   
  }
}