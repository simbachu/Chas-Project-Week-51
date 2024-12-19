#include "Sensor.h"
#include <iostream>

void Sensor::measurement_loop(float min_value, float max_value)
{
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dis(min_value, max_value);

    while (running)
    {
        {
            std::lock_guard<std::mutex> lock(sensor_mutex);
            last_measurement = dis(gen);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

Sensor::~Sensor()
{
    stop();
}

void Sensor::start(float min_value, float max_value)
{
    running = true;
    measure_thread = std::thread(&Sensor::measurement_loop, this, min_value, max_value);
}

float Sensor::poll()
{
    std::lock_guard<std::mutex> lock(sensor_mutex);
    return last_measurement;
}

void Sensor::stop()
{
    running = false;
    if (measure_thread.joinable())
    {
        measure_thread.join();
    }
}

void Sensor::operator()()
{
    // Default functor implementation
}