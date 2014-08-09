/**************************************************/
void Normalize(void)
{
  float error=0;
  float temporary[3][3];
  float renorm=0;
  
  error= -Vector_Dot_Product(&DCM_Matrix[0][0],&DCM_Matrix[1][0])*.5; //eq.19

  Vector_Scale(&temporary[0][0], &DCM_Matrix[1][0], error); //eq.19
  Vector_Scale(&temporary[1][0], &DCM_Matrix[0][0], error); //eq.19
  
  Vector_Add(&temporary[0][0], &temporary[0][0], &DCM_Matrix[0][0]);//eq.19
  Vector_Add(&temporary[1][0], &temporary[1][0], &DCM_Matrix[1][0]);//eq.19
  
  Vector_Cross_Product(&temporary[2][0],&temporary[0][0],&temporary[1][0]); // c= a x b //eq.20
  
  renorm= .5 *(3 - Vector_Dot_Product(&temporary[0][0],&temporary[0][0])); //eq.21
  Vector_Scale(&DCM_Matrix[0][0], &temporary[0][0], renorm);
  
  renorm= .5 *(3 - Vector_Dot_Product(&temporary[1][0],&temporary[1][0])); //eq.21
  Vector_Scale(&DCM_Matrix[1][0], &temporary[1][0], renorm);
  
  renorm= .5 *(3 - Vector_Dot_Product(&temporary[2][0],&temporary[2][0])); //eq.21
  Vector_Scale(&DCM_Matrix[2][0], &temporary[2][0], renorm);
}

/**************************************************/
void roll_pitch_drift(void)
{
  //Compensation the Roll, Pitch and Yaw drift. 
  static float Scaled_Omega_P[3];
  static float Scaled_Omega_I[3];
  
  //*****Roll and Pitch***************
  
  Vector_Cross_Product(&errorRollPitch[0],&DCM_Matrix[2][0],&Accel_Vector[0]); //adjust the ground of reference
  Vector_Scale(&Omega_P[0],&errorRollPitch[0],Kp_ROLLPITCH);
  
  Vector_Scale(&Scaled_Omega_I[0],&errorRollPitch[0],Ki_ROLLPITCH);
  Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);
  
  //*****YAW***************
    errorCourse=(DCM_Matrix[0][0]*Heading_Y) - (DCM_Matrix[1][0]*Heading_X);  //Calculating YAW error
 
    Vector_Scale(errorYaw,&DCM_Matrix[2][0],errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.
    
    Vector_Scale(&Scaled_Omega_P[0],&errorYaw[0],Kp_YAW);
    Vector_Add(Omega_P,Omega_P,Scaled_Omega_P);//Adding  Proportional.
    
    Vector_Scale(&Scaled_Omega_I[0],&errorYaw[0],Ki_YAW);
    Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);//adding integrator to the Omega_I   
  
}
/**************************************************/
void accel_adjust(void)
{
 
 Accel_Vector[1]=Accel_Vector[1]-(Accel_Scale(speed_3d)*Omega[2]); 
 Accel_Vector[2]=Accel_Vector[2]+(Accel_Scale(speed_3d)*Omega[1]); 
  
}
/**************************************************/

void Matrix_update(void)
{
  Gyro_Vector[0]=Gyro_Scaled(read_adc(0)); //gyro y roll
  Gyro_Vector[1]=Gyro_Scaled(read_adc(1)); //gyro x pitch
  Gyro_Vector[2]=Gyro_Scaled(read_adc(2)); //gyro Z yaw
  Accel_Vector[0]=read_adc(3);//x
  Accel_Vector[1]=read_adc(4);//y
  Accel_Vector[2]=read_adc(5);//z
  
  //digitalWrite(5,(abs(Gyro_Vector[0])>=ToRad(300))); //Turn on the LED when you saturate the gyro. 
  
  Vector_Add(&Omega[0], &Gyro_Vector[0], &Omega_I[0]);//adding proportional
  Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]);//adding Integrator
  
  accel_adjust();//adjusting centrifugal acceleration.
  
 #if OMEGAA==1
  Update_Matrix[0][0]=0;
  Update_Matrix[0][1]=-G_Dt(Omega_Vector[2]);//-z
  Update_Matrix[0][2]=G_Dt(Omega_Vector[1]);//y
  Update_Matrix[1][0]=G_Dt(Omega_Vector[2]);//z
  Update_Matrix[1][1]=0;
  Update_Matrix[1][2]=-G_Dt(Omega_Vector[0]);//-x
  Update_Matrix[2][0]=-G_Dt(Omega_Vector[1]);//-y
  Update_Matrix[2][1]=G_Dt(Omega_Vector[0]);//x
  Update_Matrix[2][2]=0;
  #endif
  #if OMEGAA==0
    Update_Matrix[0][0]=0;
  Update_Matrix[0][1]=-G_Dt(Gyro_Vector[2]);//-z
  Update_Matrix[0][2]=G_Dt(Gyro_Vector[1]);//y
  Update_Matrix[1][0]=G_Dt(Gyro_Vector[2]);//z
  Update_Matrix[1][1]=0;
  Update_Matrix[1][2]=-G_Dt(Gyro_Vector[0]);
  Update_Matrix[2][0]=-G_Dt(Gyro_Vector[1]);
  Update_Matrix[2][1]=G_Dt(Gyro_Vector[0]);
  Update_Matrix[2][2]=0;
  #endif

  Matrix_Multiply(DCM_Matrix,Update_Matrix,Temporary_Matrix); //a*b=c

  for(int x=0; x<3; x++) //Matrix Addition (update)
  {
    for(int y=0; y<3; y++)
    {
      DCM_Matrix[x][y]+=Temporary_Matrix[x][y];
    } 
  }
}







