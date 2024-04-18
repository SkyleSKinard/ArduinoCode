#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define leftButtonPin 10 // move left button connected to pin 2
#define selectButtonPin 3 // select button connected to pin 3
#define rightButtonPin 4 // move right button connected to pin 4

int leftButtonState = HIGH; // initialize left button state to HIGH (not pressed)
int leftButtonPrevState = HIGH; // initialize left button previous state to HIGH (not pressed)
int selectButtonState = HIGH; // initialize select button state to HIGH (not pressed)
int selectButtonPrevState = HIGH; // initialize select button previous state to HIGH (not pressed)
int rightButtonState = HIGH; // initialize right button state to HIGH (not pressed)
int rightButtonPrevState = HIGH; // initialize right button previous state to HIGH (not pressed)

int i = 0; // index variable for selection

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

// define arrow array to create an up arrow
byte arrow[] = {
    B00100,
    B01110,
    B10101,
    B00100,
    B00100,
    B00100,
    B00100,
    B00000,
  };

void setup() {
  // put your setup code here, to run once:
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(selectButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
  // Line 1
  lcd.setCursor(0,0);
  lcd.print("Please select a bay:");

  // Line 2
  lcd.setCursor(0,1);
  lcd.print("1"); // bay 1
  lcd.setCursor(4,1);
  lcd.print("2"); // bay 2
  lcd.setCursor(8,1);
  lcd.print("3"); // bay 3
  lcd.setCursor(12,1);
  lcd.print("4"); // bay 4
  lcd.setCursor(16,1);
  lcd.print("5"); // bay 5

  // Line 3
  lcd.createChar(0, arrow); // create the custom arrow character called from global scope
  lcd.setCursor(0,2);
  lcd.write(0); // writes byte[0] (arrow) to the screen

  // Line 4
  lcd.setCursor(0,3);
  lcd.print("Humid:15%"); // place holder for humidity sensor
  lcd.setCursor(12,3);
  lcd.print("Temp:33F"); // place holder for temperature sensor
}

void loop() {
  // put your main code here, to run repeatedly:
  leftButtonState = digitalRead(leftButtonPin); // get value of button state, HIGH = not pressed, LOW = pressed
  selectButtonState = digitalRead(selectButtonPin); // get value of button state, HIGH = not pressed, LOW = pressed
  rightButtonState = digitalRead(rightButtonPin); // get value of button state, HIGH = not pressed, LOW = pressed

  if (leftButtonPrevState == LOW && leftButtonState == HIGH)
  {
    i = i - 4; // move left 4 indexes to move to each bay

    if (i < 0) // if arrow at bay 1 on screen -> button pressed -> move to bay 5
    {
       i = 16;
    }

    lcd.setCursor(0,2); // set cursor to 0th index of third line
    lcd.print("                   "); // clear 3rd line
    lcd.setCursor(i,2); // set cursor to ith index of third line
    lcd.write(0); // print arrow character
    delay(200); // delay button presses 200 ticks
  }

  leftButtonPrevState = leftButtonState;
  //selectButtonPrevState = selectButtonPrevState;
  //rightButtonPrevState = rightButtonState;
  // interrupt pins 2,3,18,19,20,21

}
