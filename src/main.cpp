#include <Adafruit_GFX.h>
#include <Arduino.h> // Обов’язковий заголовок для PlatformIO
#include <MCUFRIEND_kbv.h>


// Визначення пінів. Окрім вказаних нижче, підключена бібліотека використовує цифрові піни вді 2 до 9
#define LCD_RST A4 // Reset
#define LCD_CS A3  // Chip Select
#define LCD_RS A2  // Register Select (Data/Command)
#define LCD_WR A1  // Write
#define LCD_RD A0  // Read

#define Green_Down_pin 22 //  analog 1
#define Blue_Left_pin 25  // digital 9
#define Yellow_Up_pin 23  // digital 8
#define Red_Right_pin 24  // digital 12
#define Y_axis_pin A14    //  analog 2
#define X_axis_pin A15    //  analog 3
#define Stick_pin 21      //    analog 0

int arrey_colors[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Ініціалізація об’єкта TFT
MCUFRIEND_kbv tft;

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
  uint16_t ID = tft.readID(); // Читаємо ID контролера, визначився як ID = 0x9341
  // if (ID == 0x0000 || ID == 0xFFFF) {
  //     ID = 0x9341; // Якщо ID не визначився, пробуємо ILI9341 як запасний
  //     варіант
  // }
  tft.begin(ID); // Запускаємо екран із визначеним ID
  for (int i = 0; i <= 4; i++) {
    int a = random(1, 5);
  
    if (a == 1) {
     arrey_colors[0] = 255;
     arrey_colors[1] = 0;
     arrey_colors[2] = 0;
    } 
    else if (a == 2) {
     arrey_colors[3] = 0;
     arrey_colors[4] = 255;
     arrey_colors[5] = 0;
    }
    else if (a == 3) {
     arrey_colors[6] = 0;
     arrey_colors[7] = 0;
     arrey_colors[8] = 255;
    }
    else if (a == 4) {
     arrey_colors[9] = 255;
     arrey_colors[10] = 191;
     arrey_colors[11] = 0;
    }
    }
  
    for (int i = 0; i <= 12; i++) {
    Serial.print(arrey_colors[i]);
    } 
  
}

void loop() {
  int Blue_Left_state = digitalRead(Blue_Left_pin);
  int Red_Right_state = digitalRead(Red_Right_pin);
  int Yellow_Up_state = digitalRead(Yellow_Up_pin);
  int Green_Down_state = digitalRead(Green_Down_pin);

  int Stick_state = digitalRead(Stick_pin);

  int X_value = analogRead(X_axis_pin);
  int Y_value = analogRead(Y_axis_pin);
  
  delay(10000);
}