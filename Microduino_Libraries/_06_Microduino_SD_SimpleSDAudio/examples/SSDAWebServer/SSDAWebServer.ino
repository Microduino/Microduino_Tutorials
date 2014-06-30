/*
  SimpleSDAudio webserver example
  
  Select files to play via webbrowser.
  
  Uses standard Ethernet-Shield with SD card or Ethernet-Arduino.
  Audio output on default pins: 9 for standard, 44 for mega Arduinos. 
 
  Don't use stereo on non-mega Arduinos as a pin collision between
  Eth-CS and second audio output will occur (both pin 10)!
  
  created  01 Jul 2012 by Lutz Lisseck, 
  with help from Ladyada SD webserver example.
  
  Visit our hackerspace website for more information: 
  http://www.hackerspace-ffm.de/wiki/index.php?title=SimpleSDAudio
 */

#include <SPI.h>
#include <Ethernet.h>
#include <SimpleSDAudio.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEB };
IPAddress ip(192,168,2, 222);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);


// We have also a favicon...
const uint8_t favData[] PROGMEM  = {
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x28, 0x01, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00,
	0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0xFE, 0xFF, 0x00, 0xFD, 0xC6, 0x8D, 0x00, 0xB2, 0x4F,
	0x00, 0x00, 0xB1, 0x5C, 0x16, 0x00, 0xB0, 0x51, 0x08, 0x00, 0xFF, 0xF9,
	0xFF, 0x00, 0xB4, 0x51, 0x0D, 0x00, 0xB6, 0x4A, 0x00, 0x00, 0xF8, 0xFD,
	0xFF, 0x00, 0xFB, 0xFF, 0xFC, 0x00, 0xE0, 0x68, 0x04, 0x00, 0xAC, 0x57,
	0x0D, 0x00, 0xA6, 0x5B, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0xCC, 0xCC, 0xCC, 0xCA, 0x5C, 0xCC, 0xCC, 0xCC, 0xCC, 0x5C, 0xCC, 0xCC,
	0x51, 0xCC, 0xCC, 0xC2, 0xC5, 0x5C, 0xBC, 0x1C, 0xC5, 0xCC, 0xCC, 0xCC,
	0x55, 0x5C, 0xCC, 0x55, 0xCA, 0x5C, 0xC1, 0x55, 0x55, 0x5C, 0xC7, 0x45,
	0xCA, 0x5C, 0xC1, 0x55, 0x55, 0x5C, 0x55, 0xC5, 0xAA, 0x5C, 0xC1, 0x55,
	0x55, 0x5C, 0x55, 0xC5, 0xAA, 0x5C, 0xC1, 0x55, 0x58, 0x5C, 0x3C, 0xC5,
	0xCA, 0x5C, 0xCC, 0xCC, 0x59, 0x0C, 0x6C, 0x55, 0xCA, 0x5C, 0xCC, 0xCC,
	0xC5, 0x5C, 0xCC, 0x1C, 0xC5, 0xCC, 0xCC, 0xCC, 0xCC, 0x5C, 0xCC, 0xCC,
	0x51, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCA, 0x5C, 0xCC, 0xCC, 0xCC,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// helper function to determine free ram at runtime
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  SdPlay.init(SSDA_MODE_MONO | SSDA_MODE_FULLRATE);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

EthernetClient *pclient;

void dir_callback(char *buf) { 
  if(pclient) {
    pclient->print("<li><a href=\"");
    pclient->print(buf);
    pclient->println("\">"); 
    pclient->print(buf);
    pclient->println("</a></li>");
  }
}

// How big our line buffer should be. 100 is plenty!
#define BUFSIZ 100
boolean playflag = false;

void loop() {
  char clientline[BUFSIZ];
  int index = 0;
  if(playflag) { SdPlay.play(); playflag = false; }
  SdPlay.worker();
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    pclient = &client;
    index = 0;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        // If it isn't a new line, add the character to the buffer
        if (c != '\n' && c != '\r') {
          clientline[index] = c;
          index++;
          // are we too big for the buffer? start tossing out data
          if (index >= BUFSIZ) 
            index = BUFSIZ -1;
          
          // continue to read more data!
          continue;
        }
        
        // got a \n or \r new line, which means the string is done
        clientline[index] = 0;
        
        // Print it out for debugging
        Serial.println(clientline);
        
        // Look for substring such as a request to get the root file
        if (strstr(clientline, "GET / ") != 0) {
          // send a standard http response header
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/html"));
          client.println();
          
          // print all the files, use a helper to keep it clean
          client.println(F("<a href=\"http://www.hackerspace-ffm.de/wiki/index.php?title=SimpleSDAudio\">SimpleSDAudio</a> V" SSDA_VERSIONSTRING " Webinterface, Free RAM: "));
          client.println(freeRam());
          client.println(F("<br /><h2>Pick file to play:</h2><ul>"));
          SdPlay.dir(&dir_callback);
          client.println(F("</ul>"));
          
        } else if (strstr(clientline, "GET /") != 0) {
          // this time no space after the /, so a sub-file!
          char *filename;
          
          filename = clientline + 5; // look after the "GET /" (5 chars)
          // a little trick, look for the " HTTP/1.1" string and 
          // turn the first character of the substring into a 0 to clear it out.
          (strstr(clientline, " HTTP"))[0] = 0;
          
          // print the file we want
          Serial.println(filename);
          
          if(strstr(filename,"favicon.ico")) {
            // Tx favicon from data block
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: image/x-icon"));
            client.println();  
            for(int i=0;i<sizeof(favData);i++) {
              client.write(pgm_read_byte_near(favData+i));
            }          
          } else {

            if (! SdPlay.setFile(filename)) {
              client.println(F("HTTP/1.1 404 Not Found"));
              client.println(F("Content-Type: text/html"));
              client.println();
              client.println(F("<h2>File Not Found!</h2>"));
              break;
            }
            
            Serial.println(F("Opened!"));
                      
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: text/html"));
            client.println();
            client.println(F("<h2>Now playing...</h2>"));
            playflag = true;
          }

        } else {
          // everything else is a 404
          client.println(F("HTTP/1.1 404 Not Found"));
          client.println(F("Content-Type: text/html"));
          client.println();
          client.println(F("<h2>File Not Found!</h2>"));
        }
        break;
      }        
        
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    pclient = NULL;
    Serial.println("client disonnected");
  }
}

