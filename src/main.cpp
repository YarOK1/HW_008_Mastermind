#include <Arduino.h> // Обов’язковий заголовок для PlatformIO
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
/*
TFT_BLACK	0x0000	(0, 0, 0)
TFT_WHITE	0xFFFF	(255, 255, 255)
TFT_RED	0xF800	(255, 0, 0)
TFT_GREEN	0x07E0	(0, 255, 0)
TFT_BLUE	0x001F	(0, 0, 255)
TFT_CYAN	0x07FF	(0, 255, 255)
TFT_MAGENTA	0xF81F	(255, 0, 255)
TFT_YELLOW	0xFFE0	(255, 255, 0)
TFT_ORANGE	0xFD20	(255, 165, 0)
TFT_PURPLE	0x780F	(128, 0, 128)
TFT_PINK	0xF81F	(255, 192, 203)
TFT_GREY	0x8430	(128, 128, 128)
TFT_DARKGREY	0x4208	(64, 64, 64)
TFT_LIGHTGREY	0xC618	(192, 192, 192)
TFT_NAVY	0x000F	(0, 0, 128)
TFT_DARKGREEN	0x03E0	(0, 128, 0)
TFT_DARKCYAN	0x03EF	(0, 128, 128)
TFT_MAROON	0x7800	(128, 0, 0)
TFT_OLIVE	0x7BE0	(128, 128, 0)
*/

// Піни TFT. Окрім цих, підключена бібліотека використовує цифрові піни від 2 до 9
#define LCD_RST A4 // Reset
#define LCD_CS A3  // Chip Select
#define LCD_RS A2  // Register Select (Data/Command)
#define LCD_WR A1  // Write
#define LCD_RD A0  // Read

// Піни для кнопок і джойстика
#define GREEN_DOWN_PIN 22 //  analog  1
#define BLUE_LEFT_PIN 25  // digital  9
#define YELLOW_UP_PIN 23  // digital  8
#define RED_RIGHT_PIN 24  // digital 12
#define Y_AXIS_PIN A14    //  analog  2
#define X_AXIS_PIN A15    //  analog  3
#define STICK_PIN 21      //  analog  0

// Константи для графіки
#define CIRCLE_RADIUS 10
#define START_X 15          // Лівий відступ до центра першого кола секретного коду
#define START_Y 15          // Верхній відступ до центра першого кола секретного коду
#define HINT_X 132          // Лівий відступ до лівого верхнього квадратику підказок
#define CIRCLE_X_SPACING 30 // Відступ між кружальцями по Х
#define CIRCLE_Y_SPACING 30 // Відступ між кружальцями по Y

enum Color { // тип даних, який перераховує елементи і може присвоювати їм чисельні еквіваленти
  NONE = 0, RED = 1, GREEN = 2, BLUE = 3, YELLOW = 4
};

// Глобальні змінні
MCUFRIEND_kbv tft;
int secretCode[4];     // Секретний код (4 кольори)
int userGuess[4][9];   // Масив кіл: secretCode + 9 спроб гравця (10 рядків по 4 кольори)
int cursorX = 0;       // Позиція курсору по X (0-3)
int cursorY = 1;       // Позиція курсору по Y (1-9)
unsigned long lastMoveTime = 0;
const int moveDelay = 200;

void drawBoard();         // малювання ігрового поля
void generateSecretCode();// генерування паролю
void handleInput();       // обробка сигналів вводу - дій гравця
void setColor(int color); // встановлення кружальцям кольорів 
void drawCursor();        // перемальовування курсору-квадратика

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(GREEN_DOWN_PIN, INPUT);
  pinMode(BLUE_LEFT_PIN, INPUT);
  pinMode(YELLOW_UP_PIN, INPUT);
  pinMode(RED_RIGHT_PIN, INPUT);
  pinMode(X_AXIS_PIN, INPUT);
  pinMode(Y_AXIS_PIN, INPUT);
  pinMode(STICK_PIN, INPUT);

  // Ініціалізація екрану
  uint16_t ID = tft.readID(); // визначився як ID = 0x9341
  tft.begin(ID); // Запускаємо екран із визначеним ID

 // Малюємо початковий екран
 drawBoard();
 generateSecretCode();
}

void loop() {
  handleInput();
  drawCursor();
}

// Малюємо ігрове поле
void drawBoard() {
  tft.fillScreen(TFT_CYAN);
  tft.setCursor(0, 1);
  tft.setTextSize(2);
  tft.setTextColor(TFT_NAVY);
  tft.println("          Mastermind");
  tft.println("          by YarOK1");

  // Секретний код (верхній ряд)
  for (int i = 0; i < 4; i++) {
    tft.fillCircle(START_X + CIRCLE_X_SPACING * i, START_Y, CIRCLE_RADIUS, TFT_NAVY);
  }

  // Поле для спроб (9 рядів)
  for (int j = 1; j < 10; j++) {
    for (int i = 0; i < 4; i++) {
      tft.fillCircle(START_X + CIRCLE_X_SPACING * i, START_Y + CIRCLE_Y_SPACING * j, CIRCLE_RADIUS, TFT_WHITE);

      // Квадратики підказок праворуч від кожного ряду
      int hintX = HINT_X + CIRCLE_X_SPACING * i * 0.8;                  // X-координата верхнього лівого кута квадратиків підказок
      int hintY = START_Y - CIRCLE_RADIUS + CIRCLE_Y_SPACING * j; // Y-координата верхнього лівого кута квадратиків підказок 
      tft.fillRect(hintX, hintY, 20, 20, TFT_NAVY);
    }
  }
}

// Генеруємо секретний код
void generateSecretCode() {
  for (int i = 0; i < 4; i++) {
    secretCode[i] = random(1, 5); // 1-4: RED, GREEN, BLUE, YELLOW
  }
}

// Обробка вводу
void handleInput() {
  int xValue = analogRead(X_AXIS_PIN);
  int yValue = analogRead(Y_AXIS_PIN);
  unsigned long currentTime = millis();

  // Рух курсору джойстиком
  if (currentTime - lastMoveTime >= moveDelay) {
    if (xValue < 410 && cursorX < 3) cursorX++;
    else if (xValue > 600 && cursorX > 0) cursorX--;
    if (yValue < 410 && cursorY > 1) cursorY--;
    else if (yValue > 600 && cursorY < 9) cursorY++;
    lastMoveTime = currentTime;
  }

  // Вибір кольору кнопками
  if (digitalRead(RED_RIGHT_PIN) == LOW) setColor(RED);
  if (digitalRead(GREEN_DOWN_PIN) == LOW) setColor(GREEN);
  if (digitalRead(BLUE_LEFT_PIN) == LOW) setColor(BLUE);
  if (digitalRead(YELLOW_UP_PIN) == LOW) setColor(YELLOW);
}

// Встановлюємо колір
void setColor(int color) {
  userGuess[cursorX][cursorY] = color;
  int x = START_X + CIRCLE_X_SPACING * cursorX;
  int y = START_Y + CIRCLE_Y_SPACING * cursorY;
  switch (color) {
    case RED:    tft.fillCircle(x, y, CIRCLE_RADIUS, TFT_RED); break;
    case GREEN:  tft.fillCircle(x, y, CIRCLE_RADIUS, TFT_GREEN); break;
    case BLUE:   tft.fillCircle(x, y, CIRCLE_RADIUS, TFT_BLUE); break;
    case YELLOW: tft.fillCircle(x, y, CIRCLE_RADIUS, TFT_YELLOW); break;
  }
  delay(200);
}

// Малюємо курсор
void drawCursor() {
  static int lastX = -1, lastY = -1;
  if (lastX != cursorX || lastY != cursorY) {
    if (lastX != -1) {
      int oldX = START_X - CIRCLE_RADIUS + CIRCLE_X_SPACING * lastX;
      int oldY = START_Y + CIRCLE_Y_SPACING * lastY;
      tft.drawRect(oldX - 3, oldY - CIRCLE_RADIUS - 3, 27, 27, TFT_CYAN);
      tft.drawRect(oldX - 2, oldY - CIRCLE_RADIUS - 2, 25, 25, TFT_CYAN);
    }
    int x = START_X - CIRCLE_RADIUS + CIRCLE_X_SPACING * cursorX;
    int y = START_Y + CIRCLE_Y_SPACING * cursorY;
    tft.drawRect(x - 3, y - CIRCLE_RADIUS - 3, 27, 27, TFT_NAVY);
    tft.drawRect(x - 2, y - CIRCLE_RADIUS - 2, 25, 25, TFT_NAVY);
    lastX = cursorX;
    lastY = cursorY;
  }
}