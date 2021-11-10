// EKSAMENSDOKUMENT HELT PÅ EKTE
#include <EEPROM.h>

// MISC. VARIABLES
bool userInGame = false;
int obstacleX = 80;
int obstacleY = 30;
int carX = 115;
int carMaxX = 169;
int carMinX = 61;
int speed = 3;
int score = 0;
int lazersAvailable = 5;
int speedUpScore = 0;
int highScore = 0;



// TFT VARIABLES / SETUP
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define BLACK 0x0000
#define GREEN 0x07E0
#define GRAY 0x9492
#define BLUE 0x063F
#define ASPHALT_GRAY 0x5289
#define YELLOW 0xFE47
#define PINK 0xEB93
#define WHITE 0xFFFF

uint16_t carColorArray[] = {PINK, BLUE, GREEN, YELLOW};
uint16_t carColor = PINK;
uint16_t lazerColor = BLUE;

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
const int analogInPinX = A0;
const int analogInPinY = A1;


int sensorValueX = 0;
//int sensorValueY = 0;
int outputValueX = 0;
//int outputValueY = 0;

void setup() {
  Serial.begin(9600);
  tft.init(135, 240);
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.setCursor(8, 50);
  tft.println("fakk alt corp. 2021");
  //  delay(3000);
  tft.fillScreen(BLACK);
  highScore = EEPROM.read(0);

}

void loop() {
  //  tft.setTextSize(2);
  //  tft.setCursor(0, 0);
  //  tft.println("Press button");
  //  tft.print("to start game");
  showHomeScreen();
  startGame(2);
  delay(200);
  while (userInGame) {
    buttonState3 = digitalRead(buttonPin3);
    buttonState2 = digitalRead(buttonPin2);
    buttonState4 = digitalRead(buttonPin4);

    sensorValueX = analogRead(analogInPinX);
    //    sensorValueY = analogRead(analogInPinY);
    // map it to the range of the analog out:
    outputValueX = map(sensorValueX, 0, 1023, 0, 255);
    //    outputValueY = map(sensorValueY, 0, 1023, 0, 255);
    if (buttonState3 == HIGH && lazersAvailable > 0) {
      activateLazer(carX);
      updateGameStats();
      buttonState3 == LOW;
    }

    if (outputValueX > 150) {
      carX++;
      moveCarRight(carX);
    }

    if (outputValueX < 100) {
      carX--;
      moveCarLeft(carX);
    }
    //  tft.fillScreen(GREEN);
    //  tft.setCursor(25, 50);
    //  tft.println("Game started");
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

void showHomeScreen() {
  tft.fillCircle(20, 70, 10, GRAY);
  tft.fillCircle(40, 50, 10, PINK);
  tft.fillCircle(60, 70, 10, GREEN);
  tft.fillCircle(40, 90, 10, GRAY);

  tft.setTextSize(1.5);
  tft.fillCircle(90, 50, 8, GREEN);
  tft.setCursor(105, 47);
  tft.print("Start game");
  tft.fillCircle(90, 70, 8, PINK);
  tft.setCursor(105, 67);
  tft.print("Car color");
}

void startGame(int buttonPin) {
  int buttonState = 0;

  while (1) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      buttonState = 0;
      chooseCarColor();
      setGameBackground();
      drawCar(carX, carColor);
      showGameStats();
      userInGame = true;
      return;
    }
  }
}

void chooseCarColor() {
  bool userHasChosenColor = false;
  int i = 0;
  tft.fillScreen(BLACK);
  tft.print("CHOOSE CAR COLOR");
  while (!userHasChosenColor) {
    buttonState1 = digitalRead(buttonPin1);
    tft.fillRect(17, 80, 34, 34, carColorArray[0]);
    tft.fillRect(73, 80, 34, 34, carColorArray[1]);
    tft.fillRect(129, 80, 34, 34, carColorArray[2]);
    tft.fillRect(185, 80, 34, 34, carColorArray[3]);
    Serial.println(outputValueX);
    
    sensorValueX = analogRead(analogInPinX);
    outputValueX = map(sensorValueX, 0, 1023, 0, 255);
    tft.drawRect(14, 77, 40, 40, GREEN);
    switch (outputValueX) {
      case 160 ... 255:
        i++;
        if (i > 3) {
          i = 3;
        }
      case 0 ... 80:
        i--;
        if (i < 0) {
          i = 0;
        }
      default:
        i = 0;
    }

    while (i = 0) {
      tft.drawRect(14, 77, 40, 40, GREEN);
      tft.drawRect(70, 77, 40, 40, BLACK);
      tft.drawRect(126, 77, 40, 40, BLACK);
      tft.drawRect(182, 77, 40, 40, BLACK);
    }

    while (i = 1) {
      tft.drawRect(14, 77, 40, 40, BLACK);
      tft.drawRect(70, 77, 40, 40, GREEN);
      tft.drawRect(126, 77, 40, 40, BLACK);
      tft.drawRect(182, 77, 40, 40, BLACK);
    }
    while (i = 2) {
      tft.drawRect(14, 77, 40, 40, BLACK);
      tft.drawRect(70, 77, 40, 40, BLACK);
      tft.drawRect(126, 77, 40, 40, GREEN);
      tft.drawRect(182, 77, 40, 40, BLACK);
    }
    while (i = 3) {
      tft.drawRect(14, 77, 40, 40, BLACK);
      tft.drawRect(70, 77, 40, 40, BLACK);
      tft.drawRect(126, 77, 40, 40, BLACK);
      tft.drawRect(182, 77, 40, 40, GREEN);
    }

    if (buttonState1 == HIGH) {
      userHasChosenColor = true;
    }

  }
  tft.fillScreen(BLACK);
}

void moveCarRight(int x) {
  if (x >= carMaxX) {
    x = carMaxX;
    carX = carMaxX;
  }
  tft.fillRect(x, 95, 10, 15, carColor);
  tft.fillRect(x + 1, 98, 8, 4, GRAY);
  tft.fillRect(x + 1, 95, 2, 1, YELLOW);
  tft.fillRect(x + 7, 95, 2, 1, YELLOW);

  tft.fillRect(x - 1, 95, 1, 15, ASPHALT_GRAY);
}

void moveCarLeft(int x) {
  if (x <= carMinX) {
    x = carMinX;
    carX = carMinX;
  }
  tft.fillRect(x, 95, 10, 15, carColor);
  tft.fillRect(x + 1, 98, 8, 4, GRAY);
  tft.fillRect(x + 1, 95, 2, 1, YELLOW);
  tft.fillRect(x + 7, 95, 2, 1, YELLOW);

  tft.fillRect(x + 11, 95, 1, 15, ASPHALT_GRAY);
}


void activateLazer(int x) {
  lazersAvailable--;
  tft.fillRect(x + 1, 0, 3, 95, lazerColor);
  tft.fillRect(x + 7, 0, 3, 95, lazerColor);
  tft.fillRect(x + 2, 0, 1, 95, WHITE);
  tft.fillRect(x + 8, 0, 1, 95, WHITE);
  delay(1000);
  tft.fillRect(x + 1, 0, 3, 95, ASPHALT_GRAY);
  tft.fillRect(x + 7, 0, 3, 95, ASPHALT_GRAY);
}

void drawCar(int x, uint16_t carColor) {
  tft.fillRect(x, 95, 10, 15, carColor);
  tft.fillRect(x + 1, 98, 8, 4, GRAY);
  tft.fillRect(x + 1, 95, 2, 1, YELLOW);
  tft.fillRect(x + 7, 95, 2, 1, YELLOW);
}

void setGameBackground() {
  tft.fillScreen(ASPHALT_GRAY);
  tft.fillRect(0, 0, 60, 135, GREEN);
  tft.fillRect(180, 0, 60, 135, GREEN);

  // fartsstriper 8D
  //  tft.fillRect(118, 5, 4, 30, YELLOW);
  //  tft.fillRect(118, 50, 4, 30, YELLOW);
  //  tft.fillRect(118, 95, 4, 30, YELLOW);
}

void showGameStats() {
  tft.setCursor(3, 1);
  tft.setTextColor(BLACK);
  tft.setTextSize(1.5);
  tft.println("score:");
  tft.setCursor(3, 10);
  tft.setTextSize(2.5);
  tft.println(score);
  tft.setCursor(3, 30);
  tft.setTextSize(1.5);
  tft.println("lazers:");
  tft.setCursor(3, 40);
  tft.setTextSize(2.5);
  tft.print(lazersAvailable);
}

void updateGameStats() {
  tft.fillRect(0, 0, 60, 70, GREEN);
  tft.fillRect(180, 0, 60, 70, GREEN);
  showGameStats();
}

void drawPillars(int x, int y) {
  if (x >= 270) {
    tft.fillRect(318, 0, x, y - 1, PINK);
    tft.drawRect(319, 0, x - 1, y, BLACK);
    tft.fillRect(318, y + 81, x, 203, GRAY);
    tft.drawRect(319, y + 80, x - 1, 204, BLACK);
  }
  else if ( x <= 268) {
    // Draws blue rectangle right of the pillar
    tft.fillRect(x + 51, 0, x + 60, y, BLUE);
    // Draws the pillar
    tft.fillRect(x + 49, 1, x + 1, y - 1, GRAY);

    tft.drawRect(x + 50, 0, x, y, BLACK);
    // Draws the blue rectangle left of the pillar
    tft.fillRect(x - 1, 0, x - 3, y, BLUE);
    // The bottom pillar
    tft.fillRect(x + 51, y + 80, x + 60, 204, PINK);
    tft.fillRect(x + 49, y + 81, x + 1, 203, GRAY);
    tft.drawRect(x + 50, y + 80, x, 204, BLACK);
    tft.fillRect(x - 1, y + 80, x - 3, 204, BLUE);
  }
}
