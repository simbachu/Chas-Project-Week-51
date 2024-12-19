#pragma once

#include <chrono>
#include <ctime>
#include <thread>
#include <vector>
#include <mutex>
#include "../SensorManager.h"

struct StatsData
{
    float recordHigh;
    float recordLow;
    float mean;
    std::chrono::time_point<std::chrono::system_clock> maxTime{std::chrono::system_clock::now()};
    std::chrono::time_point<std::chrono::system_clock> minTime{std::chrono::system_clock::now()};
    //add more stats-variables
};

struct StatisticsReport
{
    StatsData tempData;
    StatsData humData;
    StatsData pressData;
};

/*
En tråd som var 5:e sekund beräknar statstik på de senaste mätningarna
Sensorer generarar data som sparas i "senaste data" och sedan i vektorn history
Var 5:e sek läser statsmanager av denna vektor och genererar data baserad på detta och skriver till statisticsReport.
Maxvärde: recordHigh
Minvärde: recordLow
medelvärde: mean
för temp, luftfuktighet samt vindhastighet
*/

class StatisticsManager
{
    StatisticsReport latestReport; // optional if object instance and not static use only. Not currently used anywhere
    
     //Generate StatsData based on weather values
    static StatsData generateData(const std::vector<std::pair<float, std::chrono::time_point<std::chrono::system_clock>>> &values);
    static void printStats(const StatisticsReport& currentReport, const StatisticsReport& newReport);

public:
    
    static void make_StatsReport(StatisticsReport &StatData, std::mutex &, const std::vector<WeatherReport> *history, std::mutex &);
    StatisticsReport &getStatsData() { return latestReport; } // optional. Not currently used
};