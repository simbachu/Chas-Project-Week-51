#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <sstream>
#include "SensorManager.h"

std::ostream& operator << (std::ostream& os, WeatherReport& wr){
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss(std::ctime(&now));
    os << "\n" << ss.str() 
                << "Temperature: " << wr.temperature << " C"
                << "\nHumidity: " << wr.humidity << " %"
                << "\nPressure: " << wr.pressure << " hpa";
    return os;
}


/*
    Namespace for different source functions for the data.
    You could make one that consumes a list for instance.
*/

// namespace sensor_reading{
//     float random(float low, float high, std::mt19937 &gen){
//         std::uniform_real_distribution<float> interval (low, high);
//         return interval(gen);
//     }
// }

/*
    Virtual destructor for subclassing with smart pointers
*/

// Sensor::~Sensor(){

// }

/*
    Functor for threading
*/

// void TempSensor::operator()() {
//     std::mt19937 mt (std::random_device{}());
//     while(1){
//         last_measurement = sensor_reading::random(5.0f, 15.0f, mt);
//         std::cerr << std::this_thread::get_id() << " " << last_measurement << '\n';
//         std::this_thread::sleep_for(rate);
//     }
// }

/*
    For reading sensor value
*/

// float TempSensor::poll() const {
//     return last_measurement;
// }

/*
    Constructor is non-trivial and requires defining the sensors
    and the threads we are using for them
*/

SensorManager::SensorManager()
{
    temp_sensor = std::make_unique<TempSensor>();
    humidity_sensor = std::make_unique<HumiditySensor>();
    pressure_sensor = std::make_unique<PressureSensor>();

    sensor_threads.push_back(std::thread(std::ref(*humidity_sensor)));
    sensor_threads.push_back(std::thread(std::ref(*pressure_sensor)));
    sensor_threads.push_back(std::thread(std::ref(*temp_sensor)));
}

/*
    Joining the threads in the destructor like a jthread
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

/*
    Functor for threading
*/

void SensorManager::operator()(std::vector<WeatherReport> *out, std::mutex *lock_out)
{
    report(out, lock_out);
}

/*
    Could probably move all this into the functor?
*/

void displayTime() {
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        //std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Current Time: " << std::ctime(&now) << std::endl;
    }
}

void SensorManager::report(std::vector<WeatherReport> *out, std::mutex *lock_out) const
{
    std::stringstream ss;
    while (1)
    {   std::cout << "\n---------------------";
        //std::cerr << "\nThread: " << std::this_thread::get_id() << " reporting " << '\n';
        std::lock_guard<std::mutex> output_lock(*lock_out);
        WeatherReport wr = {temp_sensor->poll(),
                            humidity_sensor->poll(),
                            pressure_sensor->poll()};
        out->emplace_back(wr);

        std::cout << wr;

        //auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        //ss.str(std::ctime(&now));
        //std::cout << "\n" << ss.str() 
        //          << "Temperature: " << wr.temperature << " C"
        //          << "\nHumidity: " << wr.humidity << " %"
        //          << "\nPressure: " << wr.pressure << " hpa";
        std::this_thread::sleep_for(report_rate);
    }
}