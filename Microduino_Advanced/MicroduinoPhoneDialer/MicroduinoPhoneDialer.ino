#include <Tone.h>

String Phone_Number = "";
int i = 0, mark = 0;

//Defined freq1, freq2 as Tone instance, the definition of dual tone multi-frequency and frequency
//DTMF See the definition of frequency：[url]http://zh.wikipedia.org/zh/%E5%8F%8C%E9%9F%B3%E5%A4%9A%E9%A2%91[/url]
Tone freq1;
Tone freq2;
const int DTMF_freq1[] = {1336, 1209, 1336, 1477, 1209, 1336, 1477, 1209, 1336, 1477};
const int DTMF_freq2[] = {941,  697,  697,  697,  770,  770,  770,  852,  852,  852};

void setup()
{
	Serial.begin(9600);
    //Define the sound generated Microduino pin D11, D12
    freq1.begin(11);
    freq2.begin(12);
}

void loop()
{
    //Reads the serial data string concatenation into Phone_Number
    while (Serial.available() > 0)
    {
    	Phone_Number += char(Serial.read());
    	delay(2);
    	mark = 1;
    }
    //Play DTMF tones, phone numbers from Phone_Number, continuous length of 200ms, 300ms interval length
    PlayDTMF(Phone_Number, 200, 300);
    
    if(mark == 1)
    {
       Phone_Number = "";
       Serial.println();
       mark = 0;
   }
}


void PlayDTMF(String Number, long duration, long pause)
{
    if(Number.length() == 0 || duration <= 0 || pause <= 0) return;
    for(i = 0; i < Number.length(); i++)
    {
        if(Number[i] >= '0' && Number[i] <= '9')
        {
            Number[i] -= '0';
            Serial.print(Number[i], DEC);
            //output dual-tone multi-frequency1
            freq1.play(DTMF_freq1[Number[i]], duration);
            //output dual-tone multi-frequency2
            freq2.play(DTMF_freq2[Number[i]], duration);
            delay(pause);
        }
    }

}
