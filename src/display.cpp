// display.cpp - drawing functions for the ST7789 TFT screen
//
// Every draw function draws into "canvas" (a buffer in RAM).
// displayUpdate() copies the whole canvas to the TFT in one go.

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "display.h"
#include "config.h"

// Menu layout
#define MENU_TOP        40
#define MENU_SPACING    36
#define MENU_ITEM_H     30

static Adafruit_ST7789 tft = Adafruit_ST7789(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

// Hidden copy of the screen (320 x 240 x 2 bytes = about 150 kB of RAM)
static GFXcanvas16* canvas = NULL;

// True when the canvas has changed and still needs sending to the TFT
static bool needsUpdate = false;

bool displayInit()
{
    // SCLK, MISO (not used), MOSI, CS
    SPI.begin(PIN_TFT_SCLK, -1, PIN_TFT_MOSI, PIN_TFT_CS);

    tft.init(TFT_WIDTH, TFT_HEIGHT, TFT_SPI_MODE);
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(COLOR_BLACK);

    canvas = new GFXcanvas16(SCREEN_W, SCREEN_H);

    // new returns a canvas even when the buffer inside failed to allocate
    if (canvas == NULL || canvas->getBuffer() == NULL)
    {
        return false;
    }

    canvas->fillScreen(COLOR_BLACK);
    needsUpdate = true;
    return true;
}

void displayUpdate()
{
    if (!needsUpdate)
    {
        return;
    }

    tft.drawRGBBitmap(0, 0, canvas->getBuffer(), SCREEN_W, SCREEN_H);
    needsUpdate = false;
}

void displayClear()
{
    canvas->fillScreen(COLOR_BLACK);
    needsUpdate = true;
}

void displayDrawCentered(const char* text, int size, int y, uint16_t color)
{
    // Each character is 6 pixels wide at size 1
    int textWidth = strlen(text) * 6 * size;
    int x = (SCREEN_W - textWidth) / 2;

    if (x < 0)
    {
        x = 0;
    }

    canvas->setTextSize(size);
    canvas->setTextColor(color);
    canvas->setCursor(x, y);
    canvas->print(text);
    needsUpdate = true;
}

void displayDrawValue(const char* text, int size, int y, uint16_t color)
{
    // Each character is 8 pixels tall at size 1
    canvas->fillRect(0, y, SCREEN_W, 8 * size, COLOR_BLACK);
    displayDrawCentered(text, size, y, color);
}

void displayDrawTitle(const char* title)
{
    displayDrawCentered(title, 2, 8, COLOR_CYAN);
    canvas->drawFastHLine(10, 30, SCREEN_W - 20, COLOR_WHITE);
    needsUpdate = true;
}

void displayDrawFooter(const char* text)
{
    canvas->fillRect(0, SCREEN_H - 16, SCREEN_W, 16, COLOR_BLACK);
    displayDrawCentered(text, 1, SCREEN_H - 12, COLOR_WHITE);
}

void displayDrawMenuItem(int slot, const char* label, bool selected)
{
    int y = MENU_TOP + (slot * MENU_SPACING);
    uint16_t background = COLOR_BLACK;

    if (selected)
    {
        background = COLOR_BLUE;
    }

    canvas->fillRect(10, y, SCREEN_W - 20, MENU_ITEM_H, background);
    canvas->setTextSize(2);
    canvas->setTextColor(COLOR_WHITE);
    canvas->setCursor(20, y + 7);
    canvas->print(label);
    needsUpdate = true;
}

void displayShowMessage(const char* line1, const char* line2)
{
    displayClear();
    displayDrawCentered(line1, 2, 90, COLOR_RED);
    displayDrawCentered(line2, 1, 120, COLOR_WHITE);
}
