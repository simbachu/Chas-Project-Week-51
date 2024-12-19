#include <random>
#include <iostream>

#include "SensorManager.h"
#include "../Statistics/StatisticsManager.h"

int main(){
    SensorManager s;
    std::vector<WeatherReport> history;
    std::mutex history_lock;

    StatisticsReport statisticsReport;
    std::mutex statistics_lock;
    std::thread stats_making(StatisticsManager::make_StatsReport, std::ref(statisticsReport), std::ref(statistics_lock), &history, std::ref(history_lock));
   
    std::thread weather_monitoring(std::ref(s), &history, &history_lock);
    while (1){
        while ( history.size() == 0 ){

        }
        auto latest = history.back();
        if ( history.back().time != latest.time ){
            std::cout << "Measured " << latest.temperature << "degrees " << '\n';
            latest = history.back();
        }
    }

    weather_monitoring.join(); 
    stats_making.join();
}