#include <iostream>
#include <algorithm>
#include <random>
#include "SensorManager.h"

namespace sensor_reading{
    float random(float low, float high, std::mt19937 &gen){
        std::uniform_real_distribution<float> interval (low, high);
        return interval(gen);
    }
}

Sensor::~Sensor(){

}

void TempSensor::operator()() {
    std::mt19937 mt (std::random_device{}());
    while(1){
        last_measurement = sensor_reading::random(5.0f, 15.0f, mt);
        std::cerr << std::this_thread::get_id() << " " << last_measurement << '\n';
        std::this_thread::sleep_for(rate);
    }
}


float TempSensor::poll() const {
    return last_measurement;
}

SensorManager::SensorManager(){
    temp_sensor = std::make_unique<TempSensor>();
    sensor_threads.push_back(std::thread(std::ref(*temp_sensor)));
}

SensorManager::~SensorManager(){
    for (auto& thread : sensor_threads){
        thread.join();
    }
}

void SensorManager::operator()(std::vector<WeatherReport> *out, std::mutex *lock_out){
    report(out, lock_out);
}

void SensorManager::report(std::vector<WeatherReport> *out, std::mutex *lock_out) const{
    while (1){
        std::cerr << " " << std::this_thread::get_id() << " reporting " << '\n';
        std::lock_guard<std::mutex> output_lock(*lock_out);
        out->emplace_back(temp_sensor->poll(), 0.0, 0.0);
        std::this_thread::sleep_for(report_rate);
    }
}