#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// ---------------- TFT pins (SPI) ----------------
// These must be DEFINED, otherwise SPI.begin() below fails to compile.
// Change them to match your actual wiring.
#define TFT_CS   4
#define TFT_DC   5
#define TFT_RST  6
#define TFT_MOSI 11   // module label: SDA
#define TFT_SCLK 12   // module label: SCL

// ---------------- Joystick pins ----------------
// Must NOT overlap with the TFT pins above.
#define JOYSTICK_X  1   // VRX
#define JOYSTICK_Y  2   // VRY
#define JOYSTICK_SW 7   // SW

// Create display object (hardware SPI)
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Menu options
const char* menuItems[] = {
    "Pressure (hPa)",
    "Pressure (mbar)",
    "Temperature (C)"
};

const int menuSize = 3;
int selectedItem = 0;

// Screen state
bool onMenu = true;

// Joystick thresholds (12-bit ADC: 0-4095)
const int UP_THRESHOLD = 1000;
const int DOWN_THRESHOLD = 3000;

// Button state
bool lastButtonState = HIGH;

// Timing
unsigned long lastMoveTime = 0;
unsigned long lastButtonTime = 0;

const unsigned long MOVE_DELAY = 250;
const unsigned long BUTTON_DELAY = 250;


// Draw main menu (layout fits a 320x240 landscape screen)
void drawMenu()
{
    tft.fillScreen(ST77XX_BLACK);

    tft.setTextSize(2);
    tft.setTextColor(ST77XX_CYAN);
    tft.setCursor(70, 10);
    tft.println("WEATHER STATION");

    for (int i = 0; i < menuSize; i++)
    {
        int y = 55 + (i * 50);

        if (i == selectedItem)
        {
            tft.fillRect(15, y, 290, 40, ST77XX_BLUE);
        }

        tft.setTextColor(ST77XX_WHITE);
        tft.setCursor(25, y + 12);
        tft.println(menuItems[i]);
    }

    tft.setTextSize(1);
    tft.setCursor(85, 225);
    tft.println("Move joystick to navigate");
}


// Draw selected measurement page
void drawMeasurementPage()
{
    tft.fillScreen(ST77XX_BLACK);

    tft.setTextColor(ST77XX_CYAN);
    tft.setTextSize(2);
    tft.setCursor(20, 20);
    tft.println(menuItems[selectedItem]);

    tft.drawFastHLine(15, 50, 290, ST77XX_WHITE);

    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(3);
    tft.setCursor(35, 100);

    // Temporary example values
    if (selectedItem == 0)      tft.println("1013 hPa");
    else if (selectedItem == 1) tft.println("1013 mbar");
    else if (selectedItem == 2) tft.println("22.5 C");

    tft.setTextSize(1);
    tft.setCursor(85, 200);
    tft.println("Press to return to menu");
}


// Read joystick movement
void readJoystick()
{
    int yValue = analogRead(JOYSTICK_Y);

    if (millis() - lastMoveTime >= MOVE_DELAY)
    {
        if (yValue < UP_THRESHOLD)
        {
            selectedItem--;
            if (selectedItem < 0) selectedItem = menuSize - 1;
            drawMenu();
            lastMoveTime = millis();
        }
        else if (yValue > DOWN_THRESHOLD)
        {
            selectedItem++;
            if (selectedItem >= menuSize) selectedItem = 0;
            drawMenu();
            lastMoveTime = millis();
        }
    }
}


// Read joystick button
void readButton()
{
    bool buttonState = digitalRead(JOYSTICK_SW);

    if (buttonState == LOW &&
        lastButtonState == HIGH &&
        millis() - lastButtonTime >= BUTTON_DELAY)
    {
        if (onMenu)
        {
            onMenu = false;
            drawMeasurementPage();
        }
        else
        {
            onMenu = true;
            drawMenu();
        }

        lastButtonTime = millis();
    }

    lastButtonState = buttonState;
}


void setup()
{
    Serial.begin(115200);
    delay(2000);

    pinMode(JOYSTICK_SW, INPUT_PULLUP);

    // SCLK, MISO (unused), MOSI, SS
    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

    tft.init(240, 320);
    tft.setRotation(1);   // landscape: 320 wide x 240 tall

    drawMenu();

    Serial.println("Weather Station Started");
}


void loop()
{
    if (onMenu)
    {
        readJoystick();
    }

    readButton();
}