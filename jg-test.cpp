/**
 * @file jg-test.cpp
 * @author binasime, simbachu, DavidReyes88, Shwky88
 * 
 * @brief Group project to summarize the Course 2 (System developing C/C++)
 * A multithreaded program that polls weather reports from different sensors.
 * 
 * @version 0.1
 * @date 2025-01-23
 * 
 * @copyright Copyright (c) 2025
 */

#include <iostream>
#include "SensorManager.h"
#include "Statistics/StatisticsManager.h"


/**
 * @brief main function doxygen test
 * 
 * @return int 
 */


int main(){
    SensorManager s;
    std::vector<WeatherReport> history;
    std::mutex history_lock;

    StatisticsReport statisticsReport;
    std::mutex statistics_lock;
    std::thread stats_making(StatisticsManager::make_StatsReport, std::ref(statisticsReport), std::ref(statistics_lock), &history, std::ref(history_lock));
    
    std::thread weather_monitoring(std::ref(s), &history, &history_lock);
    while (1){
       // while ( history.size() == 0 ){
       // }
       // auto latest = history.back();
       // if ( history.back().time != latest.time ){
       //     std::cout << "Measured " << latest.temperature << "degrees " << '\n';
       //     latest = history.back();
       // }
    }

    weather_monitoring.join(); 
    stats_making.join();
}