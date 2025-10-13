#include "light_sensor.h"

float getPhotoResistance(int voltage, int Rref)
{
    return (float)Rref * voltage / (3300 - voltage);
}
