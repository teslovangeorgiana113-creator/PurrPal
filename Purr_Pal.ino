
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

//Animatia ochilor
unsigned long previousBlinkMillis = 0;

const unsigned long BLINK_INTERVAL = 3000;
const unsigned long BLINK_DURATION = 150;

bool eyesClosed = false;

// Animatia coditei
unsigned long previousTailMillis = 0;

const unsigned long TAIL_INTERVAL = 350;

int tailFrame = 0;//vom avea 3 cadre

//Stare somnic sau fresh ca Puya
enum CatState {
  AWAKE,
  SLEEPING
};

CatState catState = AWAKE;

unsigned long previousStateMillis = 0;

const unsigned long AWAKE_DURATION = 10000;
const unsigned long SLEEP_DURATION = 6000;

void drawEyes(bool closed) {
  if (closed) {
    // Ochii inchisi: doua linii orizontale
    display.drawLine(52, 30, 58, 30, SSD1306_BLACK);
    display.drawLine(71, 30, 77, 30, SSD1306_BLACK);
  } else {
    // Ochii deschisi: doua cercuri negre
    display.fillCircle(55, 30, 2, SSD1306_BLACK);
    display.fillCircle(74, 30, 2, SSD1306_BLACK);
  }
}



void drawTail() {

  if (tailFrame == 0) {
    // Codita inclinata usor spre stanga
    display.drawLine(82, 49, 94, 49, SSD1306_WHITE);
    display.drawLine(94, 49, 98, 43, SSD1306_WHITE);
    display.drawLine(98, 43, 95, 38, SSD1306_WHITE);

    display.drawLine(82, 50, 94, 50, SSD1306_WHITE);
    display.drawLine(94, 50, 99, 44, SSD1306_WHITE);
    display.drawLine(99, 44, 96, 38, SSD1306_WHITE);

  } else if (tailFrame == 1) {
    // Codita in pozitia de mijloc
    display.drawLine(82, 49, 95, 49, SSD1306_WHITE);
    display.drawLine(95, 49, 100, 43, SSD1306_WHITE);
    display.drawLine(100, 43, 98, 38, SSD1306_WHITE);

    display.drawLine(82, 50, 95, 50, SSD1306_WHITE);
    display.drawLine(95, 50, 101, 44, SSD1306_WHITE);
    display.drawLine(101, 44, 99, 38, SSD1306_WHITE);

  } else {
    // Codita inclinata usor spre dreapta
    display.drawLine(82, 49, 97, 49, SSD1306_WHITE);
    display.drawLine(97, 49, 103, 44, SSD1306_WHITE);
    display.drawLine(103, 44, 103, 39, SSD1306_WHITE);

    display.drawLine(82, 50, 97, 50, SSD1306_WHITE);
    display.drawLine(97, 50, 104, 45, SSD1306_WHITE);
    display.drawLine(104, 45, 104, 39, SSD1306_WHITE);
  }
}

void drawMouth() {
  if (catState == SLEEPING) {
    // Gurita relaxata, in timpul somnului
    display.drawLine(62, 43, 66, 43, SSD1306_BLACK);
  } else {
    // Un zambet mic, in forma de W
    display.drawLine(60, 42, 62, 44, SSD1306_BLACK);
    display.drawLine(62, 44, 64, 42, SSD1306_BLACK);
    display.drawLine(64, 42, 66, 44, SSD1306_BLACK);
    display.drawLine(66, 44, 68, 42, SSD1306_BLACK);
  }
}

// Deseneaza pisicuta in pozitie sezand
void drawCat() {

  
  // 1. Codita
  drawTail();

  // 2. Corpul
  display.fillRoundRect(48, 37, 35, 23, 11, SSD1306_WHITE);

  // 3. Labutele
  display.fillRoundRect(45, 52, 16, 9, 4, SSD1306_WHITE);
  display.fillRoundRect(69, 52, 16, 9, 4, SSD1306_WHITE);

  // Separatia dintre labute
  display.drawLine(65, 54, 65, 60, SSD1306_BLACK);

  // 4. Urechile
  display.fillTriangle(
    43, 20,
    46, 5,
    58, 18,
    SSD1306_WHITE
  );

  display.fillTriangle(
    71, 18,
    83, 5,
    86, 20,
    SSD1306_WHITE
  );

  // 5. Capul
 display.fillRoundRect(41, 17, 47, 31, 15, SSD1306_WHITE);

 //6. Gâtul = mici linii care separa capul de corp
display.drawLine(46, 46, 50, 49, SSD1306_BLACK);
display.drawLine(83, 46, 79, 49, SSD1306_BLACK);

  // 7. Ochii
  drawEyes(catState == SLEEPING || eyesClosed);

  // 8. Nasul
  display.fillTriangle(
    62, 37,
    67, 37,
    64, 40,
    SSD1306_BLACK
  );

  // Gurita pisicutei
  drawMouth();
  
  // 9. Mustatile
  display.drawLine(49, 38, 37, 35, SSD1306_WHITE);
  display.drawLine(49, 41, 37, 43, SSD1306_WHITE);

  display.drawLine(80, 38, 92, 35, SSD1306_WHITE);
  display.drawLine(80, 41, 92, 43, SSD1306_WHITE);

  if (catState == SLEEPING) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(96, 12);
  display.print("Zzz");
}

}

void updateBlink() {
  unsigned long currentMillis = millis();

  //daca ochii sunt deschisi si au trecut 3 secunde, ii inchidem
  if (!eyesClosed &&
      currentMillis - previousBlinkMillis >= BLINK_INTERVAL) {

    eyesClosed = true;
    previousBlinkMillis = currentMillis;
  //daca ochii sunt inchisi si au trecut 150 ms ii deschidem
  } else if (eyesClosed &&
         currentMillis - previousBlinkMillis >= BLINK_DURATION) {

    eyesClosed = false;
    previousBlinkMillis = currentMillis;
  }
}

void updateTail() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousTailMillis >= TAIL_INTERVAL) {
    previousTailMillis = currentMillis;

    static int direction = 1;//vrem sa isi pastreze valoare intre apelurile de functie
                             //memoreaza daca ne deplasam spre dreapta (1) sau spre stanga (-1)

    tailFrame += direction;

    if (tailFrame == 2 || tailFrame == 0) {//codita se leagana de la stanga la dreapta
      direction = -direction;
    }
  }
}


void updateCatState() {
  unsigned long currentMillis = millis();

  if (catState == AWAKE) {
    if (currentMillis - previousStateMillis >= AWAKE_DURATION) {
      catState = SLEEPING;
      previousStateMillis = currentMillis;
    }
  } else if (catState == SLEEPING) {
    if (currentMillis - previousStateMillis >= SLEEP_DURATION) {
      catState = AWAKE;
      previousStateMillis = currentMillis;
    }
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED indisponibil!");
    while (true);
  }

  display.clearDisplay();

  drawCat();

  display.display();
}

void loop() {
  updateCatState();

  if (catState == AWAKE) {
    updateBlink();
    updateTail();
  }

  display.clearDisplay();
  drawCat();
  display.display();

}
