#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "joystick.h"
#include "sensor.h"
#include "ui.h"

void setup()
{
    // ---------- Serial (debugging) ----------
    Serial.begin(SERIAL_BAUD);
    delay(500);
    Serial.println("Weather Station starting...");

    // ---------- Sensor (I2C) ----------
    bool sensorFound = sensorInit();

    if (sensorFound)
    {
        Serial.println("BMP280 found");
    }
    else
    {
        Serial.println("BMP280 NOT found");
    }

    // Display
    // Stop here if the screen buffer could not be created
    if (!displayInit())
    {
        Serial.println("ERROR: not enough memory for the screen buffer");
        while (true)
        {
            delay(1000);
        }
    }

    //Joystick
    joystickInit();

    //if the sensor is missing 
    if (!sensorFound)
    {
        displayShowMessage("BMP280 not found", "Check I2C wiring (SDA = GPIO8, SCL = GPIO9)");
        displayUpdate();
        delay(2000);
    }

    //Meni
    uiInit();
    displayUpdate();

    Serial.println("Ready");
}

void loop()
{
    // 1. Read the sensor (true when there is a new reading)
    bool newData = sensorUpdate();

    // 2. Read the joystick (up, down, left, right, press or none)
    JoyEvent event = joystickRead();

    // 3. Update the menu and pages
    uiUpdate(event, newData);

    // 4. Send the finished picture to the screen (only if something changed)
    displayUpdate();
}