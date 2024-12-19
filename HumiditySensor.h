#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

#include "Sensor.h"

class HumiditySensor : public Sensor
{
public:
    void start()
    {
        Sensor::start(40.0f, 60.0f);
    }

    void operator()()
    {
        start();
    }
};

#endif // HUMIDITYSENSOR_H