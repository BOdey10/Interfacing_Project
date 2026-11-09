// joystick.h - KY023 joystick (two analog axes + push button)

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>

// Things the joystick can do
typedef enum
{
    JOY_NONE,
    JOY_UP,
    JOY_DOWN,
    JOY_LEFT,
    JOY_RIGHT,
    JOY_PRESS
} JoyEvent;

// Set up the button pin and the ADC
void joystickInit();

// Check the joystick. Does not wait, so call it every loop.
// Returns JOY_NONE if nothing happened.
JoyEvent joystickRead();

#endif
