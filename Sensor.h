#ifndef SENSOR_H
#define SENSOR_H

#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <atomic>

class Sensor
{
private:
    float last_measurement = 0.0f;
    std::atomic<bool> running{false};
    std::thread measure_thread;
    std::mutex sensor_mutex;

    void measurement_loop(float min_value, float max_value);

public:
    Sensor() : last_measurement(0.0f), running(false) {}
    virtual ~Sensor();

    virtual void start(float min_value, float max_value);
    virtual float poll();
    virtual void stop();
    virtual void operator()();
};
#endif // SENSOR_H