#include <SPI.h>
#include <Ethernet.h>
#define buffer_NUM 200
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,199,123);
IPAddress gateway(192,168,199, 1);
IPAddress subnet(255, 255, 255, 0);
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
// listen for incoming clients
EthernetClient client;
byte buffer[buffer_NUM];
const int outletPin = 6;
boolean ledStatus;
char* on = "ON";
char* off = "OFF";
char* statusLabel;
char* buttonLabel=on;
void setup() {
// Open serial communications and wait for port to open:
	Serial.begin(115200);
	while (!Serial) {
; // wait for serial port to connect. Needed for Leonardo only
}
// start the Ethernet connection and the server:
Ethernet.begin(mac, ip, gateway, subnet);
server.begin();
Serial.print("server is at ");
Serial.println(Ethernet.localIP());
pinMode(outletPin, OUTPUT);
digitalWrite(outletPin, LOW);
ledStatus = false;
}
void loop() {
	word len;
// listen for incoming clients
	client = server.available();
	if (client) {
		Serial.println("new client");
// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				buffer[len] = c;
				len++;
				if (c == '\n' && currentLineIsBlank) {
					Ethernet_data();
					Ethernet_post();
					break;
				}
				if (c == '\n') {
// you're starting a new line
					currentLineIsBlank = true;
				}
				else if (c != '\r') {
// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}
		len=0;
		for(int i=0; i<buffer_NUM; i++)
			Serial.write(buffer[i]);
// give the web browser time to receive the data
		delay(1);
// close the connection:
		client.stop();
		Serial.println("client disonnected");
	}
}
boolean Ethernet_data()
{
	if(strstr((char *)buffer, "GET /?status=ON") != 0) {
		Serial.println("Received ON command");
		ledStatus = true;
	}
	if(strstr((char *)buffer, "GET /?status=OFF") != 0) {
		Serial.println("Received OFF command");
		ledStatus = false;
	}
	if(ledStatus) {
		digitalWrite(outletPin, HIGH);
		statusLabel = on;
		buttonLabel = off;
	} else {
		digitalWrite(outletPin, LOW);
		statusLabel = off;
		buttonLabel = on;
	}
}
void Ethernet_post()
{
	static char buf[200];
	// client.println("HTTP/1.0 200 OK");
	// client.println("Content-Type: text/html\r\nPragma: no-cache\r\n");

	  client.println("HTTP/1.1 200 OK");
	  client.println("Content-Type: text/html");
	  client.println("Connection: close");  // the connection will be closed after completion o
      client.println();
      client.println("<!DOCTYPE HTML>");


	client.println("<html><head><title>WebLed</title></head>");
	//sprintf(buf, "<body>LED Status: %s <p align=\"center\"><a href=\"/?status=%s\"><input type=\"button\" value=\"%s\"></a></p></body></html>", statusLabel, buttonLabel, buttonLabel);
	sprintf(buf, "<body>LED Status: %s <p align=\"center\"><a href=\"/?status=%s\"><img src=\"http://aiwanfund.com.cn/images/button%s.png\" /></a></p></body></html>", statusLabel, buttonLabel, buttonLabel);
	client.println(buf);
	for(int i=0; i<100; i++){
		buf[i]=NULL;
	}
}
