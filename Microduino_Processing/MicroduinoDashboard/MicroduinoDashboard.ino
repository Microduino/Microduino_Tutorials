// Reference the I2C Library
#include <Wire.h>
#include "I2Cdev.h"
// Reference the HMC5883L Compass Library
#include <HMC5883L.h>

// Store our compass as a variable.
HMC5883L compass;
// Record any errors that may occur in the compass.
int error = 0;
//rounding angle
int RoundDegreeInt;
int PreviousDegree = 0;

// Out setup routine, here we will configure the microcontroller and compass.
void setup()
{
  // Initialize the serial port.
  Serial.begin(9600);
  Wire.begin(); // Start the I2C interface.
  compass = HMC5883L(); // Construct a new HMC5883 compass.
  error = compass.SetScale(1.3); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
}

// Our main program loop.
void loop()
{
  // Retrive the raw values from the compass (not scaled).
  MagnetometerRaw raw = compass.ReadRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  // Values are accessed like so:
  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2� 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.009 ;
  heading += declinationAngle;
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI;
  //correcting the angle issue
  if (headingDegrees >= 1 && headingDegrees < 240)
  {
    headingDegrees = map(headingDegrees,0,239,0,179);
  }
  else if (headingDegrees >= 240)
  {
    headingDegrees =  map(headingDegrees,240,360,180,360);
  }
  //rounding the angle
  RoundDegreeInt =round(headingDegrees);
  //smoothing value
  if( RoundDegreeInt < (PreviousDegree + 3) && RoundDegreeInt > (PreviousDegree - 3) ) {
    RoundDegreeInt = PreviousDegree;
  }
  Output(RoundDegreeInt);
  PreviousDegree = RoundDegreeInt;
  // Normally we would delay the application by 66ms to allow the loop
  // to run at 15Hz (default bandwidth for the HMC5883L).
  // However since we have a long serial out (104ms at 9600) we will let
  // it run at its natural speed.
  // delay(66);
}

// Output the data down the serial port.
void Output(int RoundDegreeInt)
{
  //Serial.println();
  Serial.println(RoundDegreeInt);
  delay(150);
}

