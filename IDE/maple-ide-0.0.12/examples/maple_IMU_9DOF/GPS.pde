/****************************************************************
  Here you have all the parsing stuff for uBlox
 ****************************************************************/
 //You have to disable all the other string, only leave this ones:
 
 //NAV-POSLLH Geodetic Position Solution, PAGE 66 of datasheet
 //NAV-VELNED Velocity Solution in NED, PAGE 71 of datasheet
 /*NAV-STATUS Receiver Navigation Status, PAGE 67 of datasheet
 
 GPSfix Type 
 - 0x00 = no fix
 - 0x01 = dead reckonin
 - 0x02 = 2D-fix
 - 0x03 = 3D-fix
 - 0x04 = GPS + dead re
 - 0x05 = Time only fix
 - 0x06..0xff = reserved*/
 
 //Luckly uBlox has internal EEPROM so all the settings you change will remain forever.  Not like the SIRF modules!
 
void init_gps(void)
{
  //pinMode(12, OUTPUT);//Status led
 // Serial1.begin(38400); //Universal Sincronus Asyncronus Receiveing Transmiting 
 // Wait_GPS_Fix();
}

void fast_init_gps(void)
{
  //pinMode(12, OUTPUT);//Status led
  Serial1.begin(38400); //Universal Sincronus Asyncronus Receiveing Transmiting 
}

/****************************************************************
 * 
 ****************************************************************/
void decode_gps(void)
{
  static unsigned long GPS_timer=0; //used to turn off the LED if no data is received. 
  byte UBX_step=0;
  byte UBX_class=0;
  byte UBX_id=0;
  byte UBX_payload_length_hi=0;
  byte UBX_payload_length_lo=0;
  byte UBX_buffer[40];
  byte UBX_ck_a=0;
  byte UBX_ck_b=0;
  byte j=0;

  while(Serial1.available()!=0)//This loop will always run if there is any data available. 
  { 
    switch(UBX_step)//Normally i will start from zero.
    {
    case 0:  
      if(Serial1.read()==0xB5)UBX_step++; //OH first data packet is correct, so jump to the next step
      break; 
    case 1:  
      if(Serial1.read()==0x62){
        UBX_step++;
      } //ooh! The second data packet is correct, jump to the step 2
      else {
        UBX_step=0;
      } //Nop, is not correct so restart to step zero and try again.     
      break;
    case 2:
      wait_for_data(3);
      UBX_class=Serial1.read(); //Now reading and storing message class (read datasheet if you don't know whats going on) 
      UBX_id=Serial1.read(); //Storing ID of the message.
      UBX_payload_length_hi=Serial1.read(); //Storing payload high byte
      UBX_payload_length_lo=Serial1.read(); //Storing payload low byte, not needed its only for checksum
      checksum(UBX_class); //Generating checksum, read PAGE 63 to know all about UBX checksum
      checksum(UBX_id); //the same//
      checksum(UBX_payload_length_hi); // the same
      checksum(UBX_payload_length_lo); // the same about checksum
      for(int c=0; c<UBX_payload_length_hi; c++) //Saving all the payload in the buffer and continue with the checksum
      {
        wait_for_data(0); //I case there is not data in the buffer i will wait
        UBX_buffer[c]=Serial1.read(); //Reading the data from the Serial1 and storing it in the buffer
        checksum(UBX_buffer[c]); //Using the same value i just receive and send it to the checksum "creator"
      }
      wait_for_data(1); //Again waiting for data
      UBX_ck_a=Serial1.read(); //Storing the byte received  for checksum a 
      UBX_ck_b=Serial1.read(); //Storing the byte received for checksum b 

      if((ck_a=UBX_ck_a)&&(ck_b=UBX_ck_a)) //Verify the received checksum with the generated checksum.. 
      {
        //Verifing if we are in class 1, you can change this "IF" for a "Switch" in case you want to use other UBX classes.. 
        //In this case all the message im using are in class 1, to know more about classes check PAGE 60 of DataSheet.
        if(UBX_class==0x01) 
        {            
          switch(UBX_id)//Checking the UBX ID
          {
          case 0x02: //ID NAV-POSLLH 
            //Serial1.println((int)UBX_playload_length);
            j=0;
            //iTOW=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++]); //Storing GPS Millisecond Time of Week
            longUnion.byte[0] = UBX_buffer[j++];
            longUnion.byte[1] = UBX_buffer[j++];
            longUnion.byte[2] = UBX_buffer[j++];
            longUnion.byte[3] = UBX_buffer[j++];
            iTOW=longUnion.dword;

            //lon=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++]); //the same with longitude
            longUnion.byte[0] = UBX_buffer[j++];
            longUnion.byte[1] = UBX_buffer[j++];
            longUnion.byte[2] = UBX_buffer[j++];
            longUnion.byte[3] = UBX_buffer[j++];
            lon=(float)longUnion.dword/(float)10000000;


            //lat=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++]); //latitude
            longUnion.byte[0] = UBX_buffer[j++];
            longUnion.byte[1] = UBX_buffer[j++];
            longUnion.byte[2] = UBX_buffer[j++];
            longUnion.byte[3] = UBX_buffer[j++];
            lat=(float)longUnion.dword/(float)10000000;            

            //alt=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++])/10; //Height above Ellipsoid          
            longUnion.byte[0] = UBX_buffer[j++];
            longUnion.byte[1] = UBX_buffer[j++];
            longUnion.byte[2] = UBX_buffer[j++];
            longUnion.byte[3] = UBX_buffer[j++];
            alt=longUnion.dword;

            //alt_MSL=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++])/10; // Height above the sea level
            longUnion.byte[0] = UBX_buffer[j++];
            longUnion.byte[1] = UBX_buffer[j++];
            longUnion.byte[2] = UBX_buffer[j++];
            longUnion.byte[3] = UBX_buffer[j++];
            alt_MSL=(float)longUnion.dword/(float)1000;

            //hacc=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++])/10; //Horizontal Accuracy Estimate
            /*
              longUnion.byte[0] = UBX_buffer[j++];
             longUnion.byte[1] = UBX_buffer[j++];
             longUnion.byte[2] = UBX_buffer[j++];
             longUnion.byte[3] = UBX_buffer[j++];
             hacc=(float)longUnion.dword/(float)1000;
             
             //vacc=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++])/10; //Vertical Accuracy Estimate
             longUnion.byte[0] = UBX_buffer[j++];
             longUnion.byte[1] = UBX_buffer[j++];
             longUnion.byte[2] = UBX_buffer[j++];
             longUnion.byte[3] = UBX_buffer[j++];
             vacc=(float)longUnion.dword/(float)1000;*/

            data_update_event|=0x01;
            break;
          case 0x03://ID NAV-STATUS 

            if(UBX_buffer[4] >= 0x03)
            {
              gpsFix=0; //valid position
              digitalWrite(6,HIGH);//Turn LED when gps is fixed. 
            }
            else
            {
              gpsFix=1; //invalid position
              digitalWrite(6,LOW);
            }
            break;

          case 0x12:// ID NAV-VELNED 
            j=16;
            //speed_3d=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++]); //Storing Speed (3-D) 
            longUnion.byte[0] = UBX_buffer[j++];
            longUnion.byte[1] = UBX_buffer[j++];
            longUnion.byte[2] = UBX_buffer[j++];
            longUnion.byte[3] = UBX_buffer[j++];
            if(gpsFix==0)
            speed_3d=(float)longUnion.dword/(float)100;//m/s

            //ground_speed=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++]); //Ground Speed (2-D) 
            longUnion.byte[0] = UBX_buffer[j++];
            longUnion.byte[1] = UBX_buffer[j++];
            longUnion.byte[2] = UBX_buffer[j++];
            longUnion.byte[3] = UBX_buffer[j++];
            if(gpsFix==0)
            ground_speed=(float)longUnion.dword/(float)100;

            //heading=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++]); //Heading 2-D 
            longUnion.byte[0] = UBX_buffer[j++];
            longUnion.byte[1] = UBX_buffer[j++];
            longUnion.byte[2] = UBX_buffer[j++];
            longUnion.byte[3] = UBX_buffer[j++];
            if(gpsFix==0)
            ground_course=(float)longUnion.dword/(float)100000;

            /*
            //sacc=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++]); //Speed Accuracy Estimate
             longUnion.byte[0] = UBX_buffer[j++];
             longUnion.byte[1] = UBX_buffer[j++];
             longUnion.byte[2] = UBX_buffer[j++];
             longUnion.byte[3] = UBX_buffer[j++];
             sacc=longUnion.dword;
             
             //cacc=join_4_bytes(UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++],UBX_buffer[j++]); // Heading Accuracy Estimate 
             longUnion.byte[0] = UBX_buffer[j++];
             longUnion.byte[1] = UBX_buffer[j++];
             longUnion.byte[2] = UBX_buffer[j++];
             longUnion.byte[3] = UBX_buffer[j++];
             sacc=longUnion.dword;*/

            data_update_event|=0x02; //Update the flag to indicate the new data has arrived.

            break; 
          }
        }   
      }
      //Restarting variables, flushing buffer.. 
      for(int c=0; c<UBX_payload_length_hi; c++)   
      {
        UBX_buffer[c]=0; 
      }
      UBX_step=0;
      ck_a=0;
      ck_b=0;
      GPS_timer=millis(); //Restarting timer...
      break;  
    }
  }
  if(millis() - GPS_timer > 2000){
      digitalWrite(6, LOW);  //If we don't receive any byte in two seconds turn off gps fix LED... 
      gpsFix=1; 
    }  
}
/****************************************************************
 * 
 ****************************************************************/
void checksum(byte data)
{
  ck_a+=data;
  ck_b+=ck_a; 
}
/****************************************************************
 ****************************************************************/
void wait_for_data(byte many)
{
  while(Serial1.available()<=many); 
}






