#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  // put your setup code here, to run once:
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("Please select a bay:");

  lcd.setCursor(0,1);
  lcd.print("1");
  lcd.setCursor(4,1);
  lcd.print("2");
  lcd.setCursor(8,1);
  lcd.print("3");
  lcd.setCursor(12,1);
  lcd.print("4");
  lcd.setCursor(16,1);
  lcd.print("5");

  lcd.setCursor(0,2)
  lcd.print()
}

void loop() {
  // put your main code here, to run repeatedly:

  // interrupt pins 2,3,18,19,20,21
}
