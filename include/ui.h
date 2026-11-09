// ui.h - menu and page navigation

#ifndef UI_H
#define UI_H

#include "joystick.h"

// Draw the main menu
void uiInit();

// Handle a joystick event and refresh the screen.
// newData is true when the sensor has a fresh reading.
void uiUpdate(JoyEvent event, bool newData);

#endif
