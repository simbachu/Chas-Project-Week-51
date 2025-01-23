/**
 * @file SensorManager.h SensorManager.cpp
 * @author simbachu 
 * @brief 
 * @version 0.1
 * @date 2024-12-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CHAS_SENSORMANAGER_H
#define CHAS_SENSORMANAGER_H

#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <mutex>
#include <thread>
#include "TempSensor.h"
#include "HumiditySensor.h"
#include "PressureSensor.h"
//#include "DisplayReport.h"

/*
Small proof of concept.

Main file holds a history vector which the SensorManager writes to, using data from Sensors.
Various sensors can be defined, and can use different source functions for the data.

*/


/**
 * @brief Struct for storing sensor data
 * Time is default constructed to the current system time unless otherwise defined
 */
struct WeatherReport
{
    float temperature;
    float humidity;
    float pressure;
    std::chrono::time_point<std::chrono::system_clock> time{std::chrono::system_clock::now()};
};

// class Sensor{
//     public:
//     virtual ~Sensor();
//     virtual void operator()() = 0;
//     virtual float poll() const = 0;
// };

// class TempSensor : public Sensor{
//     private:
//     std::mt19937 mt;
//     std::chrono::milliseconds rate {500};
//     float last_measurement;

//     public:
//     void operator()() override;
//     virtual float poll() const override;
// };

/**
 * @brief Class for collecting information from sensors and writing 
 * 
 */

class SensorManager
{
private:
    std::chrono::seconds report_rate{2};
    std::unique_ptr<Sensor> temp_sensor;
    std::unique_ptr<Sensor> humidity_sensor;
    std::unique_ptr<Sensor> pressure_sensor;
    std::vector<std::thread> sensor_threads;

public:
    SensorManager();
    ~SensorManager();
    void operator()(std::vector<WeatherReport> *out, std::mutex *lock_out);
    void report(std::vector<WeatherReport> *out, std::mutex *lock_out) const;
};

#endif