#include "Keypad.h"

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] =
 {{'5','4','6'},
{'2','1','3'},
{'8','7','9'},
{'#','0','*'}};
// set display to four rows
// set display to three columns
   byte rowPins[ROWS] = {8, 7, 6, 5}; 
   byte colPins[COLS] = {4, 3, 2};  
   Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
   // End of necessary code
char PIN[6]={'1','2','3','4','5','6'}; // our secret number char attempt[6]={0,0,0,0,0,0};
char attempt[6]={0,0,0,0,0,0};
int z=0;
   void setup()
   {
     Serial.begin(9600);
   }
   void correctPIN()  // do this if the correct PIN is entered
   {
     Serial.println("Korrekte PIN!");
   }
   void incorrectPIN() // do this if an incorrect PIN is entered
   {
     Serial.println("Inkorrekte PIN!");
   }
void checkPIN()
   {
    int correct=0;
    int i;
    for ( i = 0; i < 6 ; i++ )
     {
       if (attempt[i]==PIN[i])
       {
        correct++; }
        }
        if (correct==6)
        {
        correctPIN(); 
        } 
        else
        {
        incorrectPIN();
        }
     for (int zz=0; zz<6; zz++) // remove previously entered code attempt from
     {
       attempt[zz]=0;
     }
}

void readKeypad()
   {
     char key = keypad.getKey();
     if (key != NO_KEY)
     {
    switch(key) 
    {
       case '*':
         z=0;
         break;
       case '#':
         delay(100);  // removes the possibility of switch bounce
         checkPIN();
         break;

         default:
          attempt[z]=key;
          z++;
        }
  }
}
void loop() {
  readKeypad();
}
