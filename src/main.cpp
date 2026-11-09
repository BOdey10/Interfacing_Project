#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "joystick.h"
#include "sensor.h"
#include "ui.h"

void setup()
{
    Serial.begin(SERIAL_BAUD);
    delay(500);
    Serial.println("Weather Station starting...");

    bool sensorOk = sensorInit();
    Serial.println(sensorOk ? "BMP280 OK" : "BMP280 NOT FOUND");

    displayInit();
    joystickInit();

    if (!sensorOk) {
        displayShowMessage("BMP280 not found", "Check I2C wiring (SDA=GPIO8, SCL=GPIO9)");
        delay(2000);
    }

    uiInit();
    Serial.println("Ready");
}

void loop()
{
    bool     fresh = sensorUpdate();
    JoyEvent ev    = joystickPoll();

    uiUpdate(ev, fresh);
}