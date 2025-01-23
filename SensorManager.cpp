/**
 * @file SensorManager.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <sstream>
#include "SensorManager.h"


/**
 * @brief Operator overload for stream output ( os << wr )
 * @param os The output stream to write to (left hand side of <<)
 * @param wr The weather report to print (right hand side of <<)
 * @return std::ostream& The result of concatenating os << wr
 */
std::ostream& operator << (std::ostream& os, WeatherReport& wr){
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss ("\n"); //Buffer stream to not get interrupted in another thread
    std::mutex ctg;
    {
        //ctime is not thread safe. We need to guard the location we are reading
        std::lock_guard<std::mutex> lock(ctg);
        ss << std::ctime(&now);
    }
    ss          << "Temperature: " << wr.temperature << " C"
                << "\nHumidity: " << wr.humidity << " %"
                << "\nPressure: " << wr.pressure << " hpa\n";
    os << ss.str(); //Move the contents of buffer to our output stream
    return os;
}

SensorManager::SensorManager()
{
    temp_sensor = std::make_unique<TempSensor>();
    humidity_sensor = std::make_unique<HumiditySensor>();
    pressure_sensor = std::make_unique<PressureSensor>();

    sensor_threads.push_back(std::thread(std::ref(*humidity_sensor)));
    sensor_threads.push_back(std::thread(std::ref(*pressure_sensor)));
    sensor_threads.push_back(std::thread(std::ref(*temp_sensor)));
}

/**
 * @brief Destroy the Sensor Manager:: Sensor Manager object
 * Joining the threads in the destructor like a jthread
 */
SensorManager::~SensorManager()
{
    temp_sensor->stop();
    humidity_sensor->stop();
    pressure_sensor->stop();
    for (auto &thread : sensor_threads)
    {
        thread.join();
    }
}

/**
 * @brief Callable overload for SensorManager
 * Functor for std::thread call
 * @param out Destination vector
 * @param lock_out Lock for the destination vector
 */
void SensorManager::operator()(std::vector<WeatherReport> *out, std::mutex *lock_out)
{
    report(out, lock_out);
}

/**
 * @brief Generate weather report from sensor data
 * @param out Destination vector for the WeatherReport
 * @param lock_out Lock for the destination vector
 */
void SensorManager::report(std::vector<WeatherReport> *out, std::mutex *lock_out) const
{
    while (1)
    {   std::cout << "\n---------SENSOR----------\n";
        std::lock_guard<std::mutex> output_lock(*lock_out);
        WeatherReport wr = {temp_sensor->poll(),
                            humidity_sensor->poll(),
                            pressure_sensor->poll()};
        out->emplace_back(wr);

        std::cout << wr;

        std::this_thread::sleep_for(report_rate);
    }
}