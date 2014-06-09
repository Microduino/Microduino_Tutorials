int microduinoPins[] = {
  11,2,4,5,6,7,9,10};

boolean segementState[10][8]= {
  {
    1,1,1,0,1,1,1,0  }
  ,
  {
    0,0,1,0,1,0,0,0  }
  ,
  {
    1,1,0,0,1,1,0,1  }
  ,
  {
    0,1,1,0,1,1,0,1  }
  ,
  {
    0,0,1,0,1,0,1,1  }
  ,
  {
    0,1,1,0,0,1,1,1  }
  ,
  {
    1,1,1,0,0,1,1,1  }
  ,
  {
    0,0,1,0,1,1,0,0  }
  ,
  {
    1,1,1,0,1,1,1,1  }
  ,
  {
    0,0,1,0,1,1,1,1  }
};


void setup() {
  // initialize the digital pin as an output.
  for(int i=0;i<8;i++) {
    pinMode(microduinoPins[i], OUTPUT); 
  }
  pinMode(3, INPUT);    //D12 is used to pause switch
}

void loop() {
  for(int j=0;j<10;j++){
    for(int i=0; i<8; i++) {
      digitalWrite(microduinoPins[i], segementState[j][i]);
    }
    delay(500);
    while(digitalRead(3)) {
    }
  }
}






