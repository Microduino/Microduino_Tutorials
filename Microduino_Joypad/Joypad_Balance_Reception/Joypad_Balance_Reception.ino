#define MOTOR1_EN  8      //PORTAD,6
#define MOTOR1_DIR 5    //PORTB,1
#define MOTOR1_STEP 7   //PORTB,3
#define MOTOR2_EN 23      //PORTD,7
#define MOTOR2_DIR 18    //PORTC,3
#define MOTOR2_STEP 22   //PORTD,4

#define LED 4
//rf=======================================
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
RF24Network network(radio);
const uint16_t this_node = 1;	//设置本机ID
const uint16_t other_node=0;

//--------------------------------
struct send_a	//发送
{
  uint32_t node_ms;		//节点运行时间
};

unsigned long last_sent=0;	//定时器

//--------------------------------
struct receive_a	//接收
{
  uint32_t ms;
  uint32_t rf_STA;
  uint32_t rf_throttle;
  uint32_t rf_steering;
};

unsigned long clock;	//主机运行时间
int tem_xuan=0;			//主机请求时序

//----------------------------
boolean node_STA=false;
int robot_throttle;
int robot_steering;

unsigned long safe_ms=millis();
//=================================================================================
#include <Wire.h>
#include <I2Cdev.h>
#include <JJ_MPU6050_DMP_6Axis.h>  // 与DMP工作库的修改版本（见注释内）

//=================================================================================
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))

//=========================================
#define DEBUG 0

#define ZERO_SPEED 65535 //零速
#define MAX_ACCEL 7 //最大ACCEL
#define MAX_THROTTLE 480 //最大油门530
#define MAX_STEERING 120 //最大转向
#define MAX_TARGET_ANGLE 12 //最大目标角度12

// 默认控制条款  
#define KP 0.22 // 0.22        
#define KD 28  // 30 28 26  
#define KP_THROTTLE 0.065  //0.08//0.065
#define KI_THROTTLE 0.05//0.05
// 对于raiseup控制增益
#define KP_RAISEUP 0.16 //kp兴起
#define KD_RAISEUP 40   //kd兴起
#define KP_THROTTLE_RAISEUP 0  //没有对raiseup速度控制
#define KI_THROTTLE_RAISEUP 0.0//油门兴起
#define ITERM_MAX_ERROR 40   // ITERM饱和常数
#define ITERM_MAX 5000

//=================================================================================

#define I2C_SPEED 400000L //I2C速度
#define Gyro_Gain 0.03048//陀螺增益
#define Gyro_Scaled(x) x*Gyro_Gain //返回每秒度的缩放陀螺仪的原始数据
#define RAD2GRAD 57.2957795//57.2957795
#define GRAD2RAD 0.01745329251994329576923690768489//0.01745329251994329576923690768489

//=================================================================================
bool Robot_shutdown=false;   // Robot shutdown flag => Out of battery
uint8_t mode;   // 0: MANUAL MODE   1: autonomous MODE
uint8_t autonomous_mode_status;   // 1: NORMAL STATUS=Walking  2: 
int16_t autonomous_mode_counter;  
int16_t autonomous_mode_distance;
int16_t pushUp_counter;  // for pushUp functionality (experimental)

// MPU控制/状态瓦尔
bool dmpReady = false;  // 设置为true，如果DMP初始化成功
uint8_t mpuIntStatus;   // 拥有从MPU实际中断状态字节
uint8_t devStatus;      // 每个设备操作后返回的状态（0=成功！0=错误）
uint16_t packetSize;    // 预计DMP数据包大小（我们18字节）
uint16_t fifoCount;     // 目前在FIFO中的所有字节数
uint8_t fifoBuffer[18]; // FIFO存储缓冲器
Quaternion q;
uint8_t loop_counter;       // 要生成媒体40Hz的循环
uint8_t slow_loop_counter;  // 慢环2HZ
long timer_old;//计时器老
long timer_value;//定时器的值
int debug_counter;//调试计数器
float dt;
int lkf;
// 类的默认I2C地址为0X68
MPU6050 mpu;
float angle_adjusted;//角度调整
float angle_adjusted_Old;//角度调整
float Kp=KP;
float Kd=KD;
float Kp_thr=KP_THROTTLE;//油门
float Ki_thr=KI_THROTTLE;
float Kp_user=KP;
float Kd_user=KD;
float Kp_thr_user=KP_THROTTLE;
float Ki_thr_user=KI_THROTTLE;
bool newControlParameters = false;//布尔新的控制参数
bool modifing_control_parameters=false;//布尔新的控制参数
float PID_errorSum;
float PID_errorOld = 0;
float PID_errorOld2 = 0;
float setPointOld = 0;
float target_angle;
float throttle;
float kkll;
float steering;
float max_throttle = MAX_THROTTLE;//最大油门=最大油门
float max_steering = MAX_STEERING;
float max_target_angle = MAX_TARGET_ANGLE;
float control_output;
int16_t motor1;
int16_t motor2;
int16_t speed_m[2];           // 电机的实际转速
uint8_t dir_m[2];             // 步进电机的实际方向
int16_t actual_robot_speed;          // 整个机器人的速度（从踏步机速测）
int16_t actual_robot_speed_Old;          // 整个机器人的速度（从踏步机速测）
float estimated_speed_filtered;//估计速度过滤
uint16_t counter_m[2];        // 计数器周期
uint16_t period_m[2][8];      // 八子时期
uint8_t period_m_index[2];    //索引子时期
int freeRam () { //免费拉姆
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
//DMP功能
//这个函数定义在传感器融合加速的重量
//默认值是0x80的
//官方InvenSense公司名称是inv_key_0_96（？）
void dmpSetSensorFusionAccelGain(uint8_t gain)
{
  // INV_KEY_0_96
  mpu.setMemoryBank(0);
  mpu.setMemoryStartAddress(0x60);
  mpu.writeMemoryByte(0);
  mpu.writeMemoryByte(gain);
  mpu.writeMemoryByte(0);
  mpu.writeMemoryByte(0);
}
// 快速计算，从四元解obtein披角度
float dmpGetPhi() {
  mpu.getFIFOBytes(fifoBuffer, 16); // 我们只看过四元
  mpu.dmpGetQuaternion(&q, fifoBuffer); 
  mpu.resetFIFO();  //我们始终复位FIFO 
  //返回( asin(-2*(q.x * q.z - q.w * q.y)) * 180/M_PI); //roll
  return (atan2(2*(q.z * q.y + q.x * q.w), q.x * q.x - q.w * q.w - q.z * q.z + q.y * q.y)* RAD2GRAD);
}
//xyzw
//wzyx

// PD的实施。 DT是毫秒
float stabilityPDControl(float DT, float input, float setPoint,  float Kp, float Kd)
{
  float error;
  float output;
  error = setPoint-input;
  // Kd的两部分实施
  //仅使用输入（传感器）的一部分而不是设定值输入输入（T-2）最大的一个
  //而第二个使用该设定值，使之更有点侵略性设定点设定点（T-1）
  output = Kp*error + (Kd*(setPoint - setPointOld) - Kd*(input - PID_errorOld2))/DT;       // + 错误 - PID_error_Old2
  //Serial.print(Kd*(error-PID_errorOld));Serial.print("\t");
  PID_errorOld2 = PID_errorOld;
  PID_errorOld = input;  // 误差为Kd值是唯一的输入组件
  setPointOld = setPoint;
  return(output);
}
//P控制实现。
float speedPControl(float input, float setPoint,  float Kp)
{
  float error;
  error = setPoint-input;
  return(Kp*error);
}
// PI实现。 DT是毫秒
float speedPIControl(float DT, float input, float setPoint,  float Kp, float Ki)
{
  float error;
  float output;
  error = setPoint-input;
  PID_errorSum += constrain(error,-ITERM_MAX_ERROR,ITERM_MAX_ERROR);
  PID_errorSum = constrain(PID_errorSum,-ITERM_MAX,ITERM_MAX);
  output = Kp*error + Ki*PID_errorSum*DT*0.001;
  return(output);
}
//在16MHz的200ns=>4条指令
void delay_200ns()  
{
  __asm__ __volatile__ (
  "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop"); 
}
ISR(TIMER1_COMPA_vect)
{
  counter_m[0]++;
  counter_m[1]++;
  if (counter_m[0] >= period_m[0][period_m_index[0]])
  {
    counter_m[0] = 0;
    if (period_m[0][0]==ZERO_SPEED)
      return;
    if (dir_m[0])
      CLR(PORTB,1);
    //      digitalWrite(MOTOR1_DIR,LOW);
    else
      SET(PORTB,1);
    //      digitalWrite(MOTOR1_DIR,HIGH);
    // 我们需要等待，以免200ns的产生步进脉冲...
    period_m_index[0] = (period_m_index[0]+1)&0x07; // 周期M指数从0到7
    //delay_200ns();
    SET(PORTB,3);
    //digitalWrite(MOTOR1_STEP,HIGH);
    delayMicroseconds(1);
    CLR(PORTB,3);
    //digitalWrite(MOTOR1_STEP,LOW);
  }
  if (counter_m[1] >= period_m[1][period_m_index[1]]) 
  {
    counter_m[1] = 0;
    if (period_m[1][0]==ZERO_SPEED)
      return;
    if (dir_m[1])
      CLR(PORTC,3);
    //digitalWrite(MOTOR2_DIR,LOW);
    else
      SET(PORTC,3);
    //digitalWrite(MOTOR2_DIR,HIGH);
    period_m_index[1] = (period_m_index[1]+1)&0x07;
    //delay_200ns();
    SET(PORTD,4);
    //digitalWrite(MOTOR2_STEP,HIGH);
    delayMicroseconds(1);
    CLR(PORTD,4);
    //digitalWrite(MOTOR2_STEP,LOW);
  }
}
void calculateSubperiods(uint8_t motor)
{
  int subperiod;
  int absSpeed;
  uint8_t j;

  if (speed_m[motor] == 0)
  {
    for (j=0;j<8;j++)
      period_m[motor][j] = ZERO_SPEED;
    return;
  }
  if (speed_m[motor] > 0 )   // 正速度
  {
    dir_m[motor] = 1;
    absSpeed = speed_m[motor];
  }
  else                       // 负速度
  {
    dir_m[motor] = 0;
    absSpeed = -speed_m[motor];
  }

  for (j=0;j<8;j++)
    period_m[motor][j] = 1000/absSpeed;
  // 计算亚期。如果模块<0.25=>子期间= 0，如果模块<0.5=>子周期= 1。如果模块<0.75子期间=2其他子期间=3
  subperiod = ((1000 % absSpeed)*8)/absSpeed;   // 优化代码来计算子期间（整数运算）
  if (subperiod>0)
    period_m[motor][1]++;
  if (subperiod>1)
    period_m[motor][5]++;
  if (subperiod>2)
    period_m[motor][3]++;
  if (subperiod>3)
    period_m[motor][7]++;
  if (subperiod>4)
    period_m[motor][0]++;
  if (subperiod>5)
    period_m[motor][4]++;
  if (subperiod>6)
    period_m[motor][2]++; 
}
void setMotorSpeed(uint8_t motor, int16_t tspeed)
{
  // 我们限制最大加速度
  if ((speed_m[motor] - tspeed)>MAX_ACCEL)
    speed_m[motor] -= MAX_ACCEL;
  else if ((speed_m[motor] - tspeed)<-MAX_ACCEL)
    speed_m[motor] += MAX_ACCEL;
  else
    speed_m[motor] = tspeed;  
  calculateSubperiods(motor);  //我们采用四个子周期来提高分辨率
  // 为了节省能量，当它没有运行...
  if ((speed_m[0]==0)&&(speed_m[1]==0))
  {
    digitalWrite(MOTOR1_EN,HIGH);   //禁用电机
    digitalWrite(MOTOR2_EN,HIGH);   //禁用电机
  }
  else
  {
    digitalWrite(MOTOR1_EN,LOW);   // 使电机
    digitalWrite(MOTOR2_EN,LOW);   // 使电机
  }
}

void setup() 
{
  Serial.begin(115200);

  // 步进引脚 
  pinMode(MOTOR1_EN,OUTPUT);    // ENABLE MOTORS
  pinMode(MOTOR2_EN,OUTPUT);    // ENABLE MOTORS

  pinMode(MOTOR1_DIR,OUTPUT);   // DIR MOTOR 1 (PB4,D8)
  pinMode(MOTOR1_STEP,OUTPUT);   // STEP MOTOR 1 (PD7,D6)*

  pinMode(MOTOR2_DIR,OUTPUT);  // DIR MOTOR 2 (PC7,13)*
  pinMode(MOTOR2_STEP,OUTPUT);  // STEP MOTOR 1 (PD6,12)*

  digitalWrite(MOTOR1_EN,HIGH);   // Disbale motors
  digitalWrite(MOTOR2_EN,HIGH);   // Disbale motors

  pinMode(LED,OUTPUT);    // ENABLE MOTORS

  //mpu==============================
  // 加入I2C总线
  Wire.begin();
  // 4000Khz fast mode
  TWSR = 0;
  TWBR = ((16000000L/I2C_SPEED)-16)/2;
  TWCR = 1<<TWEN;

  digitalWrite(LED,HIGH);
  delay(200);
  digitalWrite(LED,LOW);
  delay(200);

  Serial.println("Initializing I2C devices...");
  //mpu.initialize();
  mpu.setClockSource(MPU6050_CLOCK_PLL_ZGYRO);
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  mpu.setDLPFMode(MPU6050_DLPF_BW_20);  //10,20,42,98,188
  mpu.setRate(4);   // 0=1khz 1=500hz, 2=333hz, 3=250hz 4=200hz
  mpu.setSleepEnabled(false);

  digitalWrite(LED,HIGH);
  delay(500);
  digitalWrite(LED,LOW);
  delay(500);

  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  if (devStatus == 0) 
  {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;

    digitalWrite(LED,HIGH);
    delay(200);
    digitalWrite(LED,LOW);
    delay(200);
  } 
  else 
  { // ERROR!
    while(1)
    {
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));

      digitalWrite(LED,HIGH);
      delay(1000);
      digitalWrite(LED,LOW);
      delay(200);
    }
  }

  // 陀螺仪的校准
  //机器人必须在初始化时是稳定的
  // 一段时间来解决的事情从没有运动算法的偏置需要一些时间才能生效并重置陀螺仪偏置。15000
  digitalWrite(LED,HIGH);
  delay(200);        
  digitalWrite(LED,LOW);
  delay(500);
  digitalWrite(LED,HIGH);
  delay(200);
  digitalWrite(LED,LOW);

  Serial.print("Free RAM: ");
  Serial.println(freeRam());
  Serial.print("Max_throttle: ");
  Serial.println(max_throttle);
  Serial.print("Max_steering: ");
  Serial.println(max_steering);
  Serial.print("Max_target_angle: ");
  Serial.println(max_target_angle);

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  timer_old = millis();
  //我们将覆盖定时器使用步进电机
  //步进电机初始化
  //定时器CTC模式
  TCCR1B &= ~(1<<WGM13);
  TCCR1B |=  (1<<WGM12);
  TCCR1A &= ~(1<<WGM11); 
  TCCR1A &= ~(1<<WGM10);
  // 输出模式=00（断开）
  TCCR1A &= ~(3<<COM1A0); 
  TCCR1A &= ~(3<<COM1B0); 
  // 设置定时器预分频器
  //通常我们使用的8分频器，产生16MHz的CPU的一个2MHz的计时器
  TCCR1B = (TCCR1B & ~(0x07<<CS10)) | (2<<CS10);
  OCR1A = 80;   // 25Khz
  TCNT1 = 0;

  digitalWrite(LED,HIGH);
  delay(200);
  digitalWrite(LED,LOW);
  delay(200);

  //调整传感器融合增益
  Serial.println("Adjusting DMP sensor fusion gain...");
  dmpSetSensorFusionAccelGain(0x40);//0x20/////////////////////////////////////////////////////////调整传感器
  Serial.println("Initializing Stepper motors...");
  delay(200);
  TIMSK1 |= (1<<OCIE1A);  // 启用定时器中断
  digitalWrite(MOTOR1_EN,LOW);    // 使步进驱动器
  digitalWrite(MOTOR2_EN,LOW);    // 使步进驱动器

  // 小电机的振动，表明机器人已准备就绪

  for (uint8_t k=0;k<3;k++)
  {
    setMotorSpeed(0,3);
    setMotorSpeed(1,-3);
    delay(150);
    setMotorSpeed(0,-3);
    setMotorSpeed(1,3);
    delay(150);
  }

  mpu.resetFIFO();
  timer_old = millis();

  //nRF==============================
  SPI.begin();		//初始化SPI总线
  radio.begin();
  network.begin(/*channel*/ 70, /*node address*/ this_node);

  digitalWrite(LED,HIGH);
  Serial.println("===========start===========");
}
// 主循环//////////////////////////////////////////////////////////////////////////+++++++++++++++++++++++++++++++++++++++++++++++++++++++
void loop() 
{	   
  //===============================================================
  nRF();

  //===============================================================
  if(safe_ms>millis()) safe_ms=millis();
  if(millis()-safe_ms>1000)
  {
    steering=0;
    throttle=0;
  }

  //===============================================================
  robot();
}

void robot()
{
  //===============================================================
  timer_value = millis();

  // 新的DMP定位解决方案
  fifoCount = mpu.getFIFOCount();
  if (fifoCount>=18)
  {
    if (fifoCount>18)  // 如果我们有一个以上的数据包，我们采取简单的路径：丢弃缓冲区
    {
      mpu.resetFIFO();
      return;
    }

    loop_counter++;
    slow_loop_counter++;
    dt = (timer_value-timer_old);
    timer_old = timer_value;  
    angle_adjusted_Old = angle_adjusted;
    angle_adjusted = dmpGetPhi();
    mpu.resetFIFO();  // 我们始终复位FIFO
    // 我们计算估计机器人的速度
    actual_robot_speed_Old = actual_robot_speed;
    actual_robot_speed = (speed_m[1] - speed_m[0])/2;  // 正面：前锋
    int16_t angular_velocity = (angle_adjusted-angle_adjusted_Old)*90.0;     // 角速度角度调整角度调整旧
    int16_t estimated_speed = actual_robot_speed_Old - angular_velocity;     // 我们利用机器人速度（T-1）或（T-2），以补偿该延迟
    estimated_speed_filtered = estimated_speed_filtered*0.95 + (float)estimated_speed*0.05;//估计速度估计过滤滤速
    target_angle = speedPIControl(dt,estimated_speed_filtered,throttle,Kp_thr,Ki_thr); //目标角速度PIC ONTROL dt的速度估计过滤油门Kp_thr Ki_thr
    target_angle = constrain(target_angle,-max_target_angle,max_target_angle);   // 有限的输出  目标角度约束目标角度最大目标角度最大目标角度

    if (pushUp_counter>0)  // pushUp mode?
      target_angle = 10;

    //我们整合输出（加速度）
    control_output += stabilityPDControl(dt,angle_adjusted,target_angle,Kp,Kd);
    control_output = constrain(control_output,-800,800);   // 限制最大输出控制      
    // 控制的转向部分的输出直接注射
    motor1 = control_output + steering;
    motor2 = -control_output + steering;   //马达2反转    
    // 限制最大速度
    motor1 = constrain(motor1,-500,500);   
    motor2 = constrain(motor2,-500,500);

    // Is robot ready (upright?)
    if ((angle_adjusted<66)&&(angle_adjusted>-66))
    {
      if (node_STA) // pushUp mode?
      {
        pushUp_counter++;

        if (pushUp_counter>60) // 0.3 seconds
        {
          // Set motors to 0 => disable steppers => robot 
          setMotorSpeed(0,0);
          setMotorSpeed(1,0);
          // We prepare the raiseup mode  
          Kp = KP_RAISEUP;   // CONTROL GAINS FOR RAISE UP
          Kd = KD_RAISEUP;
          Kp_thr = KP_THROTTLE_RAISEUP;
          control_output = 0;
          estimated_speed_filtered = 0;
        }
        else
        {
          setMotorSpeed(0,motor1);
          setMotorSpeed(1,motor2);
        }
      }
      else
      {
        // NORMAL MODE
        setMotorSpeed(0,motor1);
        setMotorSpeed(1,motor2);
        pushUp_counter=0;
      }

      if ((angle_adjusted<40)&&(angle_adjusted>-40))
      {
        Kp = Kp_user;  // Default or user control gains
        Kd = Kd_user; 
        Kp_thr = Kp_thr_user;
        Ki_thr = Ki_thr_user;
      }     
      else
      {
        Kp = KP_RAISEUP;   // CONTROL GAINS FOR RAISE UP
        Kd = KD_RAISEUP;
        Kp_thr = KP_THROTTLE_RAISEUP; 
        Ki_thr = KI_THROTTLE_RAISEUP;
      }   
    }
    else   // Robot not ready, angle > 70º
    {
      setMotorSpeed(0,0);
      setMotorSpeed(1,0);
      PID_errorSum = 0;  // Reset PID I term
      Kp = KP_RAISEUP;   // CONTROL GAINS FOR RAISE UP
      Kd = KD_RAISEUP;
      Kp_thr = KP_THROTTLE_RAISEUP;
      Ki_thr = KI_THROTTLE_RAISEUP;
    }

  }
}

void nRF()
{
  network.update();
  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    receive_a rec;
    network.read(header,&rec,sizeof(rec));

    clock=rec.ms;		//接收主机运行时间赋值
    node_STA=rec.rf_STA;		//接收请求时序赋值
    robot_throttle=rec.rf_throttle;
    robot_steering=rec.rf_steering;

    {
      Serial.print("Sending...");
      send_a sen = {
        millis()
        };	//把这些数据发送出去，对应前面的发送数组

        RF24NetworkHeader header(0);
      boolean ok = network.write(header,&sen,sizeof(sen));
      if (ok)
      {
        Serial.println("ok.");

    steering=map(robot_steering,1000,2000,MAX_STEERING,-MAX_STEERING);  
    throttle=map(robot_throttle,1000,2000,MAX_THROTTLE,-MAX_THROTTLE);  
      }

      else
        Serial.println("failed.");
    }


    safe_ms=millis();
  }
}



