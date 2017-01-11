/*
 WiFiEsp example: WiFi UDP Send and Receive String

 This sketch wait an UDP packet on localPort using a WiFi shield.
 When a packet is received an 'ACK' packet is sent to the client on port remotePort.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/

#include <WiFiEsp.h>
#include <WiFiEspUdp.h>

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(8, 9); // RX, TX
#endif

char ssid[] = /*SSID HERE*/;     // your network SSID (name)
char pass[] = /*PASSWORD HERE*/;  // your network password
int status = WL_IDLE_STATUS; // the Wifi radio's status

unsigned int localPort = 3333; // local port to listen on

char packetBuffer[255];         // buffer to hold incoming packet
char ReplyBuffer[] = "ACK";     // a string to send back

WiFiEspUDP Udp;

// Robot drive pins/defines
// Pin assignments
#define AIN1 3
#define AIN2 4
#define APWM 5
#define BIN1 12
#define BIN2 13
#define BPWM 11
#define STBY 6

// Constants for motor control functions
#define STEPTIME 600
#define STRAIGHTSPEED 200
#define TURNSPEED 120
#define TURNTIME 300

void setup() {
  // ROBOT SETUP
  // Initialize pins as outputs
  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(APWM, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BPWM, OUTPUT);
  
  // Initialize serial for debugging
  Serial.begin(115200);
  // Initialize serial for ESP module
  Serial1.begin(9600);
  // Initialize ESP module
  WiFi.init(&Serial1);

  // check for the presense of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while(status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    //Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);

  Serial.print("Listening on port ");
  Serial.println(localPort);
}

void loop() {
  // if there's data available, read a packet
  //Serial.println("Checking for packet...");
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    populateUdpData(packetSize);
    processUdpRequest();

    // Send a reply to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}

void populateUdpData(int packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // Read the packet into packetbuffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);
}

void processUdpRequest() {
        // Use a switch statement to examine the first character of the packet buffer
        switch(packetBuffer[0]){
            case 'w':
                // If the value is 'w', move the robot forward
                goForward();
                break;
            case 'a':
                // If the value is 'a', move the robot left
                goLeft();
                break;
            case 's':
                // If the value is 's', move the robot backward
                goBack();
                break;
            case 'd':
                // If the value is 'd', move the robot right
                goRight();
                break;
            case 'x':
                // If the value is 'x', stop the robot
                stop();
                break;
        }
  
      // Zero out the buffer until we get a new one
    packetBuffer[0] = 0;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

//////////////// ROBOT CONTROL CODE

// Array to track current PWM values for each channel (A and B)
int pwms[] = {APWM, BPWM};

// Offsets to be used to compensate for one motor being more powerful
byte leftOffset = 0;
byte rightOffset = 0;

// Variable to track remaining time
unsigned long pwmTimeout = 0;

// Function to write out pwm values
void writePwms(int left, int right) {
  analogWrite(pwms[0], left);
  analogWrite(pwms[1], right);
}

// Move the robot forward for STEPTIME
void goForward() {
  digitalWrite(STBY, HIGH); // STBY??
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  writePwms(STRAIGHTSPEED-leftOffset, STRAIGHTSPEED - rightOffset);
  pwmTimeout = millis() + STEPTIME;
}

// Move the robot backward for STEPTIME
void goBack() {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  writePwms(STRAIGHTSPEED - leftOffset, STRAIGHTSPEED - rightOffset);
  pwmTimeout = millis() + STEPTIME;
}

// Turn the robot left for TURNTIME
void goLeft() {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  
  writePwms(TURNSPEED, TURNSPEED);
  pwmTimeout = millis() + TURNTIME;
}

// Turn the robot right for TURNTIME
void goRight() {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  
  writePwms(TURNSPEED, TURNSPEED);
  pwmTimeout = millis() + TURNTIME;
}

// Stop the robot (using standby)
void stop() {
  digitalWrite(STBY, LOW);
}

