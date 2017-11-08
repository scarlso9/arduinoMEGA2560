/*
For details on the ESP8266 code see http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
For detail on the temperature sensor code see https://github.com/UCTRONICS/uctronics_arduino_kits

This code will make an arduino controller that monitors temperature and triggers an IFTTT recipe 
that will be connected to a samsung smart things power outlet to turn on and off a heater

To get this working other than this code you will also need to create an IFTTT recipe 
for the recipe 
THIS = the webhook module that recieves GET requests
THAT = control smasung smart thigs

You will need to create two recipes, one for turning the samsung smart things on and one for turning it off

See http://IFTTT.com/ for more info
*/


/* 
 Include the libraries to communicate with the WiFi Module
 This will allow us to talk to the ESP8266 with easy to use 
 commands.
 when you click compile (the checkmark or right arrow) one of 
 the first things it does is check that it actually can find 
 the files you #include, and then it looks at those files
 (they have their own set of #includes) and makes sure their
 includes are there too.  From there the compiler usually
 goes and check the syntax, then makes sure the functions you
 are calling actually exist.  then after a few more checks it
 compiles your code into 1's and 0's. 
*/
#include "WiFiEsp.h"
#include <LiquidCrystal.h>
#include <dht11.h>


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
 Define which pins the LCD is connected to
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
*/
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);  //Define the connection LCD pin  
dht11 DHT11;
int DHT11PIN = 18;

/*
 Lets set some Globals
 Note: Generally Globals (Variables defined outside of a specific function)
       Are not goog to use.  We use them here because it is small code and 
       These variables will not be used again for any reason.
*/
char ssid[] = "FiOS-N9CWZ";            	// your network SSID (name)
char pass[] = "soy8879ski4533flat";     // your network password
int status = WL_IDLE_STATUS;     	// the Wifi radio's status
char server[] = "maker.ifttt.com"; 	// The IFTTT site (This was taken from documentation from IFTTT.com instructions)
const char content[] = "GET trigger/half_full/with/key/c9z7GybuXNU0d69DfNfyIs HTTP/1.1";               // this is the specific URL link the request is sent to (imagine a url made up of server/content) that 
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
  lcd.begin(16, 2);       // set up the LCD's number of columns and rows: 
  lcd.clear();            // clears the LCD screen and positions the cursor in the upper-left corner 
  Serial1.begin(9600);		// initialize serial for ESP module
  WiFi.init(&Serial1);		// initialize ESP module
  delay(2000);            // delay for 2s to let things initialize
  checkForShield();       // pretty sure we don't need this
  tryConnectToWiFi();     // attempt to connect to the WiFi network
  Serial.println("You're connected to the network");		// you're connect now, so print out the data
  printWifiStatus(); 						// print the wifi status.  this basically does a Serial.println somewhere inside this function after talking to the ESP8266
  checkDHT11Sensor();     // just check the sensor is working and connected.  should print the humidty and temperature to lcd screen
}

/***********************************************************************************
 This is the main loop that runs on the Arduino
 
 Here is some psuedo code that should help start what you want to do
 
 create a flag variable to tell what state the outlet is in (On/Off) and initialize it


 inside of another infinite loop do the following
 -print the temperature status to the lcd screen
 -print the flag status to the lcd screen 
 -get the temperature
 -if the temp is above a certain level and flag is false
   -notify IFTTT it is above the threshholda and it needs to turn off the outlet
   -set the flag to true so the program knows the system is off
 -if it is not above a certain level and the flag is true
   -notify IFTTT it is below the threshhold and it needs to turn back on the outlet
   -set the flag to off
************************************************************************************/
void loop()
{
  
  // Your Code Here

}


/*
 This is going the be the function that talks to IFTTT once the temperature reaches a certain level
*/
void notify()
{  
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    client.println(content);  // initialize the http request
    client.println("Host: maker.ifttt.com");
    client.println("Accept: */*");
    client.println("Content-Length: " + sizeof(content));
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

void checkForShield() 
{
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
}

void tryConnectToWiFi()
{
    while ( status != WL_CONNECTED) {          //   attempt to connect to WiFi network
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);        // connect to WPA/WPA2 network
  }
}
double Fahrenheit(double celsius) 
{
        return 1.8 * celsius + 32;
}   
double Kelvin(double celsius)
{
        return celsius + 273.15;
}    
void checkDHT11Sensor()
{
    int chk = DHT11.read(DHT11PIN);
//    lcd.setCursor(0, 0); // set the cursor to column 0, line 0
//    lcd.print("Humidity:");// Print a message of "Humidity: "to the LCD.
//    lcd.print((float)DHT11.humidity, 2);// Print a message of "Humidity: "to the LCD.
//    lcd.print(" % "); // Print the unit of the centigrade temperature to the LCD.
    
    lcd.setCursor(0, 0); // set the cursor to column 0, line 1
    lcd.print("Temp:    ");// Print a message of "Temp: "to the LCD.
    lcd.print((float)DHT11.temperature, 2);// Print a centigrade temperature to the LCD. 
    lcd.print(" C "); // Print the unit of the centigrade temperature to the LCD.
}


