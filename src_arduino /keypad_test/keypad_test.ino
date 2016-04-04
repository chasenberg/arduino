#include "Keypad.h"
#include <Password.h>

const byte ROWS = 4; // number of rows
const byte COLS = 3; // number of columns
char keys[ROWS][COLS] = {
{'5','4','6'},
{'2','1','3'},
{'8','7','9'},
{'0','*','#'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; // row pinouts of the keypad R1 = D8, R2 = D7, R3 = D6, R4 = D5
byte colPins[COLS] = {4, 3, 2};    // column pinouts of the keypad C1 = D4, C2 = D3, C3 = D2
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void setup()
{
  Serial.begin(9600);
}

String newPasswordString; //hold the new password
char newPassword[6]; //charater string of newPasswordString
 
//initialize password to 1234
//you can use password.set(newPassword) to overwrite it
Password password = Password( "1234" );
 
byte maxPasswordLength = 6; 
byte currentPasswordLength = 0;

void loop(){
   char key = keypad.getKey();
   if (key != NO_KEY){
      delay(60); 
      switch (key){
      case '#': checkPassword(); break;
      case '*': resetPassword(); break;
      default: processNumberKey(key);
      }
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

void checkPassword() {
   if (password.evaluate()){
      Serial.println(" OK.");
   } else {
      Serial.println("Falsches Passwort! Alarm!");
   } 
   resetPassword();
}

void resetPassword() {
   password.reset(); 
   currentPasswordLength = 0; 
}
