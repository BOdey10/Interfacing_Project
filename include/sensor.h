// sensor.h - BMP280 pressure and temperature sensor (I2C)

#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

// One set of readings
typedef struct
{
    float pressure;       // hPa
    float temperature;    // degrees C
    bool valid;           // false if the reading is not usable
} SensorData;

// Start I2C and the sensor. Returns false if the sensor is not found.
bool sensorInit();

// Reads the sensor every SENSOR_PERIOD_MS. Does not wait, so call it every loop.
// Returns true when a new reading is ready.
bool sensorUpdate();

// Get the latest reading
SensorData sensorGetData();

#endif
