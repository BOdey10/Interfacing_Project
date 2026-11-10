// altitude.cpp - elevation from air pressure

#include <math.h>
#include "altitude.h"

float altitudeCalculate(float pressure, float seaLevel)
{
    float ratio = pressure / seaLevel;

    return 44330.0f * (1.0f - powf(ratio, 1.0f / 5.255f));
}
