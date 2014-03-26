    /*  
     作用:Microduino驱动数码管使用电位器旋转调整数值
     
     
     
    7段数码管具体表现方法如下
          
               A
              ____
             |    |
          F  |____|  B
             | G  |
          E  |____|  C
                    o DP
               D
              
          引脚分布情况是这样滴
          
          10 9 8 7 6
           ________
          |   __   |
          |  |__|  |
          |  |__|o |
          |________|
          
          1 2 3 4 5  
          
          * 
          * 首先看3和8这两个管脚，一般是共级，套件是共阳数码管，所以这两个是+极，两个引脚其实是接在一起的，所以我们使用其中一个即可
          * 1 (E) 接到Microduino的 10号引脚
          * 2 (D) 接到Microduino的 9号引脚
          * 3 因为我们只使用1位数码管，所以请将本引脚或者8号引脚的任意一个接到Microduino的5号数字口即可
          * 4 (C) 接到Microduino的 8号引脚
          * 5 (DP) 接到Microduino的13号引脚
          * 6 (B) 接到Microduino的7号引脚
          * 7 (A) 接到Microduino的6号引脚
          * 8 因为我们只使用1位数码管，所以请将本引脚或者3号引脚的任意一个接到Microduino的5号数字口即可
          * 9 (F) 接到Microduino的11号引脚
          * 10 (G) 接到Microduino的12号引脚
          
    */
     
    #include "SevSeg.h"
    SevSeg sevseg;
    void setup() {
    //定义需要使用的引脚
      sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);
     
    }
     
    void loop() {
      //激活输出功能
		sevseg.PrintOutput();
		int val = analogRead(A0);
		val = map(val,0,1023,0,9);    //使用map函数来做数值映射
        sevseg.NewNum(val, 4);     //数码管函数输出数字，其中数值为小数点所在位置，我放在第四格所以效果就是不显示小数点
      }