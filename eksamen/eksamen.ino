// EKSAMENSDOKUMENT HELT PÅ EKTE
#include <EEPROM.h>

// MISC. VARIABLES
bool userInGame = false;
int obstacleX = 60;
int obstacleY = -30;
int carX = 115;
int carMaxX = 169;
int carMinX = 61;
int gameSpeed = 1;
int score = 0;
int lazersAvailable = 5;
int speedUpScore = 0;
int highScore = 0;

//int deltatime SPØR MATS OM DETTE NÅR DEN TID KOMMER <3



// TFT VARIABLES / SETUP
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define BLACK 0x0000
#define BROWN 0x7A40
#define GREEN 0x07E0
#define GRAY 0x9492
#define BLUE 0x063F
#define ASPHALT_GRAY 0x5289
#define YELLOW 0xFE47
#define PINK 0xEB93
#define WHITE 0xFFFF

uint16_t carColorArray[] = {PINK, BLUE, GREEN, YELLOW};
uint16_t carColor = BLUE;
uint16_t lazerColor = PINK;

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
    obstacleY = obstacleY + gameSpeed;
    drawPillars(obstacleX, obstacleY);
    buttonState3 = digitalRead(buttonPin3);
    buttonState2 = digitalRead(buttonPin2);
    buttonState4 = digitalRead(buttonPin4);

    sensorValueX = analogRead(analogInPinX);
    //    sensorValueY = analogRead(analogInPinY);
    // map it to the range of the analog out:
    outputValueX = map(sensorValueX, 0, 1023, 0, 255);
    if (obstacleY >= 150) {
      obstacleY = -30;
      obstacleX = rand() % 80 + 60;
      score++;
      if(score%10 == 0) {
        lazersAvailable++;
      }
      updateGameStats();
    }

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
    if ((obstacleY + 15 >= 95) && (obstacleY + 15 <= 110) && (carX >= obstacleX) && (carX <= obstacleX + 40)) {
      gameOver();
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
  tft.fillCircle(40, 50, 10, GRAY);
  tft.fillCircle(60, 70, 10, GREEN);
  tft.fillCircle(40, 90, 10, GRAY);

  tft.setTextSize(2);
  //  tft.fillCircle(90, 50, 8, GREEN);
  //  tft.setCursor(105, 47);
  //  tft.print("Start game");
  //  tft.fillCircle(90, 70, 8, PINK);
  tft.setCursor(90, 63);
  tft.print("Start game");
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
      tft.setTextColor(WHITE);
      delay(1000);
      tft.setCursor(115, 50);
      tft.print("3");
      delay(1000);
      tft.fillRect(110, 45, 20, 20, ASPHALT_GRAY);
      tft.setCursor(115, 50);
      tft.print("2");
      delay(1000);
      tft.fillRect(110, 45, 20, 20, ASPHALT_GRAY);
      tft.setCursor(115, 50);
      tft.print("1");
      delay(1000);
      tft.fillRect(110, 45, 20, 20, ASPHALT_GRAY);
      tft.setCursor(85, 50);
      tft.print("START!");
      delay(1000);
      tft.fillRect(75, 45, 80, 20, ASPHALT_GRAY);
      userInGame = true;
      return;
    }
  }
}

void chooseCarColor() {
  bool userHasChosenColor = false;
  int i = 0;
  tft.fillScreen(BLACK);
  tft.setCursor(25, 40);
  tft.print("CHOOSE CAR COLOR");
  while (!userHasChosenColor) {
    buttonState1 = digitalRead(buttonPin1);
    tft.fillRect(17, 80, 34, 34, carColorArray[0]);
    tft.fillRect(73, 80, 34, 34, carColorArray[1]);
    tft.fillRect(129, 80, 34, 34, carColorArray[2]);
    tft.fillRect(185, 80, 34, 34, carColorArray[3]);
    //    Serial.println(outputValueX);
    //
    sensorValueX = analogRead(analogInPinX);
    outputValueX = map(sensorValueX, 0, 1023, 0, 255);
    Serial.print(outputValueX);
    if (outputValueX >= 160) {
      i++;
      if (i > 3) {
        i = 3;
      }
      delay(200);
    } else if (outputValueX <= 80) {
      i--;
      if (i < 0) {
        i = 0;
      }
      delay(200);
    }

    Serial.print("i: ");
    Serial.println(i);

    if (i = 0) {
      tft.drawRect(14, 77, 40, 40, GREEN);
      tft.drawRect(70, 77, 40, 40, BLACK);
      tft.drawRect(126, 77, 40, 40, BLACK);
      tft.drawRect(182, 77, 40, 40, BLACK);
    } else if (i = 1) {
      tft.drawRect(14, 77, 40, 40, BLACK);
      tft.drawRect(70, 77, 40, 40, GREEN);
      tft.drawRect(126, 77, 40, 40, BLACK);
      tft.drawRect(182, 77, 40, 40, BLACK);
    } else if (i = 2) {
      tft.drawRect(14, 77, 40, 40, BLACK);
      tft.drawRect(70, 77, 40, 40, BLACK);
      tft.drawRect(126, 77, 40, 40, GREEN);
      tft.drawRect(182, 77, 40, 40, BLACK);
    } else if (i = 3) {
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
  if ((x >= obstacleX) && (x <= obstacleX + 40)) {
    obstacleY = -100;
    obstacleX = rand() % 80 + 60;
    tft.fillRect(60, 0, 120, 95, ASPHALT_GRAY);
  }
  tft.fillRect(x + 1, 0, 3, 95, ASPHALT_GRAY);
  tft.fillRect(x + 7, 0, 3, 95, ASPHALT_GRAY);
  delay(500);

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

void beep() {
  // insert tut-funksjon here
  //  (takk mats)
}

void drawPillars(int x, int y) {
  //  if (x >= 270) {
  tft.fillRect(x, y, 40, 15, BROWN);
  //  tft.fillRect(x + 90, y, 40, 15, BROWN);
  tft.fillRect(x, y - 15, 40, 15, ASPHALT_GRAY);
  //  tft.fillRect(x + 90, y - 15, 40, 15, ASPHALT_GRAY);
  if (x <= 60) {

  }
}

void gameOver() {
  delay(2000);
  tft.setTextColor(WHITE);
  tft.fillScreen(BLACK);
  obstacleY = -60;
  tft.setCursor(65, 63);
  tft.print("GAME OVER");
  delay(2000);
  score = 0;
  userInGame = false;
  lazersAvailable = 5;
  tft.fillScreen(BLACK);
  showHomeScreen();
  startGame(2);
}
