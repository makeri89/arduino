#include <LiquidCrystal.h>
#include <ezButton.h>
#include <EEPROM.h>

#define rs 4
#define en 6
#define d4 10
#define d5 11
#define d6 12
#define d7 13

#define DOWN 800
#define UP 400
#define LEFT 400
#define RIGHT 800

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
ezButton button(2);

byte meteor[8] = {
  B00000,
  B00100,
  B01010,
  B10101,
  B01010,
  B00100,
  B00000,
};

int cursorLocY = 0,
    cursorLocX = 3,
    meteorTimer = 0,
    meteorLoc = 15,
    row = 0,
    meteorOnField = 0,
    randNumber = 0,
    gameOver = 0,
    result = 0,
    gameSpeed = 100,
    gameAcc = 5,
    memoryAddr = 0,
    nameAddr = 2,
    nameField = 5,
    letterLocX = 0,
    nameGivenThisRound = 0; 

byte memoryValue;
String playerName = "";
int xValue2 = analogRead(A0);
char letters[27] = {"ABCDEFGHIJKLMNOPQRSTUWVXYZ >"};

void setup() {
  lcd.createChar(0, meteor);
  lcd.begin(16, 2);
  lcd.setCursor(cursorLocX, cursorLocY);
  lcd.print(">");
  memoryValue = EEPROM.read(memoryAddr);
  }

void loop() {
  // start new game
  if (gameOver == 0) {
    nameField = 5;
    int xValue = analogRead(A0);
    int yValue = analogRead(A1);
    lcd.setCursor(cursorLocX, cursorLocY);
    if (yValue > DOWN & cursorLocY != 1) {
      lcd.clear();
      cursorLocY = 1;
      lcd.setCursor(cursorLocX, cursorLocY);
    }
    if (yValue < UP & cursorLocY != 0) {
      lcd.clear();
      cursorLocY = 0;
      lcd.setCursor(cursorLocX, cursorLocY);
    }
    lcd.print(">");
    if (meteorOnField == 1) {
      lcd.setCursor(meteorLoc, row);
      lcd.write(byte(0));
    }
    randNumber = random(1000);
    if (randNumber > 500 & meteorOnField == 0) {
      row = random(2);
      lcd.setCursor(meteorLoc, row);
      lcd.write(byte(0));
      meteorOnField = 1;
    }
    meteorTimer++;
    if (meteorTimer >= gameSpeed) {
      lcd.print(" ");
      meteorLoc--;
      meteorTimer = 0;
    }
    // meteor goes through the field
    if (meteorLoc < 0) {
      meteorLoc = 15;
      meteorOnField = 0;
      lcd.setCursor(0,0);
      lcd.print(" ");
      lcd.setCursor(0,1);
      lcd.print(" ");
      result++;
      // game speed acceleration slows down towards the end
      if (gameSpeed >= 25){
        if (gameSpeed < 70) {
          gameAcc = 10;
        }
        if (gameSpeed < 55) {
          gameAcc = 4;
        }
        if (gameSpeed < 45) {
          gameAcc = 1;
        }
        if (gameSpeed < 40 & result % 3 != 0) {
          gameAcc = 0;
        }
        gameSpeed = gameSpeed - gameAcc;
      }
    }

    if (meteorLoc == cursorLocX & row == cursorLocY) {
      gameOver = 1;
      if (result > memoryValue) {
        EEPROM.write(memoryAddr, result);
      }
    }
  } else {
    // scores are printed if a game is not going

    int printStartLoc = 15;

    printer(printStartLoc, "GAME OVER", "SCORE: ", -1, result);
    if (memoryValue < result & nameGivenThisRound == 0) {
      enterName();
      memoryValue = EEPROM.read(memoryAddr);
    }
    String hiSc = readStringFromEEPROM(2);
    hiSc.concat(": ");
    hiSc.concat(String(memoryValue));
    printer(printStartLoc, "HIGHSCORE", hiSc, -1, -1);
    printer(printStartLoc, "HOLD TO", "RESTART", -1, -1);
  }
}

void printer(int printStartLoc, String firstRow, String secondRowStr, int secondRowInt, int score) {
  if (gameOver == 1) {
    for (int i = 0; i < 12; i++) {
      pressed();
      lcd.clear();
      lcd.setCursor(printStartLoc, 0);
      lcd.print(firstRow);
      lcd.setCursor(printStartLoc, 1);
      if (secondRowInt == -1) {
        lcd.print(secondRowStr);
      } else {
        lcd.setCursor(printStartLoc + 3, 1);
        lcd.print(secondRowInt);
      }
      if (score != -1) {
        lcd.print(score);
      }
      delay(50);
      printStartLoc--;
    }
    delay(2000);
    pressed();
  } else {
    lcd.clear();
  }
}

bool pressed() {
  button.loop();
  if (button.isPressed() & gameOver == 1) {
    result = 0;
    gameSpeed = 100;    
    meteorLoc = 15;
    gameOver = 0;
    nameGivenThisRound = 0;
    cursorLocY = 0;
    cursorLocX = 3;
    lcd.clear();
    return true;
  } else {
    return false;
  }
}

void enterName() {
    lcd.clear();
    listLetters(0, 17);
    lcd.setCursor(0, 1);
    lcd.print("NAME: ");
    lcd.setCursor(0, 0);
    cursorLocX = 0;
    lcd.cursor();
    while (true) {
      button.loop();
      if (button.isPressed() & nameField < 16) {
        if (String(letters[letterLocX]).equals(">")) {
          writeStringToEEPROM(2, playerName);
          nameGivenThisRound = 1;
          lcd.noCursor();
          break;
        } else {
          lcd.setCursor(nameField, 1);
          lcd.print(letters[letterLocX]);
          lcd.setCursor(cursorLocX, 0);
          nameField++;
          playerName.concat(String(letters[letterLocX]));
          }
        }
        cursorMove();
      }
}

void cursorMove() {
  xValue2 = analogRead(A0);
  
    if (xValue2 < LEFT & letterLocX - 1 >= 0) {
        cursorLocX--;
        if (letterLocX == 16) {
          listLetters(0, 17);
          cursorLocX = 15;        
        }
      letterLocX--;
      lcd.setCursor(cursorLocX, 0);
      }
    
    if (xValue2 > RIGHT & letterLocX + 1 <= 27) {
        cursorLocX++;
        if (letterLocX == 15) {
          listLetters(16, 12);
          cursorLocX = 0;
        }
        letterLocX++;
        lcd.setCursor(cursorLocX, 0);        
    }
    delay(150);
}

void listLetters(int val, int length) {
  for (int a = 0; a < 17; a++) {
    lcd.setCursor(a, 0);
    lcd.print(" ");
  }
  int adder = 0;
  for (int i = 0; i < length; i++) {
    lcd.setCursor(i, 0);
    lcd.print(letters[val + adder]);
    adder++;
  }
}

void writeStringToEEPROM(int addrOffset, const String &playerName)
{
  byte len = playerName.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, playerName[i]);
  }
}

String readStringFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';
  return String(data);
}
