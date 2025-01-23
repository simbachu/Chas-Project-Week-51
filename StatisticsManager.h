#pragma once

#include <chrono>
#include <ctime>
#include <thread>
#include <vector>
#include <mutex>
#include "../SensorManager.h"

/**
 * @brief Struct for storing weather statistics data for a single type of weatherdata
 * 
 */
struct StatsData
{
    float recordHigh;
    float recordLow;
    float mean;
    std::chrono::time_point<std::chrono::system_clock> maxTime{std::chrono::system_clock::now()};
    std::chrono::time_point<std::chrono::system_clock> minTime{std::chrono::system_clock::now()};
};

/**
 * @brief Struct for storing weather statistics data for all types of weatherdata
 * 
 */
struct StatisticsReport
{
    StatsData tempData;
    StatsData humData;
    StatsData pressData;
};

/**
 * @brief Class to manage statistics of weather data
 */
class StatisticsManager
{
    
    static StatsData generateData(const std::vector<std::pair<float, std::chrono::time_point<std::chrono::system_clock>>> &values);
    static void printStats(const StatisticsReport& currentReport, const StatisticsReport& newReport);

public:
    static void make_StatsReport(StatisticsReport &StatData, std::mutex &, const std::vector<WeatherReport> *history, std::mutex &);

};