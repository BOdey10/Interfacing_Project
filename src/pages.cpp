// pages.cpp - what each screen page looks like

#include <stdio.h>
#include <string.h>
#include "pages.h"
#include "display.h"
#include "altitude.h"

static const char* pageNames[PAGE_COUNT] =
{
    "Pressure (hPa)",
    "Pressure (mbar)",
    "Temperature (C)",
    "Elevation (m)",
    "Sea Level Ref"
};

// Turn a number into text, or "--" if the reading is not valid
static void makeText(char* text, int size, float value, int decimals, bool valid)
{
    if (valid)
    {
        snprintf(text, size, "%.*f", decimals, value);
    }
    else
    {
        strcpy(text, "--");
    }
}

const char* pageGetName(int page)
{
    return pageNames[page];
}

void pageDrawStatic(int page)
{
    displayDrawTitle(pageGetName(page));

    switch (page)
    {
        case PAGE_PRESSURE_HPA:
            displayDrawCentered("hPa", 4, 140, COLOR_CYAN);
            displayDrawFooter("Press: back to menu");
            break;

        case PAGE_PRESSURE_MBAR:
            displayDrawCentered("mbar", 4, 150, COLOR_CYAN);
            displayDrawFooter("Press: back to menu");
            break;

        case PAGE_TEMPERATURE:
            displayDrawCentered("degrees Celsius", 2, 140, COLOR_CYAN);
            displayDrawFooter("Press: back to menu");
            break;

        case PAGE_ELEVATION:
            displayDrawCentered("metres above sea level", 2, 130, COLOR_CYAN);
            displayDrawFooter("Set reference in 'Sea Level Ref'. Press: menu");
            break;

        case PAGE_SEA_LEVEL:
            displayDrawCentered("hPa (sea level reference)", 2, 120, COLOR_CYAN);
            displayDrawFooter("Up/Down: +-0.1   Left/Right: +-1.0   Press: done");
            break;
    }
}

void pageDrawValues(int page, SensorData data, float seaLevel)
{
    char text[24];
    float altitude = altitudeCalculate(data.pressure, seaLevel);

    switch (page)
    {
        case PAGE_PRESSURE_HPA:
            makeText(text, sizeof(text), data.pressure, 2, data.valid);
            displayDrawValue(text, 5, 80, COLOR_WHITE);
            break;

        case PAGE_PRESSURE_MBAR:
            // 1 hPa = 1 mbar, shown in a large font
            makeText(text, sizeof(text), data.pressure, 1, data.valid);
            displayDrawValue(text, 6, 70, COLOR_WHITE);
            break;

        case PAGE_TEMPERATURE:
            makeText(text, sizeof(text), data.temperature, 1, data.valid);
            displayDrawValue(text, 5, 80, COLOR_WHITE);
            break;

        case PAGE_ELEVATION:
            makeText(text, sizeof(text), altitude, 1, data.valid);
            displayDrawValue(text, 5, 80, COLOR_WHITE);

            snprintf(text, sizeof(text), "Ref: %.2f hPa", seaLevel);
            displayDrawValue(text, 2, 170, COLOR_YELLOW);
            break;

        case PAGE_SEA_LEVEL:
            makeText(text, sizeof(text), seaLevel, 2, true);
            displayDrawValue(text, 5, 70, COLOR_WHITE);

            if (data.valid)
            {
                snprintf(text, sizeof(text), "Elevation: %.1f m", altitude);
            }
            else
            {
                strcpy(text, "Elevation: --");
            }
            displayDrawValue(text, 2, 170, COLOR_YELLOW);
            break;
    }
}
