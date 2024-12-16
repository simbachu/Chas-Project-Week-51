#pragma once

#include <chrono>
#include <thread>
#include <vector>
//#include "DataManager.cpp"
#include <mutex>

//Will be declared elsewhere - only for testing
struct WeatherReport{
    //std::chrono::time_point logged_time;
    float temp;
    float humidity;
    float pressure; 
    //float windSpeed;
};
struct StatsData
{
    float recordHigh;
    float recordLow;
    float mean;
    //add more stats-variables
};

struct StatisticsReport
{
    StatsData tempData;
    StatsData humData;
    StatsData windData;
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
    static StatsData generateData(std::vector<float> *Values);

public:
    //Main statistics function. Generates and writes to StatData every 5 sec, based on the entire history vector
    static void make_StatsReport(StatisticsReport &StatData, std::mutex &, std::vector<WeatherReport> *history, std::mutex &);
    
    StatisticsReport &getStatsData() { return latestReport; } // optional. Not currently used
};