/*  pigame.ino
 *
 *  Game to memorize tpi's decimal expansion
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
String buffr = "";
const int buffr_size = 13;
int n = 0; // position
char key = {};
int score = 0;
int high_score = 0;

void ledAlert(int gap = 100, int count = 1)
{
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

int show(int x, int y, String text, int size = 1)
{
  display.setTextSize(size);
  display.setCursor(x,y);
  display.setTextColor(WHITE);
  for (int i = 0; i < text.length(); i++) {
    display.print(text[i]);
  }
  display.display();
  return 1;
}

int type(int x, int y, String text, int size = 1)
{
  display.setTextSize(size);
  display.setCursor(x,y);
  display.setTextColor(WHITE);
  for (int i = 0; i < text.length(); i++) {
    display.print(text[i]);
    display.display();
    delay(5);
  }
  display.display();
  return 1;
}

void showStats()
{
  display.setTextSize(1);
  // progress
  display.setCursor(24,8);
  display.println(buffr);
  // current position
  display.setCursor(0,25);
  display.print(String(score));
  // best position
  display.setCursor(120,25);
  display.print(String(high_score));
  display.display();
}

void gameOverScreen()
{
  ledAlert(50,10);
  display.clearDisplay();
  type(40,0,"GAME",2);
  type(40,15,"OVER",2);
  delay(1000);
  display.clearDisplay();
  type(0,0,"Next digit: " + pi.substring(n,n+1));
  delay(250);
  type(0,12,"Score: " + String(n-1));
  delay(250);
  type(0,24,"High score: " + String(high_score));

//  key = kpd.getKey();
  
}

void gameStartScreen()
{
  display.clearDisplay();
  type(32,12,"Get ready...");
  delay(500);
  display.clearDisplay();
  for(int i = 0; i < 3; i++) {
    display.clearDisplay();
    display.display();
    delay(250);
    type(48,10,"GO!",2);
    delay(250);
  }
  showStats();
}

void gameInit()
{
  buffr = "";
  n = 0;
  score = 0;
  gameStartScreen();
}

void setup()
{
  pinMode(0,OUTPUT);
  ledAlert(50,10);
  
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  gameInit();
}

void loop()
{
  display.clearDisplay();
  key = kpd.getKey();
  if (key) {
    switch (key) {
      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        // got it right
        if (key == pi[n]) {
          
          // append next digit to display
          buffr += pi[n];
          
          // remove first digit from display
          if (n > buffr_size) {
            buffr.remove(0,1);
          } else {
            // if at the beginning, add decimal point to buffer
            if (n == 0) {
              buffr += pi[n+1];
              n += 1;
            }
          }
          
          // increase position
          n += 1;
          
          // update scores
          score += 1;
          high_score = std::max(score, high_score);
          
          showStats();
          
          delay(25);
        // got it wrong
        } else {
          gameOverScreen();
          delay(1500);
          gameInit();
        }
        break;
      case '*':
        break;
      case '#':
        break;
      default:
        delay(1);
    }
  }
}

