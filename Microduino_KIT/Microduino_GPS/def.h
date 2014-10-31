#include "arduino.h"

//#define DEBUG		//是否开启串口DEBUG功能
#define GPS_Serial Serial1		//选择GPS通讯串口

#define PIN_LED1 -1	//SD状态提示LED1
#define PIN_LED2 -1	//GPS状态提示LED2
#define PIN_key1 5	//按键1
#define PIN_bat A7	//BAT
#define PIN_SD_CS 7	//SD CS

#define init_bat 5000			//电池电量刷新时间
#define init_updata 500			//gps数据刷新时间
#define init_sdwrite 3000		//SD卡写入间隔
#define init_oled 600			//OLED刷新时间
#define init_oled_updata 2000	//提示生成文件名持续时间

#define _V_max 42		//锂电池满电电压4.2V
#define _V_min 30		//锂电池没电电压3.0V

