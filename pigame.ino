/*  Keypadtest.pde
 *
 *  Demonstrate the simplest use of the  keypad library.
 *
 *  The first step is to connect your keypad to the
 *  Arduino  using the pin numbers listed below in
 *  rowPins[] and colPins[]. If you want to use different
 *  pins then  you  can  change  the  numbers below to
 *  match your setup.
 *
 */
#include <Keypad.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 12, 4, 5, 16 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 13, 14, 2 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Adafruit_SSD1306 display = Adafruit_SSD1306();

#define redledpin 0
const char pi[] = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989";
int n = 0;
char key = {};

void setup()
{
  pinMode(0,OUTPUT);
  // LED flashy thing to indicate successful start
  for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      digitalWrite(redledpin, HIGH);
    } else {
      digitalWrite(redledpin, LOW);
    }
    delay(50);
  }
  digitalWrite(redledpin, HIGH);
  
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.print("pi = ");
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.print("pi=");
  display.display();
}

void loop()
{
  display.clearDisplay();
  key = kpd.getKey();
  if (key) {
    if (key == pi[n]){
      n += 1;
      Serial.print(key);
      display.print(key);
      if (n == 1) {
        Serial.print(".");
        display.print(".");
        n += 1;
      }
      
      display.clearDisplay();
      
      // show digits
      display.setCursor(0,0);
      display.print("pi=");
      for (int i = 0; i < n; i++) {
        display.print(pi[i]);
      }
      display.display();
      
      delay(25);
    } else {
      digitalWrite(redledpin, LOW);
      delay(250);
      digitalWrite(redledpin, HIGH);
      Serial.println();
      Serial.println();
      Serial.print("Nope: ");
      Serial.println(pi[n]);
      Serial.print("Congratulations! You made it to digit ");
      Serial.print(n-1);
      Serial.print(".");
      Serial.println();
      Serial.println();
      Serial.print("pi=");
      
      display.clearDisplay();
      display.setCursor(0,0);
      display.print("Nope! ");
      display.println(pi[n]);
      display.print("Digits: ");
      display.println(n-1);
      display.display();
      delay(2000);
      display.clearDisplay();
      display.setCursor(0,0);
      display.print("pi=");
      display.display();
      
      n = 0;
    }
  }
}

