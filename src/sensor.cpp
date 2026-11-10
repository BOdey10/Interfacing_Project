// sensor.cpp - BMP280 pressure and temperature sensor (I2C)

#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "sensor.h"
#include "config.h"

static Adafruit_BMP280 bmp(&Wire);
static SensorData latestData = {0.0f, 0.0f, false};
static unsigned long lastReadTime = 0;

bool sensorInit()
{
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

    if (!bmp.begin(BMP280_ADDRESS_1) && !bmp.begin(BMP280_ADDRESS_2))
    {
        return false;
    }

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                    Adafruit_BMP280::SAMPLING_X2,     // temperature
                    Adafruit_BMP280::SAMPLING_X16,    // pressure
                    Adafruit_BMP280::FILTER_X16,
                    Adafruit_BMP280::STANDBY_MS_500);

    return true;
}

bool sensorUpdate()
{
    if ((millis() - lastReadTime) < SENSOR_PERIOD_MS)
    {
        return false;
    }
    lastReadTime = millis();

    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0f;    // convert Pa to hPa

    latestData.temperature = temperature;
    latestData.pressure = pressure;

    // Reject NaN and values that make no sense
    if (isnan(temperature) || isnan(pressure) || pressure < 300.0f || pressure > 1100.0f)
    {
        latestData.valid = false;
    }
    else
    {
        latestData.valid = true;
    }

    return true;
}

SensorData sensorGetData()
{
    return latestData;
}
