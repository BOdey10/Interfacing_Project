// altitude.h - elevation from air pressure

#ifndef ALTITUDE_H
#define ALTITUDE_H

// Barometric formula:  h = 44330 * (1 - (P / P0) ^ (1 / 5.255))
//   pressure  = pressure measured by the sensor (hPa)
//   seaLevel  = sea level reference pressure P0 (hPa)
// Returns the elevation above sea level in metres.
float altitudeCalculate(float pressure, float seaLevel);

#endif
