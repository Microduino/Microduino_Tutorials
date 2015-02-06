#include <U8glib.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>

#define GPSECHO false
#define INTERVAL_OLED 1000 //define OLED update duration
#define INTERVAL_GPS 1000 //define GPS update duration
//font setup Large, middle, small
#define setFont_L u8g.setFont(u8g_font_fur25)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar) 
#define pi 3.14159265
#define rad2deg(X) ((X)/pi*180)
#define deg2rad(X) ((X)/180*pi)

unsigned long OLEDShowTime = millis(); //OLED refersh timmer
unsigned long GPSTime = millis(); //GPS refersh timmer 

Adafruit_GPS GPS(&Serial1);
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

//input form stellarium
char inputChar[20];
//init RAHH,RAMM,RASS;
char RAHH[3]="06",RAMM[3]="45",RASS[3]="08";
//init DECSign,DECDeg,DECMM,DECSS;
char DECSign[2]="-",DECDeg[3]="16",DECMM[3]="43",DECSS[3]="17";


//String inputString;
String moveTo="stop";
String RAtargetString, DECtargetString;
String stringUT;

boolean leapyear=false;

float RADegree, DECDegree, timeHours;
//init UT current time
int timeHH=12, timeMM=38, timeSS=0;

//UT current hours/24
float UTHoursPercentage, tableADays, tableBDays;
//the days from J2000.0
float d;
//world time
float UT;
//当地的恒星时
float LST;  
//hour angle 时角
float HA;
//高度，方位
float Alt, Az;
//init your date
int year=2015,month=2,day=5;
//init your GPS north is +, east is +
float Latitude=39.959352, Longitude=116.309462;


float stepperAngle=1.8/7.6666;

int motorAzDirPin = 5;
int motorAzStepPin = 7;
int motorAzEnPin = 8;

int motorAltDirPin = 18;
int motorAltStepPin = 22;
int motorAltEnPin = 23;

int AzStepToGo=0;
int AltStepToGo=0;


/*

Table A                |  Table B
Days to beginning of   |  Days since J2000 to
month                  |  beginning of each year
                       |
Month   Normal   Leap  |  Year   Days    |  Year   Days
        year     year  |                 |
                       |                 |
Jan       0        0   |  1998   -731.5  |   2010  3651.5
Feb      31       31   |  1999   -366.5  |   2011  4016.5
Mar      59       60   |  2000     -1.5  |   2012  4381.5
Apr      90       91   |  2001    364.5  |   2013  4747.5
May     120      121   |  2002    729.5  |   2014  5112.5    
Jun     151      152   |  2003   1094.5  |   2015  5477.5
Jul     181      182   |  2004   1459.5  |   2016  5842.5
Aug     212      213   |  2005   1825.5  |   2017  6208.5
Sep     243      244   |  2006   2190.5  |   2018  6573.5
Oct     273      274   |  2007   2555.5  |   2019  6938.5
Nov     304      305   |  2008   2920.5  |   2020  7303.5
Dec     334      335   |  2009   3286.5  |   2021  7669.5


*/

//Table A: Days to beginning of month
int daysBeginMonth[12][3]={ {1,0,0}, 
{2,31,31}, 
{3,59,60}, 
{4,90,91}, 
{5,120,121},
{6,151,152}, 
{7,181,182}, 
{8,212,213}, 
{9,243,244}, 
{10,273,274},
{11,304,305}, 
{12,334,335}};


//Table B: Days since J2000 to beginning of each year
float daySinceJ2000BeginEachYear[24][2] ={ {1998, -731.5},
{1999, -366.5},
{2000, -1.5},
{2001, 364.5},
{2002, 729.5},
{2003, 1094.5},
{2004, 1459.5},
{2005, 1825.5},
{2006, 2190.5},
{2007, 2555.5},
{2008, 2920.5},
{2009, 3286.5},
{2010, 3651.5},
{2011, 4016.5},
{2012, 4381.5},
{2013, 4747.5},
{2014, 5112.5},
{2015, 5477.5},
{2016, 5842.5},
{2017, 6208.5},
{2018, 6573.5},
{2019, 6938.5},
{2020, 7303.5},
{2021, 7669.5}};



// Define a stepper and the pins it will use
AccelStepper stepperAz(1, motorAzStepPin, motorAzDirPin);
AccelStepper stepperAlt(1, motorAltStepPin, motorAltDirPin);
//A4982,left to right(OUT2B OUT2A OUT1A OUT1B):red,green,yellow,blue



void setup() {
  Serial.begin(9600);

  GPS.begin(38400); //9600/19200/38400/57600/115200
  //GPS.set_cnssmode(GPS_SBAS); //GPS_SBAS/BEIDOU_SBAS(only NEO-M8N)/QZSS_SBAS/GLONASS_SBAS
  GPS.set_powermode(CONTINUOUS); //CONTINUOUS/POWERSAVE
  GPS.set_updata(UPDATE_2HZ); //UPDATE_1HZ/UPDATE_2HZ/UPDATE_4HZ/UPDATE_5HZ
  GPS.set_config(CONFIG_SAVE); //CONFIG_DEFAULT/CONFIG_SAVE

  pinMode(motorAzEnPin, OUTPUT);  
  pinMode(motorAltEnPin, OUTPUT);
  
  
  stepperAz.setMaxSpeed(960);
  stepperAz.setAcceleration(960);

  stepperAlt.setMaxSpeed(960);
  stepperAlt.setAcceleration(960);


}

void loop() {
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial1.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    Serial1.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  //update GPS and UT during INTERVAL_GPS
  if (GPSTime > millis()) GPSTime = millis();
  if(millis()-GPSTime>INTERVAL_GPS) {
    getCurrentGPSUT();
    GPSTime = millis();
  } 

  //if there is data send from stellarium 
  if(Serial.available()>0){
    todoActions();
  }
  //OLED display
  if (OLEDShowTime > millis()) OLEDShowTime = millis();
  if(millis()-OLEDShowTime>INTERVAL_OLED) {
    //convert RA DEC to AZ Alt
    convert_RA_DEC_to_AZ_Alt();

    OLEDShow(); //调用显示库
    
    OLEDShowTime = millis();
  } 



  driverStepperMotor();


}


void todoActions(){

  int i=0;
  inputChar[i++] = Serial.read();
  delay(5);
  while((inputChar[i++] = Serial.read()) != '#'){
    delay(5);
  }
  inputChar[i]='\0';

  //inputString=inputChar;

  //ask telescope RA
  if(inputChar[1]==':' && inputChar[2]=='G' && inputChar[3]=='R' && inputChar[4]=='#'){
    answerTelRA();//answer telescope RA
  }
  //ask telescope DEC
  if(inputChar[1]==':' && inputChar[2]=='G' && inputChar[3]=='D' && inputChar[4]=='#'){
    answerTelDEC();//answer telescope DEC
  }
  if(inputChar[1]==':' && inputChar[2]=='Q' && inputChar[3]=='#'){
  // quite stop motor
    stepperAz.stop(); 
    stepperAlt.stop();
  }
  //get targetRA when user type ctrl+1
  if(inputChar[0]==':' && inputChar[1]=='S' && inputChar[2]=='r'){
    getTargetRA();
  }
  //get targetDEC when user type ctrl+1
  if(inputChar[0]==':' && inputChar[1]=='S' && inputChar[2]=='d'){
    getTargetDEC();
    //convert RA DEC to AZ Alt
    //convert_RA_DEC_to_AZ_Alt();
  }
  if(inputChar[0]==':' && inputChar[1]=='M' && inputChar[2]=='S' && inputChar[3]=='#'){
    Serial.print("0");
    moveTo="moveing...";
  }
  moveTo="stop";
  angleConvertSteps();
}





void driverStepperMotor() {

  if(AzStepToGo>0&&AzStepToGo<360) {
    stepperAz.moveTo(-AzStepToGo);
    while (stepperAz.currentPosition() != -AzStepToGo) {
      stepperAz.run();
    } 
    stepperAz.stop(); // Stop as fast as possible: sets new target
    stepperAz.runToPosition(); 
  }


  if(AltStepToGo>0&&AltStepToGo<90) {
    stepperAlt.moveTo(AltStepToGo);
    while (stepperAlt.currentPosition() != AltStepToGo) {
      stepperAlt.run();
    } 
    stepperAlt.stop(); // Stop as fast as possible: sets new target
    stepperAlt.runToPosition(); 
  }


}

void angleConvertSteps() {
  AzStepToGo=angleToStepper(Az);
  AltStepToGo=angleToStepper(Alt);
}

int angleToStepper(float angle) {
  return angle/stepperAngle;
}



void answerTelRA(){
  Serial.print(RAHH);
  Serial.print(":");
  Serial.print(RAMM);
  Serial.print(":");
  Serial.print(RASS);
  Serial.print("#");
}

void answerTelDEC(){
  Serial.print(DECSign);
  Serial.print(DECDeg);
  Serial.print((char)223);
  Serial.print(DECMM);
  Serial.print(":");
  Serial.print(DECSS);
  Serial.print("#");
}


void getTargetRA(){
  Serial.print("1");
  RAHH[0]=inputChar[3];
  RAHH[1]=inputChar[4];
  RAMM[0]=inputChar[6];
  RAMM[1]=inputChar[7];
  RASS[0]=inputChar[9];
  RASS[1]=inputChar[10];
  RAtargetString="RA: ";
  RAtargetString+=RAHH;
  RAtargetString+="h ";
  RAtargetString+=RAMM;
  RAtargetString+="m ";
  RAtargetString+=RASS;
  RAtargetString+="s";
}


void getTargetDEC(){
  Serial.print("1");
  DECSign[0]=inputChar[3];
  DECDeg[0]=inputChar[4];
  DECDeg[1]=inputChar[5];
  DECMM[0]=inputChar[7];
  DECMM[1]=inputChar[8];
  DECSS[0]=inputChar[10];
  DECSS[1]=inputChar[11];
  DECtargetString="DEC: ";
  DECtargetString+=DECSign;
  DECtargetString+=DECDeg;
  DECtargetString+="deg ";
  DECtargetString+=DECMM;
  DECtargetString+="' ";
  DECtargetString+=DECSS;
  DECtargetString+="''";
}


void convert_RA_DEC_to_AZ_Alt() {
  getRADECDegree();
  getDaysBeforeJ2000();
  getLST();
  getHA();
  caculateAltAz();
}



void OLEDShow() {
    u8g.firstPage();
    do {
      setFont_M;
      u8g.setPrintPos(5, 10);
      u8g.print(RAtargetString);
      u8g.setPrintPos(5, 20);
      u8g.print(DECtargetString);
      u8g.setPrintPos(5, 30);
      u8g.print("Az: ");
      u8g.setPrintPos(25, 30);
      u8g.print(Az);
      u8g.setPrintPos(65, 30);
      u8g.print("/ Alt: ");
      u8g.setPrintPos(100, 30);
      u8g.print(Alt);
      u8g.setPrintPos(5, 40);
      u8g.print("Lat:");
      u8g.setPrintPos(30, 40);
      u8g.print(Latitude,2);
      u8g.setPrintPos(60, 40);
      u8g.print(",Lon:");
      u8g.setPrintPos(90, 40);
      u8g.print(Longitude,2);
      u8g.setPrintPos(5, 50);
      u8g.print(stringUT);
      // u8g.setPrintPos(5, 60);
      // u8g.print(AzStepToGo);
      // u8g.setPrintPos(60, 60);
      // u8g.print(AltStepToGo);
      // u8g.setPrintPos(5, 60);
      // u8g.print(stepperAz.currentPosition());
      // u8g.setPrintPos(60, 60);
      // u8g.print(stepperAlt.currentPosition());


      

      //u8g.print(DECSign);
    } while( u8g.nextPage() );
}

void getCurrentGPSUT() {

  int getDateTrue=GPS.hour+GPS.minute+GPS.seconds+GPS.year+GPS.month+GPS.day;

  if(getDateTrue) {
    timeHH=GPS.hour;
    timeMM=GPS.minute;
    timeSS=GPS.seconds;
    year=2000+GPS.year;
    month=GPS.month;
    day=GPS.day;
  }

  //if GPS is avilible, get current GPS
  if(GPS.fix) {
    Latitude=GPS.latitude/100;
    Longitude=GPS.longitude/100;
    if(GPS.lat=='S') {
      Latitude=-Latitude;
    }
    if(GPS.lon=='W') {
      Longitude=-Longitude;
    }
  }

  stringUT=String(year);
  stringUT+="/";
  stringUT+=month;
  stringUT+="/";
  stringUT+=day;
  stringUT+="  ";
  stringUT+=timeHH;
  stringUT+=":";
  stringUT+=timeMM;
  stringUT+=":";
  stringUT+=timeSS;

}


void getRADECDegree() {
   //get RA Degrees which is hours *15, one hour=15 degrees
  RADegree=(atof(RAHH)+(atof(RAMM)/60)+(atof(RASS)/3600))*15;
  //getDEC Degree
  DECDegree=atof(DECDeg)+(atof(DECMM)/60)+(atof(DECSS)/3600);
  if(DECSign[0]=='-') {
    DECDegree=0-DECDegree;
  } 
}

void getDaysBeforeJ2000() {
  //get current time by hours
  // timeHH=GPS.hour;
  // timeMM=GPS.minute;
  // timeSS=GPS.seconds;
  timeHours=timeHH+(timeMM/60)+(timeSS/3600);
  //UT=beijing time -8
  //UT=timeHours-8;
  UT=timeHours;
  UTHoursPercentage=UT/24;
  //if this year is leap year or not
  int colum=1;
  //year=GPS.year;
  if((((year%4)==0)&&(year%100)!=0)||(year%400==0)) {
    leapyear=true;
  }else {
    leapyear=false;
  }
  if(leapyear) {
    colum=2;
  }

  //month=GPS.month;
  tableADays=daysBeginMonth[month-1][colum];
  tableBDays=daySinceJ2000BeginEachYear[year-1998][1];
  //day=GPS.day;
  //daysbeforeJ2000
  d=UTHoursPercentage+tableADays+day+tableBDays;
}

//当地的恒星时
void getLST() {
  LST = 100.46 + (0.985647 * d) + Longitude + (15*UT);
  //Add or subtract multiples of 360 to bring LST in range 0 to 360 degrees.
  long LST_Long;
  LST_Long=LST;
  LST=LST-LST_Long;
  if(LST_Long<0) {
    LST_Long=360+LST_Long%360;
  }else{
    LST_Long=LST_Long%360;
  }
  LST=LST_Long+LST;
}

void getHA() {
  HA = LST - RADegree; 
  //If HA negative, then add 360 to bring in range 0 to 360 RA must be in degrees.
  if(HA<0) {
    HA=360+HA;
  }
}

void caculateAltAz() {
 //sin(ALT) = sin(DEC)*sin(LAT)+cos(DEC)*cos(LAT)*cos(HA)
  Alt=rad2deg(asin(sin(deg2rad(DECDegree))*sin(deg2rad(Latitude))+cos(deg2rad(DECDegree))*cos(deg2rad(Latitude))*cos(deg2rad(HA))));

  Az=rad2deg(acos((sin(deg2rad(DECDegree)) - (sin(deg2rad(Alt))*sin(deg2rad(Latitude))))/(cos(deg2rad(Alt)) * cos(deg2rad(Latitude)))));
  //if sin(HA) is positive, the angle AZ is 360 - AZ
  if (rad2deg(sin(deg2rad(HA))) > 0) {
    Az= 360 - Az;
  }
}



