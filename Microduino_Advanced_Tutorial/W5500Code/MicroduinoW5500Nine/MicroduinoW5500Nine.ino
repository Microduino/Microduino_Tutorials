#include <SPI.h>
#include <Ethernet.h>

#define buffer_NUM 200

#define LED_PIN 2
char* on = "ON";
char* off = "OFF";
boolean ledStatus;
char* ledLabel;
char* linkLabel;

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

void setup () {
	Serial.begin(115200);
	// start the Ethernet connection and the server:
	Ethernet.begin(mac, ip, gateway, subnet);
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
	ledStatus = false;
	ledLabel = off;
	linkLabel = on;
}

void loop () {
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


boolean Ethernet_data() {
	if(strstr((char *)buffer, "GET /?ON") != 0) {
		ledStatus = true;
		ledLabel = on;
		linkLabel = off;
	} else if(strstr((char *)buffer, "GET /?OFF") != 0) {
		ledStatus = false;
		ledLabel = off;
		linkLabel = on;
	}
	digitalWrite(LED_PIN, ledStatus);
}



void Ethernet_post() {
  static char buf[500];
  client.println("HTTP/1.0 200 OK");
  client.println("Content-Type: text/html\r\nPragma: no-cache\r\n");
  client.println("<html><head><title>WebLed</title></head><body>");
  sprintf(buf, "<div style='position:absolute;width:200px;height:200px;top:100px;left:100px;margin:-100px 0 0 -100px'><div style='font:bold 14px verdana;text-align:center'>Relay is %s</div><br><div style='text-align:center'><a href='/?%s'><img src='http://www.lucadentella.it/files/bt_%s.png'></a></div></div></body></html>", 
  	ledLabel, linkLabel, linkLabel);
  client.println(buf);
    for(int i=0; i<500; i++){
    buf[i]=NULL;
  }
}
