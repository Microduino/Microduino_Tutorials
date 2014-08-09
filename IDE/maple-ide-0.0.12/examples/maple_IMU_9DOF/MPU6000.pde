#include <i2c.h>
// MPU 6000 registers
#define GYRO 0x68         // gyro I2C address
#define MPUREG_WHOAMI 0x75 //
#define MPUREG_SMPLRT_DIV 0x19 //
#define MPUREG_CONFIG 0x1A //
#define MPUREG_GYRO_CONFIG 0x1B
#define MPUREG_ACCEL_CONFIG 0x1C
#define MPUREG_FIFO_EN 0x23
#define MPUREG_INT_PIN_CFG 0x37
#define MPUREG_INT_ENABLE 0x38
#define MPUREG_INT_STATUS 0x3A
#define MPUREG_ACCEL_XOUT_H 0x3B //
#define MPUREG_ACCEL_XOUT_L 0x3C //
#define MPUREG_ACCEL_YOUT_H 0x3D //
#define MPUREG_ACCEL_YOUT_L 0x3E //
#define MPUREG_ACCEL_ZOUT_H 0x3F //
#define MPUREG_ACCEL_ZOUT_L 0x40 //
#define MPUREG_TEMP_OUT_H 0x41//
#define MPUREG_TEMP_OUT_L 0x42//
#define MPUREG_GYRO_XOUT_H 0x43 //
#define MPUREG_GYRO_XOUT_L 0x44 //
#define MPUREG_GYRO_YOUT_H 0x45 //
#define MPUREG_GYRO_YOUT_L 0x46 //
#define MPUREG_GYRO_ZOUT_H 0x47 //
#define MPUREG_GYRO_ZOUT_L 0x48 //
#define MPUREG_USER_CTRL 0x6A //
#define MPUREG_PWR_MGMT_1 0x6B //
#define MPUREG_PWR_MGMT_2 0x6C //
#define MPUREG_FIFO_COUNTH 0x72
#define MPUREG_FIFO_COUNTL 0x73
#define MPUREG_FIFO_R_W 0x74


// Configuration bits MPU 3000 and MPU 6000 (not revised)?
#define BIT_SLEEP 0x40
#define BIT_H_RESET 0x80
#define BITS_CLKSEL 0x07
#define MPU_CLK_SEL_PLLGYROX 0x01
#define MPU_CLK_SEL_PLLGYROZ 0x03
#define MPU_EXT_SYNC_GYROX 0x02
#define BITS_FS_250DPS              0x00
#define BITS_FS_500DPS              0x08
#define BITS_FS_1000DPS             0x10
#define BITS_FS_2000DPS             0x18
#define BITS_FS_MASK                0x18
#define BITS_DLPF_CFG_256HZ_NOLPF2  0x00
#define BITS_DLPF_CFG_188HZ         0x01
#define BITS_DLPF_CFG_98HZ          0x02
#define BITS_DLPF_CFG_42HZ          0x03
#define BITS_DLPF_CFG_20HZ          0x04
#define BITS_DLPF_CFG_10HZ          0x05
#define BITS_DLPF_CFG_5HZ           0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF  0x07
#define BITS_DLPF_CFG_MASK          0x07
#define BIT_INT_ANYRD_2CLEAR        0x10
#define BIT_RAW_RDY_EN              0x01
#define BIT_I2C_IF_DIS              0x10
#define BIT_INT_STATUS_DATA         0x01






void read_MPU_raw(void)
{
 // uint8 buffer[14];
//togglePin(14);
  // mpu-6050 address = b1101000 = d 104 = H 68
    i2c_msg msgsl[2];      
  
    msgsl[0].addr = 0x68;
    msgsl[0].flags = 0;
    msgsl[0].length = 1;
    msgsl[0].data = buffer;
    buffer[0] = 0x3B;//address to read
    // Repeated start condition, then read 6 byte back 
    msgsl[1].addr = 0x68;
    msgsl[1].flags = I2C_MSG_READ;
    msgsl[1].length = 14;
    msgsl[1].data = buffer;
    i2c_master_xfer(I2C1, msgsl, 2,0);
  
    AN[3]=((buffer[0]  << 8) | buffer[1] );//acc x
    AN[4]=((buffer[2]  << 8) | buffer[3] );//y
    AN[5]=((buffer[4]  << 8) | buffer[5] ); //z
    temp_raw=((buffer[6]  << 8) | buffer[7] ); //z
    AN[0]=((buffer[8]  << 8) | buffer[9] ); //gyro x
    AN[1]=((buffer[10] << 8) | buffer[11]);// y
    AN[2]=((buffer[12] << 8) | buffer[13]);// z
   //  togglePin(14);
}


int read_adc(int select)
{
  //return (AN[select]-AN_OFFSET[select])*SENSOR_SIGN[select];
  if (SENSOR_SIGN[select]<0)
       {
       return (AN_OFFSET[select]-AN[select]);
       }
    else {
       return (AN[select]-AN_OFFSET[select]);
    }
}
/*
void printdata(void)//ToDeg(x)
{     
  
    
    
    
    
      //Serial1.print("!");
      file.print("!");
      #if PRINT_ANALOGS==1
  
      
      file.print(read_adc(0));
      file.print (",");
      file.print(read_adc(1));
      file.print (",");
      file.print(read_adc(2));
      file.print (",");
      file.print(read_adc(3));
      file.print (",");
      file.print(read_adc(4));
      file.print (",");
      file.print(read_adc(5));
      file.print (",");
      //Serial1.print (",AN0:");
      //Serial1.print(read_adc(0));
      //Serial1.print (",AN1:");
      //Serial1.print(read_adc(1));
      //Serial1.print (",AN2:");
      //Serial1.print(read_adc(2));
      //Serial1.print (",");
    
      //Serial1.print(",AN3:");
      //Serial1.print(read_adc(3));
      //Serial1.print (",AN4:");
      //Serial1.print(read_adc(4));
      //Serial1.print (",AN5:");
      //Serial1.print(read_adc(5));
      //Serial1.println (",");
      
      
      
     
      #endif
      #if PRINT_DCM == 1

      file.print(convert_to_dec(DCM_Matrix[0][0]));
      file.print (",");
      file.print(convert_to_dec(DCM_Matrix[0][1]));
      file.print (",");
      file.print(convert_to_dec(DCM_Matrix[0][2]));
      file.print (",");
      file.print(convert_to_dec(DCM_Matrix[1][0]));
      file.print (",");
      file.print(convert_to_dec(DCM_Matrix[1][1]));
      file.print (",");
      file.print(convert_to_dec(DCM_Matrix[1][2]));
      file.print (",");
      file.print(convert_to_dec(DCM_Matrix[2][0]));
      file.print (",");
      file.print(convert_to_dec(DCM_Matrix[2][1]));
      file.print (",");
      file.print(convert_to_dec(DCM_Matrix[2][2]));
      file.print (",");
      
      
      
      //Serial1.print ("EX0:");
      //Serial1.print(convert_to_dec(DCM_Matrix[0][0]));
      //Serial1.print (",EX1:");
      //Serial1.print(convert_to_dec(DCM_Matrix[0][1]));
      //Serial1.print (",EX2:");
      //Serial1.print(convert_to_dec(DCM_Matrix[0][2]));
      //Serial1.print (",EX3:");
      //Serial1.print(convert_to_dec(DCM_Matrix[1][0]));
      //Serial1.print (",EX4:");
      //Serial1.print(convert_to_dec(DCM_Matrix[1][1]));
      //Serial1.print (",EX5:");
      //Serial1.print(convert_to_dec(DCM_Matrix[1][2]));
      //Serial1.print (",EX6:");
      //Serial1.print(convert_to_dec(DCM_Matrix[2][0]));
      //Serial1.print (",EX7:");
      //Serial1.print(convert_to_dec(DCM_Matrix[2][1]));
      //Serial1.print (",EX8:");
      //Serial1.print(convert_to_dec(DCM_Matrix[2][2]));
      //Serial1.println (",");
      #endif
      #if PRINT_EULER == 1
      
  
     file.print(ToDeg(atan2(DCM_Matrix[2][1],DCM_Matrix[2][2])));
      file.print(",");
      file.print(ToDeg(asin(DCM_Matrix[2][0])));
      file.print(",");
      file.print(ToDeg(atan2(DCM_Matrix[1][0],DCM_Matrix[0][0])));
      file.print (",");
      
      
      //Serial1.print("RLL:");
      //Serial1.print(ToDeg(atan2(DCM_Matrix[2][1],DCM_Matrix[2][2])));
      //Serial1.print(",PCH:");
      //Serial1.print(ToDeg(asin(DCM_Matrix[2][0])));
      //Serial1.print(",YAW:");
      //Serial1.print(ToDeg(atan2(DCM_Matrix[1][0],DCM_Matrix[0][0])));
      //Serial1.print (",");
      #endif
       #if PRINT_GPS == 1
      //Serial1.print("LAT:");
      //Serial1.print((long)(lat*10000000));
      //Serial1.print(",LON:");
      //Serial1.print((long)(lon*10000000));
      //Serial1.print(",ALT:");
      //Serial1.print(alt_MSL);
      //Serial1.print(",COG:");
      //Serial1.print((ground_course));
      //Serial1.print(",SOG:");
      //Serial1.print(ground_speed);
      //Serial1.print(",FIX:");
      //Serial1.print((int)gpsFix);
      #endif
      
      //Serial1.println("*");
      writeCRLF(file);
      file.sync(); 
}

long convert_to_dec(float x)
{
  return x*1000000;
}

*/
void write_imu(int dev_add,uint8 *buf)
{i2c_msg msgs[1];
    msgs[0].addr = dev_add;
    msgs[0].flags = 0;
    msgs[0].length = 2;
    msgs[0].data = buf;
    i2c_master_xfer(I2C1, msgs, 1,0);
}



void init_imu() {
uint8 buf_init[2] = {MPUREG_PWR_MGMT_1, BIT_H_RESET}; // 0001 1011

    i2c_master_enable(I2C1, I2C_FAST_MODE);
     delay(100);
       write_imu(GYRO,buf_init);
       delay(100);
       // Wake up device and select GyroZ clock (better performance)
      buf_init = {MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ};
       write_imu(GYRO,buf_init);
      delay(100);
      
      // SAMPLE RATE
      buf_init = {MPUREG_SMPLRT_DIV,0x04};// Sample rate = 200Hz    Fsample= 1Khz/(4+1) = 200Hz 0x13 = 50hz  5 hz= c7
       write_imu(GYRO,buf_init);
      delay(100);
      
      // DLPF    DLPF = 20Hz (low pass filter)BITS_DLPF_CFG_188HZ 
      buf_init = {MPUREG_CONFIG, BITS_DLPF_CFG_188HZ};
       write_imu(GYRO,buf_init);
      delay(100);
      
      // Gyro scale 2000ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Âº/s
      buf_init = {MPUREG_GYRO_CONFIG,BITS_FS_2000DPS};
       write_imu(GYRO,buf_init);
      delay(100);
      
      // Accel scele 4g (4096LSB/g)
       buf_init = {MPUREG_ACCEL_CONFIG,0x08};
       write_imu(GYRO,buf_init);
      delay(100);
      
      // INT CFG => Interrupt on Data Ready
      buf_init = {MPUREG_INT_ENABLE,BIT_RAW_RDY_EN};
       write_imu(GYRO,buf_init);
      delay(100);
      
      // INT: Clear on any read
      buf_init = {MPUREG_INT_PIN_CFG,BIT_INT_ANYRD_2CLEAR};
       write_imu(GYRO,buf_init);
      delay(100);
}

/*void MPU_warmup()
{
  
  
  
  
  
  
  
  
  
  
  
  
  
  
}*/




