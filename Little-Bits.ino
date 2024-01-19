//The Game of Life, also known simply as Life, is a cellular automaton
//devised by the British mathematician John Horton Conway in 1970.
// https://en.wikipedia.org/wiki/Conway's_Game_of_Life

#include <M5Unified.h>
#include "Logo.c"

//ULTLORES
//        #define GRIDX 30
//        #define GRIDY 20
//        #define CELLXY 4

//LORES
//#define GRIDX = 60
// #define GRIDY = 40
// #define CELLXY = 2
int viewx = 180;
int viewy = 135;
//HIRES
#define GRIDX 180
#define GRIDY 135
#define CELLXY 1

#define TFT_GREY 0x1010
#define TFT_ORANGE 0xFFA5
int runt = 0;
int res = 1;

int rcolor = random(0xFFFF);

//Number of generations
#define NUMGEN 999
int gens = NUMGEN;
uint16_t genCount = 0;

void setup() {
  //Set up the display
  M5.begin();
  M5.Lcd.setRotation(2);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.drawBitmap(27, 40, 80, 160, (uint16_t *)gImage_002);
  delay(2000);
}
//Current grid
uint8_t grid[GRIDX][GRIDY];

//The new grid for the next generation
uint8_t newgrid[GRIDX][GRIDY];

void loop() {
  int GEN_DELAY = analogRead(G0) / 400;
  initGrid();

  genCount = NUMGEN;

  drawGrid();

  //Compute generations
  for (int gen = 0; gen < gens; gen++) {
    M5.Lcd.setTextColor(TFT_WHITE, BLACK);
    M5.Lcd.setCursor(185, 5);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println("GEN");
    M5.Lcd.setCursor(185, 20);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println(gen);
    M5.Lcd.setCursor(185, 45);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println("RUN");
    M5.Lcd.setCursor(185, 60);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println(runt);
    int vol = M5.Power.getBatteryLevel();
    M5.Lcd.setCursor(185, 85);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println("BAT");
    M5.Lcd.setCursor(185, 100);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println(vol);
    M5.Lcd.display();
    delay(GEN_DELAY);

    computeCA();
    drawGrid();

    for (int16_t x = 1; x < viewx - 1; x++) {
      for (int16_t y = 1; y < viewy - 1; y++) {
        grid[x][y] = newgrid[x][y];
      }
    }

    if (M5.BtnA.wasClicked()) {
      gen = 0;
      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(TFT_WHITE, RED);
      M5.Lcd.setCursor(35, 27);
      M5.Lcd.println(F("  NUKE  "));
      delay(200);
      rcolor = random(0xFFFF);
      runt--;
      initGrid();
      int GEN_DELAY = 20;
    }
    M5.update();
  }
}

//Draws the grid on the display
void drawGrid(void) {

  int color = 0x0000;
  for (int16_t x = 1; x < 180 - 1; x++) {
    for (int16_t y = 1; y < 135 - 1; y++) {
      if ((grid[x][y]) != (newgrid[x][y])) {
        if (newgrid[x][y] == 1) color = rcolor;  //random(0xFFFF);
        else color = 0;
        M5.Lcd.fillRect(res * x, res * y, res, res, color);
      }
    }
  }
}

//Initialise Grid
void initGrid(void) {
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawRect(0, 0, 180, 135, random(0xFFFF));
  for (int16_t x = 0; x < viewx; x++) {
    for (int16_t y = 0; y < viewy; y++) {
      newgrid[x][y] = 0;

      if (x == 0 || x == viewx - 1 || y == 0 || y == viewy - 1) {
        grid[x][y] = 0;
      } else {
        if (random(3) == 1)
          grid[x][y] = 1;
        else
          grid[x][y] = 0;
      }
    }
  }
  runt++;
}

//Compute the CA. Basically everything related to CA starts here
void computeCA() {
  for (int16_t x = 1; x < viewx; x++) {
    for (int16_t y = 1; y < viewy; y++) {
      int neighbors = getNumberOfNeighbors(x, y);
      if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3)) {
        newgrid[x][y] = 1;
      } else if (grid[x][y] == 1) newgrid[x][y] = 0;
      if (grid[x][y] == 0 && (neighbors == 3)) {
        newgrid[x][y] = 1;
      } else if (grid[x][y] == 0) newgrid[x][y] = 0;
    }
  }
}

// Check the Moore neighborhood
int getNumberOfNeighbors(int x, int y) {
  return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] + grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] + grid[x][y + 1] + grid[x - 1][y + 1];
}