#include "arduino.h"

#define num_name 13		//文件名字长

char file_name[num_name]="";	//文件名
String file_name_cache ="";		//文件名缓存

boolean file_updata;		//是否更新新的GPX文件
boolean sd_sta,file_sta;	//SD卡状态、文件状态
int file_num;				//文件名后两位序号

int idate_cache;			//日期（日）缓存


// FileName_StringtoChar=======
void  vostring()
{
  String s_file_name="";

  for(int a=0;a<3;a++)
  {
    if(idate[a]<10) s_file_name+="0";
    s_file_name+=idate[a];
  }
  if(file_num<10)  s_file_name+="0";
  s_file_name+=file_num;
  s_file_name+=".gpx";

  for(int a=0;a<(num_name-1);a++)
    file_name[a]=s_file_name[a];

}
