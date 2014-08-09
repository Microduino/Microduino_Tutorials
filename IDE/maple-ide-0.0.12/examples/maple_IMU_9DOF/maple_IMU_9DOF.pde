/**
 * \file setup.cpp
 */
#include <stdio.h>
#include <io.h>
#include <signal.h>
#include <stdint.h>
#include <WProgram.h>
#include <libmaple_types.h>
#include <nvic.h>
#include <rcc.h>
#include <gpio.h>
#include <AP_Math.h>	// ArduPilot Mega Magnetometer Library



/* Scheduler includes. */
#include "MapleFreeRTOS.h"


#define GRAVITY  8192//101 this equivalent to 1G in the raw data coming from the accelerometer 
#define Accel_Scale(x) x*(GRAVITY/9.81)//Scaling the raw data of the accel to actual acceleration in meters for seconds square

#define ToRad(x) (x*0.01745329252)
#define ToDeg(x) (x*57.2957795131) 

//gyro_scale = (0.0174532 / 16.4);
//_accel_scale = 9.81 / 4096.0;

#define Gyro_Gain 0.0609 //2.5Gyro gain 0.061
#define Gyro_Scaled(x) x*ToRad(Gyro_Gain)//Return the scaled ADC raw data of the gyro in radians for second
#define G_Dt(x) x*.005 //DT .02 = 20 miliseconds, .005 - 200 hzvalue used in derivations and integrations



#define Kp_ROLLPITCH 0.015 //.015 Pitch&Roll Proportional Gain
#define Ki_ROLLPITCH 0.00001 //0.000005Pitch&Roll Integrator Gain
#define Kp_YAW 1.2 //.5Yaw Porportional Gain  
#define Ki_YAW 0.00005 //0.0005Yaw Integrator Gain

/*Min Speed Filter for Yaw Correction*/
#define SPEEDFILT 1 //1=use min speed filter for yaw drift cancellation, 0=do not use

/*For debugging propurses*/
#define OMEGAA 1 //If value = 1 will print the corrected data, 0 will print uncorrected data of the gyros (with drift)
#define PRINT_DCM 0 //Will print the whole direction cosine matrix
#define PRINT_ANALOGS 0 // If 1 will print the analog raw data
#define PRINT_EULER 1 //Will print the Euler angles Roll, Pitch and Yaw
#define PRINT_GPS 0

/* Support for optional magnetometer (1 enabled, 0 dissabled) */
#define USE_MAGNETOMETER 1 // use 1 if you want to make yaw gyro drift corrections using the optional magnetometer   
 
// Local magnetic declination (in degrees)
// I use this web : http://www.ngdc.noaa.gov/geomagmodels/Declination.jsp
#define MAGNETIC_DECLINATION -6.0    // corrects magnetic bearing to true north
// Magnetometer OFFSETS (magnetometer calibration) (only for ArduIMU v3)
#define MAG_OFFSET_X 0
#define MAG_OFFSET_Y 0
#define MAG_OFFSET_Z 0


/* Function Prototypes */
static void vread_mpu_Tsk(void* pvParameters);
static void irq_mpu(void);
static void vPrintTask(void* pvParameters);

/* Global Variables */
xSemaphoreHandle xSemaphore;
xQueueHandle xQueue;

int16 temp_raw ;
float queue_buff[6];



uint8 dump = 0;

uint8 buffer[14]; //i2c buffer
//Sensor: GYROX, GYROY, GYROZ, ACCELX, ACCELY, ACCELZ
float SENSOR_SIGN[]={1,-1,-1,-1,1,1,-1,-1,-1}; //{1,1,-1,1,-1,1}Used to change the polarity of the sensors{-1,1,-1,-1,-1,1}
               //   {1,-1,-1,-1,1,1}
int long timer=0; //general porpuse timer 
int long timer24=0; //Second timer used to print values 
int16 AN[6]; //array that store the 6 ADC filtered data
int AN_OFFSET[6]; //Array that stores the Offset of the gyros
int EX[6]; //General porpuse array to send information

float Accel_Vector[3]= {0,0,0}; //Store the acceleration in a vector
float Gyro_Vector[3]= {0,0,0};//Store the gyros rutn rate in a vector
float Omega_Vector[3]= {0,0,0}; //Corrected Gyro_Vector data
float Omega_P[3]= {0,0,0};//Omega Proportional correction
float Omega_I[3]= {0,0,0};//Omega Integrator
float Omega[3]= {0,0,0};

float errorRollPitch[3]= {0,0,0}; 
float errorYaw[3]= {0,0,0};
float errorCourse=180; 
float COGX=0; //Course overground X axis
float COGY=1; //Course overground Y axis

unsigned int counter=0;

float DCM_Matrix[3][3]= {
  {
    1,0,0  }
  ,{
    0,1,0  }
  ,{
    0,0,1  }
}; 
float Update_Matrix[3][3]={{0,1,2},{3,4,5},{6,7,8}}; //Gyros here


float Temporary_Matrix[3][3]={
  {
    0,0,0  }
  ,{
    0,0,0  }
  ,{
    0,0,0  }
};

//GPS stuff please read SiRF-Binary-Protocol-Reference-Manual page 87 for more information
union long_union {
	int32_t dword;
	uint8_t  byte[4];
} longUnion;

union int_union {
	int16_t word;
	uint8_t  byte[2];
} intUnion;

/*Flight GPS variables*/
int gpsFix=1; //This variable store the status of the GPS
float lat=0; // store the Latitude from the gps
float lon=0;// Store guess what?
float alt_MSL=0; //This is the alt.
float ground_speed=0;// This is the velocity your "plane" is traveling in meters for second, 1Meters/Second= 3.6Km/H = 1.944 knots
float ground_course=90;//This is the runaway direction of you "plane" in degrees
float climb_rate=0; //This is the velocity you plane will impact the ground (in case of being negative) in meters for seconds
char data_update_event=0; 

//uBlox Checksum
byte ck_a=0;
byte ck_b=0;
long iTOW=0; //GPS Millisecond Time of Week
long alt=0; //Height above Ellipsoid 
float speed_3d=0; //Speed (3-D)  (not used)
 
 //mag vars
 int mag_x;
 int mag_y;
 int mag_z;
 int mag_offset[3];
 float Heading;
 float Heading_X;
 float Heading_Y;
/**
 * The main function.
 */
void setup( void )
{
    // Set up the LED to steady on
    pinMode(14, OUTPUT);
    digitalWrite(14, HIGH);

    // Setup the mpu600_int as input
    pinMode(6, INPUT);
    //digitalWrite(BOARD_BUTTON_PIN, HIGH);
    init_imu();                     // start mpu
     HMC5883_init();                // start mag
     for(int c=0; c<75; c++) // "warm-up ADC
  {
    read_MPU_raw();
  }
//Serial1.println("yoooo");
  for(int y=0; y<6; y++)
  {
    AN_OFFSET[y]=AN[y];
    //Serial1.println((int)AN_OFFSET[y]);
  }
    AN_OFFSET[5]=AN[5] - GRAVITY*SENSOR_SIGN[5];
  //AN_OFFSET[5]=GRAVITY*SENSOR_SIGN[5];

    // Wait until the user is watching
   // SerialUSB.begin();
    SerialUSB.println("Press any key to continue");
    SerialUSB.read();
    SerialUSB.println("Maple IMU started");
    
    /* Get called when the button is pressed */
    attachInterrupt(6,irq_mpu,RISING);

    /* Create the Semaphore for synchronization between UART and LED task */
    vSemaphoreCreateBinary( xSemaphore)
    /* Create the Queue for communication between the tasks */
    xQueue = xQueueCreate( 5, sizeof(queue_buff) );
    
    /* Add the  tasks to the scheduler */
    xTaskCreate(vread_mpu_Tsk, (const signed char*)"MPU",1200, NULL, 2, NULL );// ---------------bigger task stack 
    xTaskCreate(vPrintTask, (const signed char*)"Print", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    /* Start the scheduler. */
    vTaskStartScheduler();
}

void loop() 
{  // idel task
}  


/**
 * The LEDs task function.
 * It waits the semaphore is given, then takes it and update the LEDs
 * \param pvParameters NULL is passed, unused here.
 */
static void vread_mpu_Tsk(void* pvParameters)
{
    
   uint8 loop_cnt = 0;
   uint8 mag_cnt = 0 ;
    
    
    /* Infinite loop */
    while(1)
    {     
        /* Increment the LED state */
      //  leds_state ++:
        /* Block until the semaphore is given */
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
        /* update the LEDs and loop */
	 togglePin(14);
         loop_cnt++;
         mag_cnt++;
         read_MPU_raw();
        
         
         
         Matrix_update(); 
         Normalize();
         roll_pitch_drift();
         
        queue_buff[0]= ToDeg(atan2(DCM_Matrix[2][1],DCM_Matrix[2][2])); //roll
   
        queue_buff[1]= ToDeg(asin(DCM_Matrix[2][0])); // pitch
        queue_buff[2]= ToDeg(atan2(DCM_Matrix[1][0],DCM_Matrix[0][0])); //yaw
         // serial output queue
          if (mag_cnt >= 4) 
        {
          HMC5883_read();                   // Read magnetometer
          
          HMC5883_calculate( queue_buff[0],  queue_buff[1]);   // Calculate heading 
          queue_buff[3] = float(mag_x);
          queue_buff[4] = float(mag_y);
           queue_buff[5]= float(mag_z);
          mag_cnt = 0;
        }
        
         
         if (loop_cnt > 40)
          {
          /* Put the read value on the queue */
          xQueueSendToBack(xQueue, &queue_buff, 0);
           loop_cnt = 0;
          }
          
               
               
          togglePin(14);

    }
}

/**
 * The irq callback function called when the button is pressed 
 * It gives the semaphore.
 */
static void irq_mpu(void)
{
    static portBASE_TYPE xHigherPriorityTaskWoken;
    /* Give the semaphore */
    xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
}
// vim:cin:ai:sts=4 sw=4
static void vPrintTask(void* pvParameters)
{
    float mpu_prt[6];
    //uint16_t samplecount = 0;
    
    /* Infinite loop */
    while (1)
    {
        /* Wait until an element is received from the queue */
        if (xQueueReceive(xQueue, &mpu_prt, portMAX_DELAY))
        {
           // samplecount++;
            /* Print the result on the uart port */
	     //iprintf("Sample #%u: reading = %u\r\n", samplecount, temp_meas);
	       SerialUSB.print("$");
	   // SerialUSB.write(mpu_prt,7);
               SerialUSB.print(mpu_prt[0]);
               SerialUSB.print(",");
               SerialUSB.print(mpu_prt[1]);
               SerialUSB.print(",");
               SerialUSB.print(mpu_prt[2]);
	     /*  SerialUSB.print(",");
               SerialUSB.print(mpu_prt[3]);
               SerialUSB.print(",");
               SerialUSB.print(mpu_prt[4]);
               SerialUSB.print(",");
               SerialUSB.print(mpu_prt[5]);*/
	       SerialUSB.println("");
	    
        }
    }
}

