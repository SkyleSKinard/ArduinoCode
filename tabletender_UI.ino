#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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
  };


void setup() {
  // put your setup code here, to run once:
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

  // interrupt pins 2,3,18,19,20,21
}
