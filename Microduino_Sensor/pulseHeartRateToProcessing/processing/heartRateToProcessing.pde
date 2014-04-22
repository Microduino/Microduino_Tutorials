    import processing.serial.*;
     
    Serial serial;  
    int sensorValue;
     
    void setup() {
      // Set the canvas size 305 x 200
      size(305, 200); 
     
      // Open the serial port, set the baud rate to 9600
      // The experiment arduino control panel is connected to COM6 port here Please adjusted according to the actual situation.
      serial = new Serial(this, "COM6", 9600);
    }
     
    void draw() {
      if ( serial.available() > 0) {
        // Read from the serial production over the Sensor value.
        sensorValue = serial.read()+25; 
        println(sensorValue);
     
        // In a canvas painting
        background(255); // white background
        fill(255,0,0);   // fill red color in circle
        ellipse(sensorValue, 100, 50, 50);
      }  
    }