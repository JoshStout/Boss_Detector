#include <SPI.h> // library for SPI communication to the ethernet module
#include <Ethernet.h> // library for ethernet module 

const int pir = 7; // PIR sensor attached to pin 7
const int led = 3; // led attached to pin 3

int state = LOW; // initalize as no motion detected 
int val = 0; // variable for reading pin status

// mac address and ip address of the ethernet module 
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0xAA, 0x98 };
IPAddress ip(192, 168, 0, 181);

IPAddress serverAddress(192, 168, 0, 180); // server ip address 
int server_port = 4080; // port number of the server

EthernetClient TCPclient; // initalize client

void setup() {
  pinMode(pir, INPUT); // initialize pin 7 as an input pin
  pinMode(led, OUTPUT); // initalize led pin as an output pin
  Ethernet.begin(mac, ip); // start ethernet connection
}

void loop() {

  // continually check connection to server 
  if (!TCPclient.connected()) {
    TCPclient.stop();
	TCPclient.connect(serverAddress, server_port); 
  }

  // motion detected when pin reading goes from HIGH to LOW
  val = digitalRead(pir); // read pir pin state 
  if (val == HIGH) { // check if sensor is HIGH state to begin reading for motion
    digitalWrite(led, HIGH); // turn on led 
    if (state == LOW) { // sensor detected motion 
      state = HIGH; // update state variable
      TCPclient.write("motion detected\n"); // send message to server
    }
  } else {
    digitalWrite(led, LOW); // turn off led
    if (state == HIGH) { // sensor stopped sensing motion 
      state = LOW; // reset state variable to low 
    }
  }
}