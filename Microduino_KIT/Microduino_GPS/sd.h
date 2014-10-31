#include "arduino.h"

#include <SD.h>
File myFile;

void vosdread(char* name_cache)		//SD卡文件读取
{
#ifdef DEBUG
  Serial.println(" ");
  Serial.println("=== SD READ ===");
#endif

  myFile = SD.open(name_cache);		//读取完成写入的上一个文件
  if (myFile) 						//如果有对应文件
  {
    // read from the file until there's nothing else in it:
    while (myFile.available()) 
    {
      char c=myFile.read();
#ifdef DEBUG
      Serial.write(c);	//串口输出
#endif
    }
    myFile.close();					//结束
  } 
#ifdef DEBUG
  else 								//找不到对应文件
  Serial.println("=== SD error opening test.");

  Serial.println("=== SD READ Complete ===");
  Serial.println(" ");
#endif
}

void vosdwrite(char* name_cache,char choose)		//SD卡文件写入
{
#ifdef DEBUG
  Serial.println(" ");
  Serial.println("=== SD WRITE ===");
#endif

  myFile = SD.open(name_cache, FILE_WRITE);		//打开需要更新文件

  if (myFile) 					//如果有对应文件
  {
    digitalWrite(PIN_LED1,true);		//LED1亮，SD卡开始写入

    switch(choose)			//写入文件头、内容、尾
    {
    case 'H':
      myFile.println("<?xml version=\"1.0\"?>");
      myFile.println("<gpx creator=\"Geoinfor Scientek Consultant Inc.\" version=\"1.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/0\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd\">");
      myFile.println("<time>2013-09-27T13:10:31Z</time>");
      myFile.println("<trk>");
      myFile.println("<trkseg>");
#ifdef DEBUG
      Serial.println("=== SD ~~~choose Head");      
#endif
      break;
    case 'M':
      {
        static char buf1[46];
        static char buf2[20];
        static char buf3[46];
        static char buf4[20];
        static char buf5[20];
        static char buf6[20];
        static char buf7[20];
        {
          //buf1_float-------------
          char c_latitude[20],c_longitude[20];
          dtostrf(f_latitude,4,6,c_latitude);  //将获取的数值转换为字符型数组
          dtostrf(f_longitude,4,6,c_longitude);  //将获取的数值转换为字符型数组
          sprintf(buf1, "<trkpt lat=\"%s\" lon=\"%s\">", c_latitude,c_longitude); 

          //buf2_float-------------
          float s_Height;
          char c_Height[10];
          s_Height=f_Height;
          dtostrf(s_Height,4,1,c_Height);  //将获取的数值转换为字符型数组          
          sprintf(buf2, "<ele>%s</ele>", c_Height); 

          //buf3_int--------------
          sprintf(buf3, "<time>20%d-%d-%dT%d:%d:%dZ</time>", idate[0], idate[1], idate[2],itime[0],itime[1],itime[2]); 

          //buf4_normal--------------
          sprintf(buf4, "<sym>Dot</sym>"); 

          //buf5_int--------------
          sprintf(buf5, "<sat>%d</sat>",i_satellites); 

          //buf6_float--------------
          float s_fixquality;
          char c_fixquality[10];
          s_fixquality=f_fixquality;
          dtostrf(s_fixquality,4,1,c_fixquality);  //将获取的数值转换为字符型数组          
          sprintf(buf6, "<pdop>%s</pdop>",c_fixquality); 

          //buf7_normal--------------
          sprintf(buf7, "</trkpt>"); 
        }
        myFile.println(buf1);
        myFile.println(buf2);
        myFile.println(buf3);
        myFile.println(buf4);
        myFile.println(buf5);
        myFile.println(buf6);
        myFile.println(buf7);
#ifdef DEBUG
        Serial.println("=== SD ~~~choose Main");
#endif
      }
      break;
    case 'B':
      myFile.println("</trkseg>");
      myFile.println("</trk>");
      myFile.println("</gpx>");
#ifdef DEBUG
      Serial.println("=== SD ~~~choose Body");
#endif
      break;
    }
    myFile.close();			//结束

#ifdef DEBUG
    Serial.println("===SD Write DONE===");
#endif
  }
#ifdef DEBUG
  else 			//找不到对应文件
    Serial.println("===SD Write ERROR===");
#endif

  digitalWrite(PIN_LED1,false);		//LED1灭，写入结束

#ifdef DEBUG
  Serial.println("=== SD WRITE Complete ===");
  Serial.println(" ");
#endif
}

void vosdbegin(char* name_cache)		//SD卡文件初始检测
{
#ifdef DEBUG
  Serial.println(" ");
  Serial.println("=== SD BEGIN ===");
#endif
  if (SD.exists(name_cache)) 	//如果SD卡内有当前需记录的文件
  {
#ifdef DEBUG
    Serial.println("=== SD exists.OK");
#endif
    file_sta=true;    
  }
  else 							//如果SD卡内无当前需记录的文件
  {
#ifdef DEBUG
    Serial.println("=== SD exist.ERROR");
    Serial.println("=== SD Creating NEW.HEAD...");
#endif
    vosdwrite(file_name,'H');	//创建新文件头
    //    vosdread(file_name);

#ifdef DEBUG
    Serial.println("  ");
    Serial.print("=== SD The NUM ");
    Serial.print(file_num);
    Serial.print(" File:");
    Serial.print(file_name);
    Serial.print(" is ");
    Serial.print("Create");
    Serial.println("  ");
#endif
    // open a new file and immediately close it:

    if (SD.exists(name_cache)) 	//再次验证有无当前需记录的文件
    {
#ifdef DEBUG
      Serial.println("=== SD exists.OK");
#endif
      file_sta=true;    
    }
    else 
    {
#ifdef DEBUG
      Serial.println("=== SD exist.ERROR");
#endif
      file_sta=false;    
    }
  }

#ifdef DEBUG
  Serial.println(" ");
  Serial.println("===SD BEGIN Complete===");
#endif
}


void vosd_datawrite()		//常规：GPX数据写入
{
  if(STA)
  {
    //      vostring();
    vosdbegin(file_name);		//SD卡文件初始检测
    if(sd_sta && file_sta)
    {
#ifdef DEBUG
      Serial.println(" ");
      Serial.println("=== SD DATAWRITE === ");
#endif
      vosdwrite(file_name,'M');
      //        vosdread(file_name);    

#ifdef DEBUG
      Serial.print("=== SD The NUM ");
      Serial.print(file_num);
      Serial.print(" File:");
      Serial.print(file_name);
      Serial.print(" is ");
      Serial.print("Updata");
      Serial.println("=== SD DATAWRITE Complete === ");
      Serial.println(" ");
#endif
    }
  }
}

void  vosd_dataupdata()		//更新新的GPX文件
{
  eeprom_read();				//读取EEPROM，获取上个文件日期
  if(idate_cache!=idate[2])		//如果日期变化
  {
    file_num=1;		//文件序号从1计
    idate_cache=idate[2];
  }
  //    Serial.print("idate[2]:");
  //    Serial.println(idate[2]);

  vostring();				//更新文件名
  vosdbegin(file_name);     //SD卡文件初始检测
  if(sd_sta && file_sta)	//SD及其文件正常时
  {
#ifdef DEBUG
    Serial.println(" ");
    Serial.println("== SD updata ==");
#endif
    vosdwrite(file_name,'B');	//写入文件尾
    //    vosdread(file_name);
#ifdef DEBUG
    Serial.print("The NUM ");
    Serial.print(file_num);
    Serial.print(" File:");
    Serial.print(file_name);
    Serial.print(" is ");
    Serial.print("Complete");
#endif
    //-------------------
    file_num++;					//更新文件序号
    if(file_num>99)
      file_num=1;

    vostring();					//更新文件名
    vosdbegin(file_name); 	    //SD卡文件初始检测
    //    vosdread(file_name);

#ifdef DEBUG
    Serial.println(" ");
    Serial.println("== SD updata Complete ==");
#endif
  }
  //  Serial.print("idate_cache:");
  //  Serial.println(idate_cache);
  eeprom_write();  			//写入EEPROM
}
