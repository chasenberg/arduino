//#include <Adafruit_SleepyDog.h>
#include <avr/sleep.h>
#include "Keypad.h"
#include <Password.h>
#include <Pushetta.h>
#include <SPI.h>
#include <Ethernet.h>

//#define RELAY_ON 0
//#define RELAY_OFF 1
//#define RELAY_1  2

int laser = 9;

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x19 };
char APIKEY[] = "21a6e54e956aa7526f638e6d0274c09307326434"; 
char CHANNEL[] = "Arduino_safety";

char serverName[] = "api.pushetta.com";
boolean lastConnected = false;
EthernetClient client;


const byte ROWS = 4; 
const byte COLS = 3; 
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; // row pinouts of the keypad R1 = D8, R2 = D7, R3 = D6, R4 = D5
byte colPins[COLS] = {4, 3, 2};    // column pinouts of the keypad C1 = D4, C2 = D3, C3 = D2
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String newPasswordString; //hold the new password
char newPassword[6]; //charater string of newPasswordString

Password password = Password("1234");
 
byte maxPasswordLength = 6; 
byte currentPasswordLength = 0;

void setup()
{
  //pinMode(RELAY_1, OUTPUT);
  //digitalWrite(RELAY_1, RELAY_OFF);
  pinMode(laser,OUTPUT);
  digitalWrite(laser,HIGH);
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0)
  {
    while (true);                         
  }
  delay(1000);
}

void loop(){
   char key = keypad.getKey();
   if (key != NO_KEY){
      delay(100); 
      switch (key){
      case '#': checkPassword(); break;
      case '*': resetPassword(); break;
      default: processNumberKey(key);
      }
   }
  int sensorValue1 = analogRead(A0);
  float voltage1 = sensorValue1 * (5.0 / 1023.0);
  int sensorValue2 = analogRead(A1);
  float voltage2 = sensorValue2 * (5.0 / 1023.0);
  float diff = abs(voltage2 - voltage1);
  if (diff > 1.5) {
    //digitalWrite(RELAY_1, RELAY_ON);
    delay(100);
    //digitalWrite(RELAY_1, RELAY_OFF);
    sendToPushetta(CHANNEL, "Lichtschranke wurde unterbrochen!!");
    delay(2000);
  }
}

void checkPassword() {
   if (password.evaluate()){
      Serial.println(" Korrektes Passwort, System wird abgeschaltet.");
      sendToPushetta(CHANNEL, "Korrektes Passwort, System wird abgeschaltet.");
      digitalWrite(laser,LOW);
      set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
      sleep_enable();   
      sleep_mode(); 
      
   } else {
      Serial.println("Falsches Passwort! Alarm!");
      sendToPushetta(CHANNEL, "Falsches Passwort! Alarm!");
   } 
}
void processNumberKey(char key) {
   Serial.print(key);
   currentPasswordLength++;
   password.append(key);
   if (currentPasswordLength == maxPasswordLength) {
      checkPassword();
   } 
}

void resetPassword() {
   password.reset(); 
   currentPasswordLength = 0; 
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
    client.println(text.length() + 46);
    client.println();
    client.print("{ \"body\" : \"");
    client.print(text);
    client.println("\", \"message_type\" : \"text/plain\" }");
    client.println();
  }
}


