#include "Keypad.h"
 
const byte ROWS = 4; // number of rows
const byte COLS = 3; // number of columns
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'#','0','*'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; // row pinouts of the keypad R1 = D8, R2 = D7, R3 = D6, R4 = D5
byte colPins[COLS] = {4, 3, 2};    // column pinouts of the keypad C1 = D4, C2 = D3, C3 = D2
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
 
void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
  char first = keypad.getKey();
  char second = keypad.getKey(); 
  char third = keypad.getKey(); 
  char fourth = keypad.getKey(); 
  if (first != NO_KEY)
    Serial.println(first);
}
