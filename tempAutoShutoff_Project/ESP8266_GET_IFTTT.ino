/*
 WiFiEsp example: WebClient
 This sketch connects to google website using an ESP8266 module to
 perform a simple web search.
 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/


/* 
 Include the libraries to communicate with the WiFi Module
 This will allow us to talk to the ESP8266 with easy to use 
 commands.
*/
#include "WiFiEsp.h"


/* 
 Not sure why or if we even need this...
 Going to leave it in for now in case.
 Pay no attention
*/
// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

/*
 Lets set some Globals
 Note: Generally Globals (Variables defined outside of a specific function
       Are not goog to use.  We use them here because it is small code and 
       These variables will not be used again for any reason.
*/
char ssid[] = "FiOS-N9CWZ";            	// your network SSID (name)
char pass[] = "soy8879ski4533flat";     // your network password
int status = WL_IDLE_STATUS;     	// the Wifi radio's status
char server[] = "maker.ifttt.com"; 	// The IFTTT site (This was taken from documentation from IFTTT.com instructions)

/* 
 Initialize the Ethernet client object.  
 This is for the WiFi Module.  It says 
 turn on, initializat, etc.. basic stuff
*/
WiFiEspClient client;

/************************************************************************************
 Setup a few things
 1) Set up the Serial so the Arduino can talk to your computer 
    and you can debug using the Arduino serial monitor.
 2) Further initialize the ESP8266 Module
   a) connect to your WiFi 
   b) make sure your WiFi can reach the internet (in this case the IFTTT site above)
 3) (NOT IMPLEMENTED YET) Initialize the temp sensor
************************************************************************************/
void setup()
{
  Serial.begin(115200);		// initizliat Serial for debugging
  Serial1.begin(9600);		// initialize serial for ESP module
  WiFi.init(&Serial1);		// initialize ESP module

 /*
  Not sure what this is for...
  leave it in for now might take out later.
  ignore for now.
 */
 // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

 /* 
  attempt to connect to WiFi network
 */
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);				// connect to WPA/WPA2 network
  }

  Serial.println("You're connected to the network");		// you're connect now, so print out the data

  printWifiStatus(); 						// print the wifi status.  this basically does a Serial.println somewhere inside this function after talking to the ESP8266

}




/***********************************************************************************
 This is the main loop that runs on the Arduino
************************************************************************************/
void loop()
{
  if (sensor.temp){


}


/*
 This is going the be the function that talks to IFTTT once the temperature reaches a certain level
*/
void notify()
{  
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    String content = "trigger/half_full/with/key/c9z7GybuXNU0d69DfNfyIs";
    client.println("GET /trigger/half_full/with/key/c9z7GybuXNU0d69DfNfyIs HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Accept: */*");
    client.println("Content-Length: " + content.length());
    client.println("Content-Type: text/html; charset=utf-8");
    client.println();
    client.println(content);
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

