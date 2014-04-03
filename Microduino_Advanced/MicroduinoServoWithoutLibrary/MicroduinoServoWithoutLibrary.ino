int sp1=7;//define servo digital pin7
int pulsewidth;//Defined variable pulse width
int val;
int val1;
int myangle1;

void servopulse(int sp1,int val1)//Define a pulse function
{
  myangle1=map(val1,0,180,500,2480);
  digitalWrite(sp1,HIGH);//set servo interface HIGH 
  delayMicroseconds(myangle1);
  digitalWrite(sp1,LOW);//set servo interface LOW
  delay(20-val1/1000);
}

void setup()
{
  pinMode(sp1,OUTPUT);//Setting the servo interface output
  Serial.begin(9600);
  delay(500);
  Serial.println("servu=o_seral_simple ready" ) ;
}

void loop()//0-9 the number is converted to an angle from 0 to 180, and so the number of times corresponding LED blinks
{
  val=Serial.read();//Read the serial value of the port
  
  if(val>'0'&&val<='9')
  {
    val1=val-'0';
    val1=map(val1,0,9,0,180);
    Serial.print("moving servo to ");
    Serial.print(val1,DEC);
    Serial.println();
    for(int i=0;i<=50;i++)
    {
      servopulse(sp1,val1);
    }
  }

}
