#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,20,4);
char Data[20]; // 20 is the number of chars it can hold
const byte Usercount = 3;

char Master[2][7] = {
  "123456",
  "000123"
};

char * User[Usercount] = {
  "Master","Andrew","INVALID"};

byte currentCommand = 0;
byte MasterCount = 6;
byte good_Count = 0;

int user = 0;
bool Password_is_good, User_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {2,3,4,5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10,9,8}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
}

void loop()
{
  lcd.setCursor(0,0);
  lcd.print("Enter Password");
  customKey = customKeypad.getKey();
  if (customKey)
  {
    Data[currentCommand] = customKey;
    lcd.setCursor(currentCommand,1);
    lcd.print(Data[currentCommand]);
    currentCommand++;
  }

  if(currentCommand == MasterCount)
  {
    delay(1000);
    while(user != 2)
    {
      for(int count = 0; count < MasterCount; count++)
      {
        if(Data[count] == Master[user][count])
        {
          good_Count++;
        }  
      }
      if(good_Count == MasterCount)
      {
        good_Count = 0;
        Password_is_good = true; 
        break;
      }
      else
      { 
        good_Count = 0; 
        user++; 
        Password_is_good = false;
      }
    }

    lcd.setCursor(0,0);
    if(Password_is_good)
    {
      lcd.clear();
      Serial.print("Password is good");
      lcd.setCursor(0,1);
      Serial.print(User[user]);
      delay(2000);
      user = 0;
      lcd.clear();
      clearData(); 
    }
    else 
    {
      lcd.clear();
      lcd.print("Password is bad");
      lcd.setCursor(0,1);
      lcd.print(User[user]);
      delay(2000);
      user = 0;
      lcd.clear();
      clearData();
    }
  }
  if(customKey == '*')
  {
    lcd.clear();
    clearData();
    while(User_is_good != true)
    {
      lcd.setCursor(0,0);
      lcd.print("Change Password");
      lcd.setCursor(0,1);
      lcd.print("Enter User: ");
      user = customKeypad.getKey();
      if(user)
      {
        user -= '0'; // convert from char to int
        lcd.print(user);
        if(user < Usercount) 
        { // if within number of users
          delay(1000);
          User_is_good = true; //gets out of while loop
          user -= 1; // ENTER: 1 = master, 2 = user, but the array is 0 for master and 1 for user. This makes it so.
          lcd.clear();
        }
        else 
        {
          delay(1000);
          lcd.clear();
          lcd.print("Invalid User");
          delay(2000);
          lcd.clear(); 
        }
      } 
    }
    User_is_good = false;// stays in while loop
    while(customKey != '#')
    {
      lcd.setCursor(0,0);
      lcd.print(User[user]); // prints out the user
      lcd.setCursor(0,1);
      lcd.print("Press # to confirm");
      customKey = customKeypad.getKey();
      if (customKey)
      {
        Master[user][currentCommand] = customKey;
        lcd.setCursor(currentCommand,2);
        lcd.print(Master[user][currentCommand]);
        MasterCount = currentCommand;
        currentCommand++;
      }
    }
    if(customKey == '#') // press # to store new passwrd for user
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(User[user]);
      lcd.print(" is reset");
      delay(1000);
      clearData();
      lcd.clear();
    }
  }
}

void clearData() 
{
  user=0;
  while(currentCommand != 0)
  {   // This can be used for any array size, 
    Data[currentCommand--] = 0; //clear for new data
  }
  return;
}

