/*--------------------------------------------------------------
  Program:      AnaloVoltmeter

  Description:  4 channel software voltmeter that displays
                voltage readings in a Processing application
                running on a computer.
  
  Hardware:     Microduino with voltage dividers on A2 to A5.
                
  Software:     Developed using Arduino 1.0.5 software
                Should be compatible with Arduino 1.0 +
                voltmeter Processing software runs on PC
--------------------------------------------------------------*/

// number of analog samples to take per reading, per channel
#define NUM_SAMPLES 10
// voltage divider calibration values
#define DIV_1    11.1346
#define DIV_2    11.1969
#define DIV_3    11.0718
#define DIV_4    11.0718
// ADC reference voltage / calibration value
#define V_REF    4.991

int sum[4] = {0};                // sums of samples taken
unsigned char sample_count = 0;  // current sample number
float voltage[4] = {0.0};        // calculated voltages
char l_cnt = 0;                  // used in 'for' loops

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    // take a number of analog samples and add them up
    while (sample_count < NUM_SAMPLES) {
        // sample each channel A2 to A5
        for (l_cnt = 0; l_cnt < 4; l_cnt++) {
            sum[l_cnt] += analogRead(A2 + l_cnt);
        }
        sample_count++;
        delay(10);
    }
    // calculate the voltage for each channel
    for (l_cnt = 0; l_cnt < 4; l_cnt++) {
        voltage[l_cnt] = ((float)sum[l_cnt] / (float)NUM_SAMPLES * V_REF) / 1024.0;
    }
    
    // each voltage is multiplied by the resistor network
    // division factor to calculate the actual voltage
    voltage[0] = voltage[0] * DIV_1;
    voltage[1] = voltage[1] * DIV_2;
    voltage[2] = voltage[2] * DIV_3;
    voltage[3] = voltage[3] * DIV_4;
    
    // send voltages to Processing application via serial port / USB
    // voltage 1 - A (pin A2)
    Serial.print("A ");
    Serial.print(voltage[0], 1);
    Serial.print("V ");
    // voltage 2 - B (pin A3)
    Serial.print("B ");
    Serial.print(voltage[1], 1);
    Serial.print("V ");
    // voltge 3 - C (pin A4)
    Serial.print("C ");
    Serial.print(voltage[2], 1);
    Serial.print("V ");
    // voltage 4 - D (pin A5)
    Serial.print("D ");
    Serial.print(voltage[3], 1);
    Serial.print("V ");
    Serial.println("");
    delay(10);
    // reset count and sums
    sample_count = 0;
    for (l_cnt = 0; l_cnt < 4; l_cnt++) {
        sum[l_cnt] = 0;
    }
}
