#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define dhtSensorPin 13 // temperature/humidity sensor connected to pin 2
#define dhtType DHT22 // define type of sensor
#define leftButtonPin 10 // move left button connected to pin 10
#define selectButtonPin 11 // select button connected to pin 11
#define rightButtonPin 12 // move right button connected to pin 12

#define dirPin1 52 /* Z-Axis*/
#define stepPin1 53 /* Z-Axis*/
#define dirPin2 44 /* Z-Axis*/
#define stepPin2 45 /* Z-Axis*/
#define stepsPerRevolution 1600

#define dirPin3 50 /* X-Axis*/
#define stepPin3 51 /* X-Axis*/
#define dirPin4 46 /* X-Axis*/
#define stepPin4 47 /* X-Axis*/

#define dirPin5 48 /* Y-Axis*/
#define stepPin5 49 /* Y-Axis*/

// interrupt pins 2,3,18,19,20,21

DHT dht(dhtSensorPin, dhtType);

int leftButtonState = HIGH; // initialize left button state to HIGH (not pressed)
int leftButtonPrevState = HIGH; // initialize left button previous state to HIGH (not pressed)
int selectButtonState = HIGH; // initialize select button state to HIGH (not pressed)
int selectButtonPrevState = HIGH; // initialize select button previous state to HIGH (not pressed)
int rightButtonState = HIGH; // initialize right button state to HIGH (not pressed)
int rightButtonPrevState = HIGH; // initialize right button previous state to HIGH (not pressed)

int i = 0; // index variable for selection
int count = 0; // loop control variable for select button

const int BUTTON_PIN3 = 3; /* Z-Axis*/
const int BUTTON_PIN2 = 2; /* Z-Axis*/
int lastState3 = HIGH; /* Z-Axis*/
int lastState2 = HIGH; /* Z-Axis*/
int currentState3; /* Z-Axis*/
int currentState2; /* Z-Axis*/

const int BUTTON_PIN5 = 5; /* X-Axis*/
const int BUTTON_PIN4 = 4; /* X-Axis*/
int lastState5 = HIGH; /* X-Axis*/
int lastState4 = HIGH; /* X-Axis*/
int currentState5; /* X-Axis*/
int currentState4; /* X-Axis*/

const int BUTTON_PIN7 = 7; /* Y-Axis*/
const int BUTTON_PIN6 = 6; /* Y-Axis*/
int lastState7 = HIGH; /* Y-Axis*/
int lastState6 = HIGH; /* Y-Axis*/
int currentState7; /* Y-Axis*/
int currentState6; /* Y-Axis*/

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

  dht.begin(); // initialize the temp/humidity sensor

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
  lcd.print("RH: "); // place holder for humidity sensor
  lcd.setCursor(9,3);
  lcd.print("Temp: "); // place holder for temperature sensor

  Serial.begin(9600);
  pinMode(BUTTON_PIN3, INPUT_PULLUP); /* Z-Axis*/
  pinMode(BUTTON_PIN2, INPUT_PULLUP); /* Z-Axis*/
  pinMode(stepPin1, OUTPUT); /* Z-Axis*/
  pinMode(dirPin1, OUTPUT); /* Z-Axis*/
  pinMode(stepPin2, OUTPUT); /* Z-Axis*/
  pinMode(dirPin2, OUTPUT); /* Z-Axis*/

  pinMode(BUTTON_PIN5, INPUT_PULLUP); /* X-Axis*/
  pinMode(BUTTON_PIN4, INPUT_PULLUP); /* X-Axis*/
  pinMode(stepPin3, OUTPUT); /* X-Axis*/
  pinMode(dirPin3, OUTPUT); /* X-Axis*/
  pinMode(stepPin4, OUTPUT); /* X-Axis*/
  pinMode(dirPin4, OUTPUT); /* X-Axis*/

  pinMode(BUTTON_PIN7, INPUT_PULLUP); /* X-Axis*/
  pinMode(BUTTON_PIN6, INPUT_PULLUP); /* X-Axis*/
  pinMode(stepPin5, OUTPUT); /* X-Axis*/
  pinMode(dirPin5, OUTPUT); /* X-Axis*/
}

void loop() {
  lcd.setCursor(3,3);
  lcd.print((int)dht.readHumidity()); // display humidity to screen
  lcd.print("%");
  lcd.setCursor(14,3);
  lcd.print((int)dht.readTemperature() * (9/5) + 32); // display temperature to screen
  lcd.print(" F");

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

  if (rightButtonPrevState == LOW && rightButtonState == HIGH)
  {
    i = i + 4; // move right 4 indexes to move to each bay

    if (i > 16) // if arrow at bay 1 on screen -> button pressed -> move to bay 5
    {
       i = 0;
    }

    lcd.setCursor(0,2); // set cursor to 0th index of third line
    lcd.print("                   "); // clear 3rd line
    lcd.setCursor(i,2); // set cursor to ith index of third line
    lcd.write(0); // print arrow character
    delay(200); // delay button presses 200 ticks
  }

  if (selectButtonPrevState == LOW && selectButtonState == HIGH)
  {
    while(count < 5) // loop 15 times to imitate blinks
    {
        lcd.setCursor(0, 2);
        lcd.print("                  "); // clear 3rd line
        delay(500);
        lcd.setCursor(i, 2);
        lcd.write(0);
        delay(500);
        count = count + 1; // increment count
    }
    count = 0; // reset count for loop
    i = 0; // reset i back to 0
    lcd.setCursor(0, 2); 
    lcd.print("                  "); // clear 3rd line
    lcd.setCursor(i, 2); // reset index back to 0
    lcd.write(0); // place arrow back under bay 1
  }

  leftButtonPrevState = leftButtonState;
  selectButtonPrevState = selectButtonState;
  rightButtonPrevState = rightButtonState;

  currentState3 = digitalRead(BUTTON_PIN3); /* Z-Axis*/
  currentState2 = digitalRead(BUTTON_PIN2); /* Z-Axis*/
  
  currentState5 = digitalRead(BUTTON_PIN5); /* X-Axis*/
  currentState4 = digitalRead(BUTTON_PIN4); /* X-Axis*/

  currentState7 = digitalRead(BUTTON_PIN7); /* Y-Axis*/
  currentState6 = digitalRead(BUTTON_PIN6); /* Y-Axis*/

  if(lastState2 == HIGH && currentState2 == LOW && currentState3 == HIGH){
    digitalWrite(dirPin1, HIGH);
    digitalWrite(dirPin2, HIGH);
     Serial.println("UP");
      for (int i=0; i < 0.1 * stepsPerRevolution; i++) {
    // These four lines result in 1 step:
        digitalWrite(stepPin1, HIGH);
        digitalWrite(stepPin2, HIGH);
        
        delayMicroseconds(125);
        digitalWrite(stepPin1, LOW);
        digitalWrite(stepPin2, LOW);
      
        delayMicroseconds(125);
    } 

  }

   if(lastState3 == HIGH && currentState3 == LOW && currentState2 == HIGH){
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, LOW);
     Serial.println("DOWN");
      for (int i=0; i < 0.1 * stepsPerRevolution; i++) {
    // These four lines result in 1 step:
        digitalWrite(stepPin1, HIGH);
        digitalWrite(stepPin2, HIGH);
        
        delayMicroseconds(125);
        digitalWrite(stepPin1, LOW);
        digitalWrite(stepPin2, LOW);
        delayMicroseconds(125);
    }

  }

  if(lastState4 == HIGH && currentState4 == LOW && currentState5 == HIGH){
    digitalWrite(dirPin3, LOW);
    digitalWrite(dirPin4, HIGH);
     Serial.println("FORWARD");
      for (int i=0; i < 0.1 * stepsPerRevolution; i++) {
    // These four lines result in 1 step:
        digitalWrite(stepPin3, HIGH);
        digitalWrite(stepPin4, HIGH);
        
        delayMicroseconds(125);
        digitalWrite(stepPin3, LOW);
        digitalWrite(stepPin4, LOW);
        delayMicroseconds(125);
    }

  }
    
   if(lastState5 == HIGH && currentState5 == LOW && currentState4 == HIGH){
    digitalWrite(dirPin3, HIGH);
    digitalWrite(dirPin4, LOW);
     Serial.println("BACKWARD");
      for (int i=0; i < 0.1 * stepsPerRevolution; i++) {
    // These four lines result in 1 step:
        digitalWrite(stepPin3, HIGH);
        digitalWrite(stepPin4, HIGH);
        
        delayMicroseconds(125);
        digitalWrite(stepPin3, LOW);
        digitalWrite(stepPin4, LOW);
        delayMicroseconds(125);
    }

  }

  if(lastState6 == HIGH && currentState6 == LOW && currentState7 == HIGH){
    digitalWrite(dirPin5, LOW);
     Serial.println("RIGHT");
      for (int i=0; i < 0.1 * stepsPerRevolution; i++) {
    // These four lines result in 1 step:
        digitalWrite(stepPin5, HIGH);
        
        delayMicroseconds(125);
        digitalWrite(stepPin5, LOW);
        delayMicroseconds(125);
    }

  }
    
   if(lastState7 == HIGH && currentState7 == LOW && currentState6 == HIGH){
    digitalWrite(dirPin5, HIGH);
     Serial.println("LEFT");
      for (int i=0; i < 0.1 * stepsPerRevolution; i++) {
    // These four lines result in 1 step:
        digitalWrite(stepPin5, HIGH);
        
        delayMicroseconds(125);
        digitalWrite(stepPin5, LOW);
        delayMicroseconds(125);
    }
}

}
