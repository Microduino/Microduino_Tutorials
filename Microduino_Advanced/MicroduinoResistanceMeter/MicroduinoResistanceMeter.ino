    #define N 12
    #define basis 20000.0   //Reference resistor

    int potpin = A4;
    float val, r;

    void setup()
    {
        Serial.begin(9600);
    }

    void loop()
    {
        val = analogRead(potpin);
        r = ((basis * 1023.0) / (1023.0 - val)) - basis;
        delay(100);
        Serial.print(" R:  ");
        if(filter() >= 1000)
        {
            if(filter() >= 500.0*1000.0)
            {
                Serial.println("Infinity!!");
            }
            else
            {
                Serial.print(filter() / 1000.0);
                Serial.println("K ohm");
            }
        }
     
        else
        {
            Serial.print(filter());
            Serial.println(" ohm");
        }
    }

	//Filtering algorithm
    float filter()
    {
        float sum = 0;
        for(int count = 0; count < N; count++)
        {
            sum += r;
            delay(5);
        }
        return (float)(sum / N);
    }
