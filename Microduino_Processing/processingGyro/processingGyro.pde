import processing.serial.*;
  
Serial myPort;  //myPort
  
boolean firstSample = true;
  
float [] RwAcc = new float[3];         // Projection of the gravity acceleration sensor on x / y / z-axis
float [] Gyro = new float[3];          //Read gyroscope
float [] RwGyro = new float[3];        //Re-read the gyroscope
float [] Awz = new float[2];           //Angle XZ / YZ plane and the Z-axis (in degrees) R between the projection
float [] RwEst = new float[3];
  
  
int lastTime = 0;
int interval = 0;
float wGyro = 10.0;
  
int lf = 10; //10 represents '\ n' in the ASCII table
byte[] inBuffer = new byte[100];
  
PFont font;
final int VIEW_SIZE_X = 600, VIEW_SIZE_Y = 600;
  
  
void setup()
{
  size(VIEW_SIZE_X, VIEW_SIZE_Y, P3D);
  
  myPort = new Serial(this, Serial.list()[0], 9600); // first COM port
  
  // set font
  font = loadFont("ArialMT-25.vlw");
}
  
  
void readSensors() {
  if (myPort.available() > 0) {
    if (myPort.readBytesUntil(lf, inBuffer) > 0) {
      String inputString = new String(inBuffer);
      String [] inputStringArr = split(inputString, ',');
  
      //The raw data is converted to G
      RwAcc[0] = float(inputStringArr[0]) / 16384.0;
      RwAcc[1] = float(inputStringArr[1])/ 16384.0;
      RwAcc[2] = float(inputStringArr[2])/ 16384.0;
  
      //The raw data is converted to "degrees / sec."
      Gyro[0] = float(inputStringArr[3]) / 131.0;
      Gyro[1] = float(inputStringArr[4]) / 131.0;
      Gyro[2] = float(inputStringArr[5]) / 131.0;
    }
  }
}
  
  
void normalize3DVec(float [] vector) {
  float R;
  R = sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
  vector[0] /= R;
  vector[1] /= R; 
  vector[2] /= R;
}
  
  
float squared(float x) {
  return x*x;
}
  
  
void buildBoxShape() {
  //box(60, 10, 40);
  noStroke();
  beginShape(QUADS);
  
  //Z+  (drawing area)
  fill(#00ff00);
  vertex(-30, -5, 20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
  
  //Z-
  fill(#0000ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, 5, -20);
  vertex(-30, 5, -20);
  
  //X-
  fill(#ff0000);
  vertex(-30, -5, -20);
  vertex(-30, -5, 20);
  vertex(-30, 5, 20);
  vertex(-30, 5, -20);
  
  //X+
  fill(#ffff00);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(30, 5, -20);
  
  //Y-
  fill(#ff00ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(-30, -5, 20);
  
  //Y+
  fill(#00ffff);
  vertex(-30, 5, -20);
  vertex(30, 5, -20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
  
  endShape();
}
  
  
void drawCube() { 
  pushMatrix();
  translate(300, 450, 0);
  scale(4, 4, 4);
  
  rotateX(HALF_PI * -RwEst[0]);
  rotateZ(HALF_PI * RwEst[1]);
  
  buildBoxShape();
  
  popMatrix();
}
  
  
void getInclination() {
  int w = 0;
  float tmpf = 0.0;
  int currentTime, signRzGyro;
  
  
  readSensors();
  normalize3DVec(RwAcc);
  
  currentTime = millis();
  interval = currentTime - lastTime;
  lastTime = currentTime;
  
  if (firstSample || Float.isNaN(RwEst[0])) { // NaN used to wait to check over the data from Microduino
    for (w=0;w<=2;w++) {
      RwEst[w] = RwAcc[w];    //Initialization acceleration sensor readings
    }
  }
  else {
    //Assessed on RwGyro
    if (abs(RwEst[2]) < 0.1) {
      //Rz value is very small, its role is as calculated by reference to the value of Axz and Ayz prevent amplification of fluctuations produce erroneous results.
      //In this case the current gyro data is skipped, the previous use.
      for (w=0;w<=2;w++) {
        RwGyro[w] = RwEst[w];
      }
    }
    else {
      //Angle ZX / ZY plane and the Z axis between the projection R, based on the most recent value RwEst
      for (w=0;w<=1;w++) {
        tmpf = Gyro[w];                        //Get the current gyroscope deg / s
        tmpf *= interval / 1000.0f;                     //Get the angle changes
        Awz[w] = atan2(RwEst[w], RwEst[2]) * 180 / PI;   //Get angle and convert to degrees
        Awz[w] += tmpf;             //Depending on the angle of movement of the gyroscope is updated after
      }
  
      //Determine how much RzGyro, mainly to see how much curvature Axz
      //When Axz at -90 .. 90 => cos (Awz)> = 0 when within this range is accurate RzGyro
      signRzGyro = ( cos(Awz[0] * PI / 180) >=0 ) ? 1 : -1;
  
      //From the perspective of the value equation Awz reverse calculation RwGyro Please check website [url] http://starlino.com/imu_guide.html [/ url]
      for (w=0;w<=1;w++) {
        RwGyro[0] = sin(Awz[0] * PI / 180);
        RwGyro[0] /= sqrt( 1 + squared(cos(Awz[0] * PI / 180)) * squared(tan(Awz[1] * PI / 180)) );
        RwGyro[1] = sin(Awz[1] * PI / 180);
        RwGyro[1] /= sqrt( 1 + squared(cos(Awz[1] * PI / 180)) * squared(tan(Awz[0] * PI / 180)) );
      }
      RwGyro[2] = signRzGyro * sqrt(1 - squared(RwGyro[0]) - squared(RwGyro[1]));
    }
  
    //The gyro and the acceleration sensor are combined value
    for (w=0;w<=2;w++) RwEst[w] = (RwAcc[w] + wGyro * RwGyro[w]) / (1 + wGyro);
  
    normalize3DVec(RwEst);
  }
  
  firstSample = false;
}
  
  
void draw() { 
  getInclination();
  
  background(#000000);
  fill(#ffffff);
  
  textFont(font, 20);
  //float temp_decoded = 35.0 + ((float) (temp + 13200)) / 280;
  //text("temp:\n" + temp_decoded + " C", 350, 250);
  text("RwAcc (G):\n" + RwAcc[0] + "\n" + RwAcc[1] + "\n" + RwAcc[2] + "\ninterval: " + interval, 20, 50);
  text("Gyro (°/s):\n" + Gyro[0] + "\n" + Gyro[1] + "\n" + Gyro[2], 220, 50);
  text("Awz (°):\n" + Awz[0] + "\n" + Awz[1], 420, 50);
  text("RwGyro (°/s):\n" + RwGyro[0] + "\n" + RwGyro[1] + "\n" + RwGyro[2], 20, 180);
  text("RwEst :\n" + RwEst[0] + "\n" + RwEst[1] + "\n" + RwEst[2], 220, 180);
  
  // display axes
  pushMatrix();
  translate(450, 250, 0);
  stroke(#ffffff);
  scale(100, 100, 100);
  line(0, 0, 0, 1, 0, 0);
  line(0, 0, 0, 0, -1, 0);
  line(0, 0, 0, 0, 0, 1);
  line(0, 0, 0, -RwEst[0], RwEst[1], RwEst[2]);
  popMatrix();
  
  drawCube();
}
