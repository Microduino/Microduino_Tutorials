/*--------------------------------------------------------------
  Program:     AnalogVoltmeterProcessong

  Description:  Gets a string containing voltages from an
                Arduino over the USB port and displays the
                voltages in a window.

--------------------------------------------------------------*/
import processing.serial.*;

Serial ser_port;                  // for serial port
char rx_byte;                     // stores single byte received from serial port
String volts[] = {"", "", "", ""};// 4 voltages from the 4 channels
int volts_index = 0;              // current channel
boolean got_volts = false;        // flags received voltage
int graph[][];                    // graph values for each channel
int graph_x = 0;                  // current x position of graph
PFont fnt;                        // for font
// voltage scale for channels, default set to 30V
int ch1_scale = 30;
int ch2_scale = 30;
int ch3_scale = 30;
int ch4_scale = 30;

void setup()
{
    size(600, 450);
    background(0);
    fnt = createFont("Arial", 16, true);
    
    println(Serial.list());
    // modify Serial.list()[0] to select correct serial port
    ser_port = new Serial(this, Serial.list()[0], 9600);
    
    // initialize graph array
    graph = new int[4][400];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 400; j++) {
            graph[i][j] = -1;  // fill graphs with invalid values
        }
    }
}

void draw()
{
    // update voltage in window if voltages received from USB port
    if (got_volts) {
        got_volts = false;
        background (0);
        DrawGraphAxis(160, 28, 400, 80);
        DrawGraphAxis(160, 128, 400, 80);
        DrawGraphAxis(160, 228, 400, 80);
        DrawGraphAxis(160, 328, 400, 80);
        // display voltages
        voltage(1, volts[0]);
        voltage(2, volts[1]);
        voltage(3, volts[2]);
        voltage(4, volts[3]);
        // draw the voltage graphs
        DrawGraph(volts[0], 0, 161, 107, ch1_scale);
        DrawGraph(volts[1], 1, 161, 207, ch2_scale);
        DrawGraph(volts[2], 2, 161, 307, ch3_scale);
        DrawGraph(volts[3], 3, 161, 407, ch4_scale);
        // draw the scale selection radio buttons
        DrawScaleSelect(10, 54, ch1_scale);
        DrawScaleSelect(10, 154, ch2_scale);
        DrawScaleSelect(10, 254, ch3_scale);
        DrawScaleSelect(10, 354, ch4_scale);
    }
}

// called when data has been received on the serial port
void serialEvent(Serial p)
{
    while (p.available() > 0) {
        // get a single character from the serial port
        rx_byte = ser_port.readChar();
        // determine which channel voltage is from
        if (rx_byte == 'A') {
            volts_index = 0;
            volts[volts_index] = "";
        }
        else if (rx_byte == 'B') {
            volts_index = 1;
            volts[volts_index] = "";
        }
        else if (rx_byte == 'C') {
            volts_index = 2;
            volts[volts_index] = "";
        }
        else if (rx_byte == 'D') {
            volts_index = 3;
            volts[volts_index] = "";
        }
        // only use characters 0 to 9 and .
        if ((rx_byte >= '0' && rx_byte <= '9') || (rx_byte == '.')) {
            volts[volts_index] += rx_byte;  // build the voltage string
        }
        // all 4 voltages received?
        if ((rx_byte == 'V') && (volts_index == 3) &&
            (volts[0].length() != 0) && (volts[1].length() != 0) &&
            (volts[2].length() != 0) && (volts[3].length() != 0)) {
            got_volts = true;    // flag received temperature
            //print ("Got voltage: ");    // debug
            //println (volts[volts_index]);      // debug
        }
    }
}

// display the channel number and voltage
void voltage (int channel, String volts)
{
    int y_pos = 40;
    int spacing = 100;
    String txt = "";
    
    textFont(fnt, 18);
    if (channel == 1) {
        fill (255, 0, 0);
        rect (15, y_pos - 20, 110, 25, 7);
        fill(255);
        txt = "CH1: " + volts + " V";
    }
    else if (channel == 2) {
        y_pos += spacing;
        fill (0, 255, 0);
        rect (15, y_pos - 20, 110, 25, 7);
        fill(255);
        txt = "CH2: " + volts + " V";
    }
    else if (channel == 3) {
        y_pos += (spacing * 2);
        fill (0, 0, 255);
        rect (15, y_pos - 20, 110, 25, 7);
        fill(255);
        txt = "CH3: " + volts + " V";
    }
    else if (channel == 4) {
        y_pos += (spacing * 3);
        fill (255, 0, 255);
        rect (15, y_pos - 20, 110, 25, 7);
        fill(255);
        txt = "CH4: " + volts + " V";
    }
    
    text (txt, 20, y_pos);
}

void DrawGraphAxis(int pos_x, int pos_y, int width, int height)
{
    stroke(255);
    line(pos_x, pos_y, pos_x, pos_y + height);      // vertical axis
    line(pos_x, pos_y + height, pos_x + width, pos_y + height);    // horizontal axis
}

void DrawGraph (String voltage, int channel, int pos_x, int pos_y, int scale)
{
    int y_start;  // temporary index into graph array when drawing graph
    
    graph[channel][graph_x] = int(voltage);
    // select colour, depending on channel number
    if (channel == 0) {
        stroke(255, 0, 0);
    }
    else if (channel == 1) {
        stroke(0, 255, 0);
    }
    else if (channel == 2) {
        stroke(0, 0, 255);
    }
    else if (channel == 3) {
        stroke(255, 0, 255);
    }
    y_start = graph_x;    // temporary copy of graph index
    // draw the graph
    for (int i = 0; i < 400; i++) {
        if (graph[channel][y_start] >= 0) {
            int y_val = (graph[channel][y_start] * 80 / scale);
            if (y_val > 80) {
                y_val = 80;
            }
            point(i + pos_x, pos_y - y_val);
        }
        y_start--;
        if (y_start < 0) {
            y_start = 399;
        }
    }
    if (channel == 3) {
        // only increment graph x position on last channel 
        graph_x++;
        if (graph_x >= 400) {
          graph_x = 0;
        }
    }
}

// draw the scale select box with radio buttons
void DrawScaleSelect (int x_pos, int y_pos, int scale)
{
    fill(0);
    stroke(255);
    rect (x_pos, y_pos, 120, 60, 7);  // box around scale selection
    
    fill(255);
    textFont(fnt, 16);
    text ("5V", x_pos + 25, y_pos + 25);
    text ("10V", x_pos + 80, y_pos + 25);
    text ("20V", x_pos + 25, y_pos + 50);
    text ("30V", x_pos + 80, y_pos + 50);
    noStroke();
    fill(255);
    // draw the radio buttons
    ellipse (x_pos + 15, y_pos + 20, 10, 10);
    ellipse (x_pos + 70, y_pos + 20, 10, 10);
    ellipse (x_pos + 15, y_pos + 45, 10, 10);
    ellipse (x_pos + 70, y_pos + 45, 10, 10);
    fill(0);
    // draw the dot in the radio button
    if (scale == 5) {
        ellipse (x_pos + 15, y_pos + 20, 6, 6);
    }
    else if (scale == 10) {
        ellipse (x_pos + 70, y_pos + 20, 6, 6);
    }
    else if (scale == 20) {
        ellipse (x_pos + 15, y_pos + 45, 6, 6);
    }
    else if (scale == 30) {
        ellipse (x_pos + 70, y_pos + 45, 6, 6);
    }
}

// called when mouse button is clicked
// determine which radio button was pressed and change to selected scale
void mousePressed()
{
    // first determine which box click was made in
    // channel 1
    if (mouseX >= 10 && mouseX <= 130 && mouseY >= 54 && mouseY <= 114) {
        // determine which radio button was clicked
        // 5V
        if (mouseX >= 20 && mouseX <= 30 && mouseY >= 69 && mouseY <= 79) {
            ch1_scale = 5;
        }
        // 10V
        else if (mouseX >= 75 && mouseX <= 85 && mouseY >= 69 && mouseY <= 79) {
            ch1_scale = 10;
        }
        // 20V
        else if (mouseX >= 20 && mouseX <= 30 && mouseY >= 94 && mouseY <= 104) {
            ch1_scale = 20;
        }
        // 30V
        else if (mouseX >= 75 && mouseX <= 85 && mouseY >= 94 && mouseY <= 104) {
            ch1_scale = 30;
        }
    }
    // channel 2
    else if (mouseX >= 10 && mouseX <= 130 && mouseY >= 154 && mouseY <= 214) {
        // 5V
        if (mouseX >= 20 && mouseX <= 30 && mouseY >= 169 && mouseY <= 179) {
            ch2_scale = 5;
        }
        // 10V
        else if (mouseX >= 75 && mouseX <= 85 && mouseY >= 169 && mouseY <= 179) {
            ch2_scale = 10;
        }
        // 20V
        else if (mouseX >= 20 && mouseX <= 30 && mouseY >= 194 && mouseY <= 204) {
            ch2_scale = 20;
        }
        // 30V
        else if (mouseX >= 75 && mouseX <= 85 && mouseY >= 194 && mouseY <= 204) {
            ch2_scale = 30;
        }
    }
    // channel 3
    else if (mouseX >= 10 && mouseX <= 130 && mouseY >= 254 && mouseY <= 314) {
        // 5V
        if (mouseX >= 20 && mouseX <= 30 && mouseY >= 269 && mouseY <= 279) {
            ch3_scale = 5;
        }
        // 10V
        else if (mouseX >= 75 && mouseX <= 85 && mouseY >= 269 && mouseY <= 279) {
            ch3_scale = 10;
        }
        // 20V
        else if (mouseX >= 20 && mouseX <= 30 && mouseY >= 294 && mouseY <= 304) {
            ch3_scale = 20;
        }
        // 30V
        else if (mouseX >= 75 && mouseX <= 85 && mouseY >= 294 && mouseY <= 304) {
            ch3_scale = 30;
        }
    }
    // channel 4
    else if (mouseX >= 10 && mouseX <= 130 && mouseY >= 354 && mouseY <= 414) {
        // 5V
        if (mouseX >= 20 && mouseX <= 30 && mouseY >= 369 && mouseY <= 379) {
            ch4_scale = 5;
        }
        // 10V
        else if (mouseX >= 75 && mouseX <= 85 && mouseY >= 369 && mouseY <= 379) {
            ch4_scale = 10;
        }
        // 20V
        else if (mouseX >= 20 && mouseX <= 30 && mouseY >= 394 && mouseY <= 404) {
            ch4_scale = 20;
        }
        // 30V
        else if (mouseX >= 75 && mouseX <= 85 && mouseY >= 394 && mouseY <= 404) {
            ch4_scale = 30;
        }
    }
}
