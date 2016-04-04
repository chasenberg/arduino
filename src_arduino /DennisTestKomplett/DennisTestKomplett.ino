
#include "DHT.h"
#define DHTPIN 2 
#define DHTTYPE DHT22 
#include <SPI.h>
#include <Ethernet.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
  
int LDR = 0;     //analog pin to which LDR is connected, here we set it to 0 so it means A0
int LDRValue = 0;      //that’s a variable to store LDR values
int light_sensitivity = 500;    //This is the approx value of light surrounding your LDR
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 178, 180);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  dht.begin();
  lcd.begin();
  lcd.backlight();
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.print(LDRValue);
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("F.:");
  lcd.print(h);
  lcd.print(" %");
  lcd.setCursor(0,1);
  lcd.print("T.:");
  lcd.print(t);
  lcd.print(" C");
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          client.print("Feuchtigkeit: ");
          client.print(h);
          client.print(" %\t");
          client.print("Temperatur: ");
          client.print(t);
          client.print(" *C ");
          client.print(f);
          client.print(" *F\t");
          client.print("Hitzeindex: ");
          client.print(hic);
          client.print(" *C ");
          client.print(hif);
          client.println(" *F");
          Serial.print("Feuchtigkeit: ");
          Serial.print(h);
          Serial.print(" %\t");
          Serial.print("Temperatur: ");
          Serial.print(t);
          Serial.print(" *C ");
          Serial.print(f);
          Serial.print(" *F\t");
          Serial.print("Hitzeindex: ");
          Serial.print(hic);
          Serial.print(" *C ");
          Serial.print(hif);
          Serial.println(" *F"); 
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(20);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    Ethernet.maintain();
    LDRValue = analogRead(LDR);
    Serial.println(light_sensitivity);
  if(LDRValue < light_sensitivity)
    {
      lcd.backlight();
    }
  else
    {
      lcd.noBacklight();
    }

  }
  delay(2000);
}

