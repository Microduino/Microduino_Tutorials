    /*
     作用:介绍使用arduino模拟口测量电压，作为0-5V电压表使用实验
    */
     
    float temp;   //创建一个浮点型变量temp作为存储空间准备存放数据
    void setup()
    {
      Serial.begin(9600);     //使用9600的波特率进行串口通讯
    }
    void loop()
    {
     
      int V1 = analogRead(A0);                    
    //从A0口读取电压数据存入刚刚创建整数型变量V1，模拟口的电压测量范围为0-5V 返回的值为0-1024
      float vol = V1*(5.0 / 1023.0);               
    //我们将 V1的值换算成实际电压值存入浮点型变量 vol
      if (vol == temp)                             
    //这部分的判断是用来过滤重复的数据，只有本次的电压值和上次不一时才进行输出
      {
        temp = vol;                               //比较完成后，将这次的值存入比对比用的变量temp
      }
      else
      {
        Serial.print(vol);                       //串口输出电压值，并且不换行  
        Serial.println(" V");                    //串口输出字符V，并且换行
        temp = vol;
        delay(1000);                           //输出完成后等待1秒钟，用于控制数据的刷新速度。
      }
    }
