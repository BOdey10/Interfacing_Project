// joystick.cpp - KY023 joystick (two analog axes + push button)

#include "joystick.h"
#include "config.h"

// Button debounce variables
static bool buttonState = HIGH;
static bool lastReading = HIGH;
static unsigned long lastDebounceTime = 0;

// Time of the last joystick move (used to slow down repeats)
static unsigned long lastMoveTime = 0;

// Returns true once each time the button is pressed
static bool buttonWasPressed()
{
    bool pressed = false;
    bool reading = digitalRead(PIN_JOY_SW);

    if (reading != lastReading)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > BUTTON_DEBOUNCE_MS)
    {
        if (reading != buttonState)
        {
            buttonState = reading;

            if (buttonState == LOW)
            {
                pressed = true;
            }
        }
    }

    lastReading = reading;
    return pressed;
}

// Returns up, down, left, right or none
static JoyEvent readDirection()
{
    JoyEvent event = JOY_NONE;

    // Only allow a new move every JOY_REPEAT_MS
    if ((millis() - lastMoveTime) < JOY_REPEAT_MS)
    {
        return JOY_NONE;
    }

    int x = analogRead(PIN_JOY_X);
    int y = analogRead(PIN_JOY_Y);

    if (y < JOY_LOW)
    {
        event = JOY_INVERT_Y ? JOY_DOWN : JOY_UP;
    }
    else if (y > JOY_HIGH)
    {
        event = JOY_INVERT_Y ? JOY_UP : JOY_DOWN;
    }
    else if (x < JOY_LOW)
    {
        event = JOY_INVERT_X ? JOY_RIGHT : JOY_LEFT;
    }
    else if (x > JOY_HIGH)
    {
        event = JOY_INVERT_X ? JOY_LEFT : JOY_RIGHT;
    }

    if (event != JOY_NONE)
    {
        lastMoveTime = millis();
    }

    return event;
}

void joystickInit()
{
    pinMode(PIN_JOY_SW, INPUT_PULLUP);

    analogReadResolution(12);
    analogSetPinAttenuation(PIN_JOY_X, ADC_11db);
    analogSetPinAttenuation(PIN_JOY_Y, ADC_11db);
}

JoyEvent joystickRead()
{
    if (buttonWasPressed())
    {
        return JOY_PRESS;
    }

    return readDirection();
}
