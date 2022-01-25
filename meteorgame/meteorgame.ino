#include <LiquidCrystal.h>
#include <ezButton.h>

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
  B10001,
  B01010,
  B00100,
  B00000,
};

int cursorLocY = 0,
    cursorLocX = 2,
    meteorTimer = 0,
    meteorLoc = 15,
    row = 0,
    meteorOnField = 0,
    randNumber = 0,
    gameOver = 0,
    result = 0,
    gameSpeed = 100,
    gameAcc = 5;

void setup() {
  lcd.createChar(0, meteor);
  lcd.begin(16, 2);
  lcd.setCursor(cursorLocX, cursorLocY);
  lcd.print(">");
}

void loop() {
  button.loop();
  // start new game
  if (button.isPressed() & gameOver == 1) {
    gameOver = 0;
    lcd.clear();
  }
  if (gameOver == 0) {
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
    if (meteorLoc <= 0) {
      meteorLoc = 15;
      meteorOnField = 0;
      lcd.setCursor(1,0);
      lcd.print(" ");
      lcd.setCursor(1,1);
      lcd.print(" ");
      result++;
      if (gameSpeed >= 30){
        if (gameSpeed < 70) {
          gameAcc = 4;
        }
        if (gameSpeed < 50) {
          gameAcc = 3;
        }
        if (gameSpeed < 40) {
          gameAcc = 2;
        }
        gameSpeed = gameSpeed - gameAcc;         
      }
    }

    if (meteorLoc == cursorLocX & row == cursorLocY) {
      int printStartLoc = 15;
      for (int i = 0; i < 12; i++) {
        lcd.clear();
        lcd.setCursor(printStartLoc, 0);
        lcd.print("GAME OVER");
        lcd.setCursor(printStartLoc, 1);
        lcd.print("SCORE: ");
        lcd.print(result);
        delay(100);
        printStartLoc--;
      }
      meteorLoc = 15;
      gameOver = 1;
      result = 0;
      gameSpeed = 100;
    }
  }
}
