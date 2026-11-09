// pages.h - what each screen page looks like

#ifndef PAGES_H
#define PAGES_H

#include <Arduino.h>
#include "sensor.h"

// The pages in the same order as the menu
typedef enum
{
    PAGE_PRESSURE_HPA,
    PAGE_PRESSURE_MBAR,
    PAGE_TEMPERATURE,
    PAGE_ELEVATION,
    PAGE_SEA_LEVEL,
    PAGE_COUNT              // number of pages (keep this last)
} Page;

// Name of a page (used as the menu text and the page title)
const char* pageGetName(int page);

// Draw the parts of a page that do not change (title, units, footer)
void pageDrawStatic(int page);

// Draw the values that change. Safe to call again and again.
void pageDrawValues(int page, SensorData data, float seaLevel);

#endif
