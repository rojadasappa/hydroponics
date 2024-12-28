#include <SPI.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
//Constants
int TURB= A0;              //Analog channel A0 as used to measure battery voltage
int VALUE = 0;
// Replace with your network credentials
const char* ssid     = "ESP32";
const char* password = "12345678";
#include <Servo.h>
Servo servo;
#include <DHT.h>
DHT dht2 (14,DHT11); 
// Set web server port number to 80
WiFiServer server(80);
String output10State = "off";
String output9State = "off";
String output8State = "off";
const int output10 = 5;      // D1
const int output9 = 12;       // D2
const int output8 = 16;       // D3

// Variable to store the HTTP request
String header;

void setup() 
{
  Serial.begin(115200);     // Initialize the output variables as outputs
  pinMode(output10, OUTPUT);
  pinMode(output9, OUTPUT);
  pinMode(output8, OUTPUT);
  digitalWrite(output10, LOW);
  digitalWrite(output9, LOW);
  digitalWrite(output8, LOW);
  servo.attach(2); //D4
  servo.write(0);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  

   VALUE=analogRead(TURB); 
   Serial.println(VALUE);
   delay(200);
   Serial.println("Temperature in C:");  
   Serial.println((dht2.readTemperature( ))); 
   delay(200); 
   Serial.println("Humidity in C:");  
   Serial.println((dht2.readHumidity()));    

   WiFiClient client = server.available();   // Listen for incoming clients
   if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:

    
    
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            
  
            // turns the GPIOs on and off
            if (header.indexOf("GET /10/on") >= 0) 
            {
              Serial.println("GPIO 10 on");
              output10State = "on";
              servo.write(180);
              delay(1000);
              servo.write(0);
              delay(1000);
             } 
            else if (header.indexOf("GET /10/off") >= 0) 
            {
              Serial.println("GPIO 10 off");
              output10State = "off";
              servo.write(0);
                         
            }
            if (header.indexOf("GET /9/on") >= 0) 
            {
              Serial.println("GPIO 9 on");
              output9State = "on";
              digitalWrite(output9,HIGH);
             } 
            else if (header.indexOf("GET /9/off") >= 0) 
            {
              Serial.println("GPIO 9 off");
              output9State = "off";
              digitalWrite(output9,LOW);
                         
            }
            if (header.indexOf("GET /8/on") >= 0) 
            {
              Serial.println("GPIO 8 on");
              output8State = "on";
              digitalWrite(output8,HIGH);
             } 
            else if (header.indexOf("GET /8/off") >= 0) 
            {
              Serial.println("GPIO 8 off");
              output8State = "off";
              digitalWrite(output8,LOW);
                         
            }
                       
            
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

                    
            
            
            // Web Page Heading
            client.println("<body><h1>HYDROPHONIC PLANTATION MONITORING USING ESP8266 WEBSERVER</h1>");
            client.println("<br>");

            
            client.println("TURBIDITY SENSOR =");
            client.println("<br>");
            client.println("<br>");
            client.print(VALUE);
            client.println("<br>");
            client.println("<br>");

           
  
            if(VALUE<900)
            {
            client.println("T-SENSOR value is Low,WATER IS POLLUTED ");
            }
            else
            {
            client.println("T-Sensor value is Normal");
            }
            client.println("<br>");
            client.println("<br>");
            client.println("TEMP =");
            client.println("<br>");
            client.println("<br>");
            client.print((dht2.readTemperature( )));
            client.println("<br>");
            client.println("<br>");

            client.println("HUMIDITY =");
            client.println("<br>");
            client.println("<br>");
            client.print((dht2.readHumidity()));
            client.println("<br>");
            client.println("<br>");
            
            client.println("<br>");
            client.println("<br>");
            client.println("TEMP =");
            client.println("<br>");
            client.println("<br>");
            client.print((dht2.readTemperature( )));
            client.println("<br>");
            client.println("<br>"); 
            
            // If the output5State is off, it displays the ON button       
            if (output10State=="off") 
            {
              client.println("<p><a href=\"/10/on\"><button class=\"button\">SERVO</button></a></p>");
              client.println("<br>");
            } 
            else 
            {
              client.println("<p><a href=\"/10/off\"><button class=\"button button2\">S-OFF</button></a></p>");
            } 
            if (output9State=="off") 
            {
              client.println("<p><a href=\"/9/on\"><button class=\"button\">FAN</button></a></p>");
              client.println("<br>");
            } 
            else 
            {
              client.println("<p><a href=\"/9/off\"><button class=\"button button2\">F-OFF</button></a></p>");
              client.println("<br>");
            } 
            if (output8State=="off") 
            {
             
              client.println("<p><a href=\"/8/on\"><button class=\"button\">LIGHT</button></a></p>");
              client.println("<br>");
            } 
            else 
            {
              client.println("<p><a href=\"/8/off\"><button class=\"button button2\">L-OFF</button></a></p>");
              client.println("<br>");
            } 
   
            
  
                       
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  }
