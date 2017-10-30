/*
 WiFiEsp example: WebClient
 This sketch connects to google website using an ESP8266 module to
 perform a simple web search.
 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/

#include "WiFiEsp.h"
#include <HX711.h>
HX711 scale(A2, A3);
//const sixth = 10 //needs updating
//const 

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "FiOS-N9CWZ";            // your network SSID (name)
char pass[] = "soy8879ski4533flat";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "maker.ifttt.com";

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  printWifiStatus();

  //Serial.println();
  //Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  //if (client.connect(server, 80)) {
    //Serial.println("Connected to server");
    // Make a HTTP request
 //   String content = "trigger/half_full/with/key/c9z7GybuXNU0d69DfNfyIs";
 //   client.println("GET /trigger/half_full/with/key/c9z7GybuXNU0d69DfNfyIs HTTP/1.1");
 //   client.println("Host: maker.ifttt.com");
 //   client.println("Accept: */*");
 //   client.println("Content-Length: " + content.length());
 //   client.println("Content-Type: text/html; charset=utf-8");
 //   client.println();
 //   client.println(content);
  //}
  
  
}

void loop()
{


//  if (scale.getGram() >=100){
//    Serial.print("half full at ");
   // notify();
//  }
  Serial.print(scale.get_scale(), 1);
  Serial.println(" lbs");
  Serial.print(scale.get_units(10), 1);
  Serial.println(" units");
  delay(100);
  
  // if there are incoming bytes available
  // from the server, read them and print them
 // while (client.available()) {
 //   char c = client.read();
 //   Serial.write(c);
  //}

  // if the server's disconnected, stop the client
//  if (!client.connected()) {
//    Serial.println();
//    Serial.println("Disconnecting from server...");
//    client.stop();
//
    // do nothing forevermore
//    while (true);
//  }
}

void notify()
{  if (client.connect(server, 80)) {
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
void scalesetup(){
  for (int i;i<20;i++){
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC
  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));    // print the average of 20 readings from the ADC
  delay(500);
  }
  Serial.print("Setting Scale - ");
  scale.set_scale();
  Serial.println("DONE");
  Serial.print("Tare-ing");
  scale.tare();
  Serial.println(" DONE ");
  Serial.println("Set wieght on scale");
  delay(2500);
  Serial.println("[Calibrating]");
  Serial.print("Getting Units... ");
  Serial.println(scale.get_units(10));
  scale.set_scale( scale.get_units(10)/200 );
  for (int i=0;i<10;i++){
  Serial.print("Load \t");
  Serial.println(scale.get_units(10));
  Serial.println("  lbs");
  delay(500);
  }
}
}

