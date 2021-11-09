// EKSAMENSDOKUMENT HELT PÅ EKTE

// MISC. VARIABLES
bool userInGame = false;

// TFT VARIABLES / SETUP
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define PINK 0xF81F
#define BLACK 0x0000
#define GREEN 0x07E0
#define GRAY 0x9492
#define BLUE 0x063F

#if defined(ARDUINO_FEATHER_ESP32)
#define TFT_CS         14
#define TFT_RST        15
#define TFT_DC         32

#elif defined(ESP8266)
#define TFT_CS         4
#define TFT_RST        16
#define TFT_DC         5

#else
#define TFT_CS        10
#define TFT_RST        9
#define TFT_DC         8
#endif

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

#define I2C_ADDR 0x27
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

// BUTTONS VARIABLES
const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4 = 5;

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;

// JOYSTICK VARIABLES

void setup() {
  Serial.begin(57600);
  tft.init(135, 240);
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.setCursor(8, 50);
  tft.println("fakk alt corp. 2021");
  //  delay(3000);
  tft.fillScreen(BLACK);


}

void loop() {
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.println("Press button");
  tft.print("to start game");
  tft.fillCircle(20, 70, 10, GRAY);
  tft.fillCircle(40, 50, 10, GRAY);
  tft.fillCircle(60, 70, 10, GREEN);
  tft.fillCircle(40, 90, 10, GRAY);
  startGame(2); 
  delay(200);
  while(userInGame) {
//  tft.fillScreen(GREEN);
//  tft.setCursor(25, 50);
//  tft.println("Game started");
  drawPillars(100, 40);
  delay(2000);
  }
    
  

//  buttonState1 = digitalRead(buttonPin1);
//  buttonState2 = digitalRead(buttonPin2);
//  buttonState3 = digitalRead(buttonPin3);
//  buttonState4 = digitalRead(buttonPin4);
//
//  if (buttonState1 == HIGH) {
//    showMenuScreen();
//  } else if (buttonState2 == HIGH) {
//    showMenuScreen();
//  } else if (buttonState3 == HIGH) {
//    showMenuScreen();
//  } else if (buttonState4 == HIGH) {
//    showMenuScreen();
//  }
}

void startGame(int buttonPin) {
  int buttonState = 0;
  
  while (1) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      tft.fillScreen(BLACK);
      userInGame = true;
      return;
    }
  }
}

void drawPillars(int x, int y) {
    if (x>=270){
      tft.fillRect(318, 0, x, y-1, PINK);
      tft.drawRect(319, 0, x-1, y, BLACK);
      tft.fillRect(318, y+81, x, 203, GRAY);
      tft.drawRect(319, y+80, x-1, 204, BLACK); 
    }
    else if( x<=268) {
      // Draws blue rectangle right of the pillar
      tft.fillRect(x+51, 0, x+60, y, BLUE);
      // Draws the pillar

      tft.fillRect(x+49, 1, x+1, y-1, GRAY);
  
      tft.drawRect(x+50, 0, x, y, BLACK);
      // Draws the blue rectangle left of the pillar
      tft.fillRect(x-1, 0, x-3, y, BLUE);
      // The bottom pillar
      tft.fillRect(x+51, y+80, x+60, 204, PINK);
      tft.fillRect(x+49, y+81, x+1, 203, GRAY);
      tft.drawRect(x+50, y+80, x, 204, BLACK);
      tft.fillRect(x-1, y+80, x-3, 204, BLUE);
  }
}
