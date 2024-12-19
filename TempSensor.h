#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#include "Sensor.h"

class TempSensor : public Sensor
{
public:
    void start()
    {
        Sensor::start(15.0f, 30.0f);
    }

    void operator()()
    {
        start();
    }
};

#endif // TEMPSENSOR_H