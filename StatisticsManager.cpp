#define DEBUG
#include <algorithm>
#include <iostream>
#include "StatisticsManager.h"
 struct WeatherReport
 {
     float temperature;
     float humidity;
     float pressure;
     std::chrono::time_point<std::chrono::system_clock> time{std::chrono::system_clock::now()};
 };

#ifdef DEBUG
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif

void StatisticsManager::make_StatsReport(StatisticsReport &outStatData, std::mutex &statistics_mutex, const std::vector<WeatherReport> *history, std::mutex &history_mutex)
{
    LOG("Starting thread");
    int i = 0;
    while (1) // add condition
    //while (i<2) //Catch2 test
    {
        std::vector<float> tempValues;
        std::vector<float> humValues;
        std::vector<float> windValues;

        {
            std::unique_lock<std::mutex> lock(history_mutex);
        
            LOG("Locked mutex in stats manager");
            for (const auto &report : *history)
            {
                tempValues.push_back(report.temperature);
                humValues.push_back(report.humidity);
                // windValues.push_back(report.pressure);
            }
        }
        StatsData tempData;
        StatsData humData;
        StatsData windData;

        if (!tempValues.empty())
        {
            LOG("Printing temp data: ");
            tempData = generateData(&tempValues);
        }
        if (!humValues.empty())
        {
            LOG("Printing hum data: ");
            humData = generateData(&humValues);
        }
        //generateData(wind)...

        std::unique_lock<std::mutex> lock(statistics_mutex, std::try_to_lock); // Attempt to lock the mutex
        if (lock.owns_lock())
        {
            outStatData.tempData = tempData;
            outStatData.humData = humData;
            // outStatData.windData = windData;
        }
        std::cout << "StatsManager sleeping for 5 s" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));
        i++;
    }
}

StatsData StatisticsManager::generateData(std::vector<float> *Values)
{
    StatsData Data = StatsData();
    // for(int i = 0; i<Values->size();i++)
    //     {
    //         std::cout << Values->at(i) << std::endl;
    //     }

    Data.recordHigh = *std::max_element(Values->begin(), Values->end());
    LOG("   Max value: " << Data.recordHigh);

    Data.recordLow = *std::min_element(Values->begin(), Values->end());
    LOG("   Min value: " << Data.recordLow);

    float sum(0);
    for (int i = 0; i < Values->size(); i++)
    {
        sum += Values->at(i);
    }
    Data.mean = sum / Values->size();
    LOG("   Mean value: " << Data.mean);

    return Data;
}