// app.cpp - starts up all the parts and runs the main loop

#include <Arduino.h>
#include "app.h"
#include "config.h"
#include "display.h"
#include "joystick.h"
#include "sensor.h"
#include "ui.h"

void appInit()
{
    Serial.begin(SERIAL_BAUD);
    delay(500);
    Serial.println("Weather Station starting...");

    bool sensorFound = sensorInit();

    if (sensorFound)
    {
        Serial.println("BMP280 found");
    }
    else
    {
        Serial.println("BMP280 NOT found");
    }

    displayInit();
    joystickInit();

    if (!sensorFound)
    {
        displayShowMessage("BMP280 not found", "Check I2C wiring (SDA = GPIO8, SCL = GPIO9)");
        delay(2000);
    }

    uiInit();
    Serial.println("Ready");
}

void appUpdate()
{
    bool newData = sensorUpdate();
    JoyEvent event = joystickRead();

    uiUpdate(event, newData);
}
