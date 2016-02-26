#include <Pushetta.h>
#include <SPI.h>
#include <Ethernet.h>

#define RELAY_ON 0
#define RELAY_OFF 1
#define RELAY_1  2


// Be sure this address is unique in your network
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x19 };        
char APIKEY[] = "21a6e54e956aa7526f638e6d0274c09307326434"; // Put here your API key
char CHANNEL[] = "Arduino_safety";// and here your channel name
 
char serverName[] = "api.pushetta.com";
boolean lastConnected = false; 
EthernetClient client;


void setup() {
  // initialize digital pin 13 as an output.
  pinMode(RELAY_1, OUTPUT);
  digitalWrite(RELAY_1, RELAY_OFF);
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) 
  {
  while(true);                          // no point in carrying on, so do nothing forever
  }
  delay(1000);            
}

// the loop function runs over and over again forever
void loop() {
  int sensorValue1 = analogRead(A0);
  float voltage1 = sensorValue1 * (5.0 / 1023.0);
  int sensorValue2 = analogRead(A1);
  float voltage2 = sensorValue2 * (5.0 / 1023.0);
  float diff = abs(voltage2 - voltage1);
  if (diff > 1.5) {
    digitalWrite(RELAY_1, RELAY_ON);
    delay(100);
    digitalWrite(RELAY_1, RELAY_OFF);
    sendToPushetta(CHANNEL, "Lichtschranke wurde unterbrochen!!");
    delay(2000);
  }
  Serial.println(diff);
  //delay(1000);
}

//Function for sending the request to Pushetta
void sendToPushetta(char channel[], String text){
  client.stop();              
  if (client.connect(serverName, 80)) 
  {                   
  client.print("POST /api/pushes/");
  client.print(channel);
  client.println("/ HTTP/1.1");
  client.print("Host: ");
  client.println(serverName);
  client.print("Authorization: Token ");
  client.println(APIKEY);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(text.length()+46);
  client.println();
  client.print("{ \"body\" : \"");
  client.print(text);
  client.println("\", \"message_type\" : \"text/plain\" }");
  client.println(); 
  } 
}
