#include <Adafruit_GFX.h>
#include <Arduino.h> // Обов’язковий заголовок для PlatformIO
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

// Визначення пінів. Окрім вказаних нижче, підключена бібліотека використовує
// цифрові піни від 2 до 9
#define LCD_RST A4 // Reset
#define LCD_CS A3  // Chip Select
#define LCD_RS A2  // Register Select (Data/Command)
#define LCD_WR A1  // Write
#define LCD_RD A0  // Read

#define Green_Down_pin 22 //  analog  1
#define Blue_Left_pin 25  // digital  9
#define Yellow_Up_pin 23  // digital  8
#define Red_Right_pin 24  // digital 12
#define Y_axis_pin A14    //  analog  2
#define X_axis_pin A15    //  analog  3
#define Stick_pin 21      //  analog  0
#define R 10
#define Circle_Y_size 30
#define Circle_X_size 30
int Position_Y = 24;
int Position_X = 15;

int arrey_colors[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int you_arrey_colors[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// clang-format off
int Position_Circle[4][10] = {
  1, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0
};
// clang-format on

// Ініціалізація об’єкта TFT
MCUFRIEND_kbv tft;

int i = 0;
int j = 0;

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(0));

  pinMode(Green_Down_pin, INPUT);
  pinMode(Blue_Left_pin, INPUT);
  pinMode(Yellow_Up_pin, INPUT);
  pinMode(Red_Right_pin, INPUT);

  pinMode(Y_axis_pin, INPUT);
  pinMode(X_axis_pin, INPUT);
  pinMode(Stick_pin, INPUT);

  // Ініціалізація екрану
  uint16_t ID = tft.readID(); // визначився як ID = 0x9341
  tft.begin(ID); // Запускаємо екран із визначеним ID

  tft.fillScreen(TFT_CYAN);
  tft.setCursor(0, 1);
  tft.setTextSize(2);
  tft.setTextColor(TFT_NAVY);
  tft.println("          Mastermind");
  tft.println("          by YarOK1");

  // clang-format off
  for (int j = 0; j < 4; j++) {
    tft.fillCircle(Position_X + Circle_X_size * j, Position_Y, R, TFT_NAVY); // "закриті" кола паролю
    for (int i = 1; i < 10; i++) {
      tft.fillCircle(Position_X + Circle_X_size * j, Position_Y + Circle_Y_size * i, R, TFT_WHITE); // білі кола
      tft.fillRect(Position_X - 8 + (Circle_X_size - 6) * j + 116, Position_Y - 9 + Circle_Y_size * i, 19, 19, TFT_NAVY); // fill-квадрати праворуч для оцінки вірності
    }
  }
  // clang-format on
}

void loop() {
  for (int i = 0; i <= 11; i++) Serial.print(you_arrey_colors[i]);

  int Blue_Left_state = digitalRead(Blue_Left_pin);
  int Red_Right_state = digitalRead(Red_Right_pin);
  int Yellow_Up_state = digitalRead(Yellow_Up_pin);
  int Green_Down_state = digitalRead(Green_Down_pin);

  int Stick_state = digitalRead(Stick_pin);

  int X_value = analogRead(X_axis_pin);
  int Y_value = analogRead(Y_axis_pin);

    for (int i = 0; i <= 4; i++) {
    int a = random(1, 5);

    if (a == 1) {
      arrey_colors[0] = 255;
      arrey_colors[1] = 0;
      arrey_colors[2] = 0;
    } else if (a == 2) {
      arrey_colors[3] = 0;
      arrey_colors[4] = 255;
      arrey_colors[5] = 0;
    } else if (a == 3) {
      arrey_colors[6] = 0;
      arrey_colors[7] = 0;
      arrey_colors[8] = 255;
    } else if (a == 4) {
      arrey_colors[9] = 255;
      arrey_colors[10] = 191;
      arrey_colors[11] = 0;
    }
  }

  for (int i = 0; i <= 12; i++) {
    Serial.print(arrey_colors[i]);
  }
  
  tft.drawRect(5 + 30 * i, Circle_Y_size + 13, 21, 22, TFT_CYAN); // подвійний квадрат довкола білих кіл
  tft.drawRect(4 + 30 * i, Position_Y + 18, 23, 24, TFT_CYAN); // подвійний квадрат довкола білих кіл

    if (X_value < 410 and i < 3) {
    i++;
    }
    if (X_value > 600 and i > 0) {
    i--;
    }

  // clang-format off
  tft.drawRect(5 + 30 * i, Circle_Y_size + 13, 21, 22, TFT_NAVY); // подвійний квадрат довкола білих кіл
  tft.drawRect(4 + 30 * i, Position_Y + 18, 23, 24, TFT_NAVY); // подвійний квадрат довкола білих кіл
  // clang-format on
  
  if (Blue_Left_state == 0) {
  tft.fillCircle(Position_X + 30 * i, Position_Y + Circle_Y_size, R, TFT_BLUE);
  switch (i) {
  case 0:
  you_arrey_colors[0] = 0;
  you_arrey_colors[1] = 0;
  you_arrey_colors[2] = 255;
  break;
  case 1:
  you_arrey_colors[3] = 0;
  you_arrey_colors[4] = 0;
  you_arrey_colors[5] = 255;
  break;
  case 2:
  you_arrey_colors[6] = 0;
  you_arrey_colors[7] = 0;
  you_arrey_colors[8] = 255;
  break;
  case 3:
  you_arrey_colors[9] = 0;
  you_arrey_colors[10] = 0;
  you_arrey_colors[11] = 255;
  break;
  }
  }
  if (Green_Down_state == 0) {
  tft.fillCircle(Position_X + 30 * i, Position_Y + Circle_Y_size, R, TFT_GREEN);
  switch (i) {
    case 0:
    you_arrey_colors[0] = 0;
    you_arrey_colors[1] = 255;
    you_arrey_colors[2] = 0;
    break;
    case 1:
    you_arrey_colors[3] = 0;
    you_arrey_colors[4] = 255;
    you_arrey_colors[5] = 0;
    break;
    case 2:
    you_arrey_colors[6] = 0;
    you_arrey_colors[7] = 255;
    you_arrey_colors[8] = 0;
    break;
    case 3:
    you_arrey_colors[9] = 0;
    you_arrey_colors[10] = 255;
    you_arrey_colors[11] = 0;
    break;
    }
  }
  if (Red_Right_state == 0) {
  tft.fillCircle(Position_X + 30 * i, Position_Y + Circle_Y_size, R, TFT_RED);
  switch (i) {
    case 0:
    you_arrey_colors[0] = 255;
    you_arrey_colors[1] = 0;
    you_arrey_colors[2] = 0;
    break;
    case 1:
    you_arrey_colors[3] = 255;
    you_arrey_colors[4] = 0;
    you_arrey_colors[5] = 0;
    break;
    case 2:
    you_arrey_colors[6] = 255;
    you_arrey_colors[7] = 0;
    you_arrey_colors[8] = 0;
    break;
    case 3:
    you_arrey_colors[9] = 255;
    you_arrey_colors[10] = 0;
    you_arrey_colors[11] = 0;
    break;
    }
  }
  if (Yellow_Up_state == 0) {
  tft.fillCircle(Position_X + 30 * i, Position_Y + Circle_Y_size, R, TFT_YELLOW);
  switch (i) {
    case 0:
    you_arrey_colors[0] = 255;
    you_arrey_colors[1] = 191;
    you_arrey_colors[2] = 0;
    break;
    case 1:
    you_arrey_colors[3] = 255;
    you_arrey_colors[4] = 191;
    you_arrey_colors[5] = 0;
    break;
    case 2:
    you_arrey_colors[6] = 255;
    you_arrey_colors[7] = 191;
    you_arrey_colors[8] = 0;
    break;
    case 3:
    you_arrey_colors[9] = 255;
    you_arrey_colors[10] = 191;
    you_arrey_colors[11] = 0;
    break;
    }
  }
  delay(1000);
}
