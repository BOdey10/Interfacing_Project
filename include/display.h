// display.h - drawing functions for the ST7789 TFT screen
//
// All drawing is done into a memory buffer (a hidden copy of the screen).
// Nothing appears on the TFT until displayUpdate() sends the finished
// picture to the screen in one go. This stops flickering.

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

// Colours (RGB565)
#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_BLUE      0x001F
#define COLOR_RED       0xF800
#define COLOR_CYAN      0x07FF
#define COLOR_YELLOW    0xFFE0

// Start the SPI bus, the display and the memory buffer.
// Returns false if there is not enough memory for the buffer.
bool displayInit();

// Send the buffer to the screen. Does nothing if nothing was drawn
// since the last update, so it is safe to call every loop.
void displayUpdate();

// Fill the whole buffer with black
void displayClear();

// Draw a title at the top of the screen with a line under it
void displayDrawTitle(const char* title);

// Draw a small line of text at the bottom of the screen
void displayDrawFooter(const char* text);

// Draw one menu row (slot 0 is the top row)
void displayDrawMenuItem(int slot, const char* label, bool selected);

// Draw text centred on the screen at height y
void displayDrawCentered(const char* text, int size, int y, uint16_t color);

// Same as displayDrawCentered but wipes the old text first (for changing values)
void displayDrawValue(const char* text, int size, int y, uint16_t color);

// Show a two line message (used for errors)
void displayShowMessage(const char* line1, const char* line2);

#endif
