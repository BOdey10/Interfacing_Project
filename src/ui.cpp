// ui.cpp - menu and page navigation

#include <math.h>
#include "ui.h"
#include "config.h"
#include "display.h"
#include "pages.h"
#include "sensor.h"

typedef enum
{
    STATE_MENU,
    STATE_PAGE
} UiState;

static UiState state = STATE_MENU;
static int selectedItem = 0;                    // highlighted menu row
static int currentPage = PAGE_PRESSURE_HPA;     // page being shown
static float seaLevel = SEA_LEVEL_DEFAULT;      // sea level reference (hPa)

// Draw one menu row
static void drawMenuRow(int row)
{
    displayDrawMenuItem(row, pageGetName(row), row == selectedItem);
}

// Draw the whole menu
static void drawMenu()
{
    displayClear();
    displayDrawTitle("WEATHER STATION");

    for (int i = 0; i < PAGE_COUNT; i++)
    {
        drawMenuRow(i);
    }

    displayDrawFooter("Up/Down: move   Press: open");
}

// Open the selected page
static void openPage(int page)
{
    currentPage = page;
    state = STATE_PAGE;

    displayClear();
    pageDrawStatic(currentPage);
    pageDrawValues(currentPage, sensorGetData(), seaLevel);
}

// Go back to the menu
static void closePage()
{
    state = STATE_MENU;
    drawMenu();
}

// Move the highlight up (-1) or down (+1), wrapping around
static void moveSelection(int step)
{
    int oldItem = selectedItem;

    selectedItem = selectedItem + step;

    if (selectedItem < 0)
    {
        selectedItem = PAGE_COUNT - 1;
    }
    else if (selectedItem >= PAGE_COUNT)
    {
        selectedItem = 0;
    }

    // Only redraw the two rows that changed (avoids flicker)
    drawMenuRow(oldItem);
    drawMenuRow(selectedItem);
}

// Change the sea level reference and keep it within limits
static void changeSeaLevel(float amount)
{
    seaLevel = seaLevel + amount;

    if (seaLevel < SEA_LEVEL_MIN)
    {
        seaLevel = SEA_LEVEL_MIN;
    }
    if (seaLevel > SEA_LEVEL_MAX)
    {
        seaLevel = SEA_LEVEL_MAX;
    }

    // Round to 2 decimal places to stop small float errors building up
    seaLevel = roundf(seaLevel * 100.0f) / 100.0f;

    pageDrawValues(currentPage, sensorGetData(), seaLevel);
}

// Joystick handling while the menu is showing
static void handleMenuEvent(JoyEvent event)
{
    if (event == JOY_UP)
    {
        moveSelection(-1);
    }
    else if (event == JOY_DOWN)
    {
        moveSelection(1);
    }
    else if (event == JOY_PRESS)
    {
        openPage(selectedItem);
    }
}

// Joystick handling while a page is showing
static void handlePageEvent(JoyEvent event)
{
    if (event == JOY_PRESS)
    {
        closePage();
        return;
    }

    // Only the sea level page uses the other directions
    if (currentPage != PAGE_SEA_LEVEL)
    {
        return;
    }

    if (event == JOY_UP)
    {
        changeSeaLevel(SEA_LEVEL_STEP_FINE);
    }
    else if (event == JOY_DOWN)
    {
        changeSeaLevel(-SEA_LEVEL_STEP_FINE);
    }
    else if (event == JOY_RIGHT)
    {
        changeSeaLevel(SEA_LEVEL_STEP_COARSE);
    }
    else if (event == JOY_LEFT)
    {
        changeSeaLevel(-SEA_LEVEL_STEP_COARSE);
    }
}

void uiInit()
{
    state = STATE_MENU;
    selectedItem = 0;
    drawMenu();
}

void uiUpdate(JoyEvent event, bool newData)
{
    if (state == STATE_MENU)
    {
        handleMenuEvent(event);
    }
    else
    {
        handlePageEvent(event);

        // Refresh the numbers when the sensor has a new reading
        if (newData && state == STATE_PAGE)
        {
            pageDrawValues(currentPage, sensorGetData(), seaLevel);
        }
    }
}
