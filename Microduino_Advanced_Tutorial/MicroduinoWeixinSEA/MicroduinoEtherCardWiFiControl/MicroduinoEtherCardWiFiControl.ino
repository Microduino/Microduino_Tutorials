#include <EtherCard.h>
// ethernet interface mac address
static byte mymac[] = {
	0x04,0x69,0x69,0x2b,0x30,0x31
};
// remote website name
char website[] PROGMEM = "1.leirobot.sinaapp.com";

char state = '0';
char c;

int sensrdata;

byte Ethernet::buffer[700]; // a very small tcp/ip buffer is enough here
static long timer;


static void response_callback (byte status, word off, word len) {
//Serial.println((const char*) Ethernet::buffer + off + 207);
//Serial.println((const char*) Ethernet::buffer + off);

	char *s = strstr((char *) Ethernet::buffer + off, "{1}");


	if(strcmp(s,"{1}")==0) {
		state='1';
	} else {
		state='0';
	}
//Serial.println(state);

}
void setup () {
	Serial.begin(9600);
	Serial.println("Client Demo");
	Serial.println();
	if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
		Serial.println( "\nFailed to access Ethernet controller");
	if (!ether.dhcpSetup())
		Serial.println("DHCP failed");
	ether.printIp("IP: ", ether.myip);
	ether.printIp("GW: ", ether.gwip);
	ether.printIp("Netmask: ", ether.netmask);
	ether.printIp("DNS: ", ether.dnsip);
	if (!ether.dnsLookup(website))
		Serial.println("DNS failed");
	ether.printIp("SRV: ", ether.hisip);
// testing the gateway internection
	while (ether.clientWaitingGw())
		ether.packetLoop(ether.packetReceive());
	Serial.println("Gateway found");

	pinMode(7, OUTPUT); 
}
void loop () {

int n = analogRead(A0);    //Read the voltage value of pin A0
sensrdata = n * (5.0 / 1023.0*100);   //Storage temperature data, temperature data obtained from the voltage values ​​are translated

ether.packetLoop(ether.packetReceive());
if (millis() > timer) {

	char stringData[25];
	itoa(sensrdata,stringData,10);

	if(state == '0'){
		digitalWrite(7, LOW);      
	}else if(state == '1'){
		digitalWrite(7, HIGH);
	}

	timer = millis() + 5000;
	ether.browseUrl(PSTR("/downup.php?token=doubleq&data="), stringData, website, response_callback);
}
}
