#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>
#include <sstream>
#include "DisplayReport.h"

std::mutex mtx;

void displayTime() {
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));       
        
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        
        std::lock_guard<std::mutex> lock(mtx);       
        std::cout << "Current Time: " << std::ctime(&now) << std::endl;
    }
}

void displayWeather(WeatherReport &wr){

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss(std::ctime(&now));
        std::cout << "\n" << ss.str() 
                  << "Temperature: " << wr.temperature << " C"
                  << "\nHumidity: " << wr.humidity << " %"
                  << "\nPressure: " << wr.pressure << " hpa";
}
