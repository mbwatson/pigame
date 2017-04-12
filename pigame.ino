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
// 1000(-ish?) digits of pi
String pi =
"3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706"
"7982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381"
"9644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412"
"7372458700660631558817488152092096282925409171536436789259036001133053054882046652138414695194151160"
"9433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949"
"1298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051"
"3200056812714526356082778577134275778960917363717872146844090122495343014654958537105079227968925892"
"3542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318"
"5950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473"
"035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989";
String buff = "";
int n = 0; // position
char key = {};
int high_score = 0;

//string substr(n,k) {
//  return
//}

int show(String text, int wait = 0) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  for (int i = 0; i < text.length(); i++) {
    display.print(text[i]);
  }
  display.display();
  delay(wait);
  return 1;
}

void initGame() {
  buff = "";
  show("\nGET READY!", 1000);
  show("\nEnter digits!");
  n = 0;
}

void ledAlert(int gap = 100, int count = 1) {
  for (int i = 0; i < count; i++) {
    if (i % 2 == 0) {
      digitalWrite(redledpin, HIGH);
    } else {
      digitalWrite(redledpin, LOW);
    }
    delay(gap);
  }
  digitalWrite(redledpin, HIGH);
}

void setup()
{
  pinMode(0,OUTPUT);
  ledAlert(50,10);
  
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  initGame();
}

void loop()
{
  display.clearDisplay();
  key = kpd.getKey();
  if (key) {
    if (key == pi[n]){
      buff += pi[n];
      n += 1;
      // resize string of characters displayed
      if (n > 20) {
        buff = pi.substring(n - 20, n);
      } else {
        // if at the beginning, add decimal point to buffer
        if (n == 1) {
          buff += pi[n];
          n += 1;
        }
      }
      
      Serial.print(buff);
      show("\n" + buff, 0);
      
      delay(25);
    } else {
      if (n-1 > high_score) {
        high_score = n-1;
      }
      
      // alert error
      ledAlert(50,10);
      
      // Serial note
      Serial.println("Nope! " + pi.substring(n,n+1) + ".\nScore: " + (n-1) + "\nHigh score:" + high_score);
      
      // OLED note
      show("Nope! " + pi.substring(n,n+1) + ".\nScore: " + (n-1) + "\nHi score: " + high_score, 2000);
      // Start over
      initGame();
    }
  }
}

