#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include "Sensor.h"

class PressureSensor : public Sensor
{
public:
    void start()
    {
        Sensor::start(980.0f, 1020.0f);
    }

    void operator()()
    {
        start();
    }
};

#endif // PRESSURESENSOR_H