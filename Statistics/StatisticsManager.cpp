//#define DEBUG
#include <algorithm>
#include <iostream>
#include "StatisticsManager.h"
#include "../DataBase/DataBaseManager.h"
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

//Main statistics function. Generates and writes to StatData every 5 sec, based on the entire history vector
void StatisticsManager::make_StatsReport(StatisticsReport &outReport, std::mutex &statistics_mutex, const std::vector<WeatherReport> *history, std::mutex &history_mutex)
{
    DataBaseManager dbManager;
    sqlite3* db = dbManager.openDatabase("./DataBase/StatisticsDB.db"); //edit the path to your \\ProjectWeek repo
     if (!db)
     {
        std::cerr << "Error opening database" << std::endl;
        return;
     } 

    dbManager.createTable(db);
    dbManager.readData(db); //this populates dbManager.currentReport
    
    LOG("Starting thread");
    int i = 0;
    while (1) // add condition
    //while (i<2) //Catch2 test
    {
        std::vector<float> tempValues;
        std::vector<float> humValues;
        std::vector<float> pressValues;
        int history_size;
        {
            std::unique_lock<std::mutex> lock(history_mutex);
        
            history_size = history->size();
            LOG("Locked mutex in stats manager");
            //Loop through history and save values
            for (const auto &report : *history)
            {
                tempValues.push_back(report.temperature);
                humValues.push_back(report.humidity);
                pressValues.push_back(report.pressure);
            }
        }
        //Create structs that will make up the StatisticsReport (outReport)
        StatsData tempData;
        StatsData humData;
        StatsData pressData;
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

        std::unique_lock<std::mutex> lock(statistics_mutex, std::try_to_lock); // Attempt to lock the mutex
        if (lock.owns_lock())
        {
            outReport.tempData = tempData;
            outReport.humData = humData;
            outReport.pressData = pressData;

            //Save the report to a SQL Database
            dbManager.insertData(db, outReport);
        }
        if(history_size > 0)
        //readData also prints out the info
        dbManager.readData(db);
        std::cout << "StatsManager sleeping for 5 s" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));
        i++;
    }
    sqlite3_close(db);
}

 //Generate StatsData based on weather values
StatsData StatisticsManager::generateData(std::vector<float> *Values)
{
    StatsData Data = StatsData();

    Data.recordHigh = *std::max_element(Values->begin(), Values->end());
    LOG("   Max value: " << Data.recordHigh);

    Data.recordLow = *std::min_element(Values->begin(), Values->end());
    LOG("   Min value: " << Data.recordLow);

    float sum(0);
    for (int i = 0; i < Values->size(); i++)
    {
        sum += Values->at(i);
    }
    Data.mean = sum / (Values->size());
    LOG("   Mean value: " << Data.mean);

    return Data;
}