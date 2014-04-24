    #include <EtherCard.h>
    static byte mymac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};
    static byte myip[] = {192,168,1,107};
    byte Ethernet::buffer[700];
     
    void setup () {
     
      Serial.begin(57600);
      Serial.println("PING Demo");
     
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "\nFailed to access Ethernet controller");
     
      if (!ether.staticSetup(myip))
        Serial.println("Failed to set IP address");
    }
     
    void loop() {
     
      ether.packetLoop(ether.packetReceive());
    }
