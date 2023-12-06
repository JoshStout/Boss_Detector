#include <SPI.h> // library for SPI communication to the ethernet module
#include <Ethernet.h> // library for ethernet module 
#include <Keyboard.h> // library to emulate keyboard 

String readString; // variable for client message 
bool motionDetected = false; // initialize to false 

// mac address and ip address of the ethernet module
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
IPAddress ip(192, 168, 0, 180);

const int serverPort = 4080; // port number of the server 
EthernetServer TCPserver(serverPort); // initialize server 

void setup() {
  // start connection and server 
  Ethernet.begin(mac, ip);
  TCPserver.begin();
}

void loop() {
  EthernetClient client = TCPserver.available(); // listen for clients
  
  // if client connects and no motionDetected previously
  if (client && !motionDetected) {
    char c = client.read(); // read one byte from client  
    readString += c; // add byte to String 
    if (readString.equals("motion detected\n")) {
      motionDetected = true; // must reset ardunio to change back to false 
	  changeWindow(); // change window only once
    }
  }
}

// Switches the active window by holding down the left alt button, 
// presses the Tab key, and releases all buttons.
void changeWindow() {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_TAB);
  Keyboard.releaseAll();
}