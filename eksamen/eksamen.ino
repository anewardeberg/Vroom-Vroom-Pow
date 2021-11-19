// EKSAMENSDOKUMENT HELT PÅ EKTE
#include <EEPROM.h>
#include "pitches.h"

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
const int speakerPin = 7;


//int deltatime SPØR MATS OM DETTE NÅR DEN TID KOMMER <3



// TFT VARIABLES / SETUP
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define BLACK 0x0000
#define BROWN 0x7A40
#define BEIGE 0xC4CE
#define GREEN 0x07E0
#define GRAY 0x9492
#define SKY_BLUE 0x971F
#define BLUE 0x063F
#define ASPHALT_GRAY 0x5289
#define YELLOW 0xFE47
#define PINK 0xEB93
#define WHITE 0xFFFF
#define PURPLE 0xD2BF
#define ORANGE 0xFC80
#define RED 0xF920

uint16_t carColorArray[4] = {PINK, BLUE, GREEN, YELLOW};
uint16_t lazerColorArray[6] = {PINK, BLUE, GREEN, PURPLE, ORANGE, RED};
uint16_t carColor = PINK;
uint16_t lazerColor = GREEN;

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
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin4, INPUT);
  tft.init(135, 240);
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  splashScreen();

  // Uncomment to reset high score:
  //   EEPROM.write(0, 0);
}

void loop() {
  highScore = EEPROM.read(0);
  showHomeScreen();
  startGame(2);

  delay(200);
  while (userInGame) {
    obstacleY = obstacleY + gameSpeed;
    drawLogs(obstacleX, obstacleY);
    buttonState3 = digitalRead(buttonPin3);
    buttonState2 = digitalRead(buttonPin2);
    buttonState4 = digitalRead(buttonPin4);

    sensorValueX = analogRead(analogInPinX);
    outputValueX = map(sensorValueX, 0, 1023, 0, 255);
    if (obstacleY >= 150) {
      obstacleY = -30;
      obstacleX = rand() % 80 + 60;
      score++;
      if (score % 10 == 0) {
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

    if ((obstacleY + 15 >= 95) &&
        (obstacleY + 15 <= 110) &&
        (carX >= obstacleX) &&
        (carX <= obstacleX + 40)) {
      gameOver();
    } else if ((obstacleY + 15 >= 95) &&
               (obstacleY + 15 <= 110) &&
               (carX + 10 >= obstacleX) &&
               (carX + 10 <= obstacleX + 40)) {
      gameOver();
    }
  }
}

void showHomeScreen() {
  tft.fillRect(0, 0, 240, 100, SKY_BLUE);
  tft.fillRect(0, 100, 240, 35, GREEN);
  tft.fillRect(0, 100, 240, 8, ASPHALT_GRAY);

  tft.fillCircle(20, 46, 10, GRAY);
  tft.fillCircle(40, 25, 10, GRAY);
  tft.fillCircle(60, 45, 10, GREEN);
  tft.fillCircle(40, 65, 10, GRAY);

  drawHomeScreenCars();
  tft.setTextSize(2);

  tft.setCursor(10, 115);
  tft.print("High score: ");
  tft.print(highScore);
  tft.setCursor(90, 43);
  tft.print("Start game");
}

void startGame(int buttonPin) {
  delay(200);
  int buttonState = 0;

  while (1) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      buttonState = 0;
      lazerColor = lazerColorArray[random(6)];
      selectCarColor();
      showGameInstructions();
      setGameBackground();
      drawCar(carX, carColor);
      showGameStats();
      tft.setTextColor(WHITE);
      delay(1000);
      for (int i = 3; i > 0; i--) {
        tft.setCursor(115, 50);
        tone(speakerPin, NOTE_C4);
        tft.print(i);
        delay(800);
        noTone(speakerPin);
        delay(200);
        tft.fillRect(110, 45, 20, 20, ASPHALT_GRAY);
      }
      tft.setCursor(85, 50);
      tone(speakerPin, NOTE_C5);
      tft.print("START!");
      delay(800);
      noTone(speakerPin);
      delay(200);
      tft.fillRect(75, 45, 80, 20, ASPHALT_GRAY);
      delay(1000);
      userInGame = true;
      return;
    }
  }
}

void selectCarColor() {
  buttonState1 = 0;
  bool userHasChosenColor = false;
  int i = 0;
  tft.fillScreen(BLACK);
  tft.setCursor(25, 40);
  tft.print("SELECT CAR COLOR");
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
    if (outputValueX >= 180) {
      i++;
      if (i > 3) {
        i = 3;
      }
    } else if (outputValueX <= 60) {
      i--;
      if (i < 0) {
        i = 0;
      }
    }

    Serial.print("i: ");
    Serial.println(i);


    if (i == 0) {
      tft.drawRect(14, 77, 40, 40, GREEN);
      tft.drawRect(70, 77, 40, 40, BLACK);
      tft.drawRect(126, 77, 40, 40, BLACK);
      tft.drawRect(182, 77, 40, 40, BLACK);
    } else if (i == 1) {
      tft.drawRect(14, 77, 40, 40, BLACK);
      tft.drawRect(70, 77, 40, 40, GREEN);
      tft.drawRect(126, 77, 40, 40, BLACK);
      tft.drawRect(182, 77, 40, 40, BLACK);
    } else if (i == 2) {
      tft.drawRect(14, 77, 40, 40, BLACK);
      tft.drawRect(70, 77, 40, 40, BLACK);
      tft.drawRect(126, 77, 40, 40, GREEN);
      tft.drawRect(182, 77, 40, 40, BLACK);
    } else if (i == 3) {
      tft.drawRect(14, 77, 40, 40, BLACK);
      tft.drawRect(70, 77, 40, 40, BLACK);
      tft.drawRect(126, 77, 40, 40, BLACK);
      tft.drawRect(182, 77, 40, 40, GREEN);
    }
    carColor = carColorArray[i];
    delay(200);

    if (buttonState1 == HIGH) {
      buttonState1 = 0;
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
  for (int i = 1000; i > 700; i--) {
    tone(speakerPin, i);
    delay(1);
  }
  noTone(speakerPin);
  delay(800);
  if ((x >= obstacleX) && (x <= obstacleX + 40)) {
    obstacleY = -100;
    obstacleX = rand() % 80 + 60;
    tft.fillRect(60, 0, 120, 95, ASPHALT_GRAY);
  } else if ((x + 9 >= obstacleX) && (x + 9 <= obstacleX + 40)) {
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

void showGameInstructions() {
  tft.fillRect(30, 15, 26, 7, GRAY);
  tft.fillRect(40, 22, 6, 12, ASPHALT_GRAY);
  tft.fillRect(24, 34, 38, 10, GRAY);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(80, 25);
  tft.print("MOVE CAR");

  tft.fillCircle(175, 90, 10, PINK);
  tft.fillCircle(195, 70, 10, GRAY);
  tft.fillCircle(215, 90, 10, GRAY);
  tft.fillCircle(195, 110, 10, GRAY);
  tft.drawLine(160, 110, 175, 90, PINK);
  tft.drawFastHLine(20, 110, 140, PINK);
  tft.setCursor(20, 85);
  tft.print("SHOOT LAZER");


  tft.setCursor(0, 125);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  for (int i = 6; i > 0; i--) {
    tft.setCursor(0, 125);
    tft.print(i);
    delay(1000);
    tft.fillRect(0, 120, 10, 15, BLACK);
  }

}

void updateGameStats() {
  tft.fillRect(0, 0, 60, 70, GREEN);
  tft.fillRect(180, 0, 60, 70, GREEN);
  showGameStats();
}

void splashScreen() {
  tft.fillRect(0, 0, 240, 100, SKY_BLUE);
  tft.fillRect(0, 100, 240, 35, GREEN);
  tft.fillRect(0, 100, 240, 8, ASPHALT_GRAY);
  tft.setCursor(30, 30);  
  tft.setTextSize(2);
  tft.print("VROOM VROOM POW");
  sideCarAnimation();
  delay(1000);
}

void sideCarAnimation() {
  for (int i = -40; i < 240; i++) {
    tft.fillRect(i, 86, 25, 6, carColor);
    tft.fillRect(i, 92, 35, 10, carColor);
    tft.fillRect(i + 18, 87, 7, 4, GRAY);
    tft.fillRect(i + 33, 93, 2, 3, YELLOW);
    tft.fillRect(i + 3, 100, 5, 5, BLACK);
    tft.fillRect(i + 25, 100, 5, 5, BLACK);

    tft.fillRect(i - 5, 102, 8, 3, ASPHALT_GRAY);
    tft.fillRect(i + 8, 102, 17, 3, ASPHALT_GRAY);
    tft.fillRect(i - 5, 100, 5, 2, ASPHALT_GRAY);
    tft.fillRect(i - 5, 86, 5, 14, SKY_BLUE);
    delay(10);
  }
}

void drawHomeScreenCars() {
  int sideCarX = 30;
  for (int i = 0; i < 4; i++) {
    tft.fillRect(sideCarX, 86, 25, 6, carColorArray[i]);
    tft.fillRect(sideCarX, 92, 35, 10, carColorArray[i]);
    tft.fillRect(sideCarX + 18, 87, 7, 4, GRAY);
    tft.fillRect(sideCarX + 33, 93, 2, 3, YELLOW);
    tft.fillRect(sideCarX + 3, 100, 5, 5, BLACK);
    tft.fillRect(sideCarX + 25, 100, 5, 5, BLACK);
    sideCarX = sideCarX + 50;
  }
}

void drawLogs(int x, int y) {
  tft.fillRect(x, y, 40, 15, BROWN);
  tft.fillRect(x + 2, y + 2, 10, 10, BEIGE);

  tft.fillRect(x + 2, y, 10, 2, BROWN);
  tft.fillRect(x, y - 15, 40, 15, ASPHALT_GRAY);
}

void gameOver() {
  tone(speakerPin, NOTE_C2);
  delay(1500);
  noTone(speakerPin);
  tft.setTextColor(WHITE);
  tft.fillRect(0, 0, 240, 100, SKY_BLUE);
  tft.fillRect(0, 100, 240, 35, GREEN);
  tft.fillRect(0, 100, 240, 8, ASPHALT_GRAY);
  tft.fillRect(70, 86, 25, 6, carColor);
  tft.fillRect(70, 92, 35, 10, carColor);
  tft.fillRect(88, 87, 7, 4, GRAY);
  tft.fillRect(103, 93, 2, 3, YELLOW);
  tft.fillRect(73, 100, 5, 5, BLACK);
  tft.fillRect(95, 100, 5, 5, BLACK);
  tft.setCursor(20, 23);
  tft.println("GAME OVER");
  tft.setCursor(20, 43);
  if (score > highScore) {
    EEPROM.write(0, score);
    tft.print("New high score: ");
  } else {
    tft.print("Score: ");
  }
  tft.print(score);
  delay(5000);

  obstacleY = -60;
  score = 0;
  carX = 115;
  userInGame = false;
  lazersAvailable = 5;
  tft.fillScreen(BLACK);
  showHomeScreen();
  startGame(2);
}
