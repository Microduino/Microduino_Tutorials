    /*
     useage: To introduce the use of microduino analog port to measure voltage, 0-5V voltage as a table using the experimental
    */
     
    float temp;   //Create a float variable temp as a storage space to store data preparation
    void setup()
    {
      Serial.begin(9600);
    }
    void loop()
    {
     
      int V1 = analogRead(A0);                    
    //
A0 reads the voltage from the mouth into the newly created data type integer variables V1, analog voltage measurement range of ports 0-5V return value 0-1024
      float vol = V1*(5.0 / 1023.0);               
    //We will be converted into the actual voltage value V1 value into a float variable vol
      if (vol == temp)                             
    //This part of the judgment is used to filter duplicate data, only the second and last time varying voltage value when output
      {
        temp = vol;
      }
      else
      {
        Serial.print(vol);                       //Serial output voltage value, and does not wrap
        Serial.println(" V");                    //Serial output character V, and wrap
        temp = vol;
        delay(1000);                           //Wait a second output after the completion, the refresh rate for the control data.
      }
    }
