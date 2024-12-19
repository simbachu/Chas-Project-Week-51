// #define DEBUG
#include <algorithm>
#include <iostream>
#include "StatisticsManager.h"
#include "../DataBase/DataBaseManager.h"

#ifdef DEBUG
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif

// Main statistics function. Generates and writes to StatData every 5 sec, based on the entire history vector
void StatisticsManager::make_StatsReport(StatisticsReport &outReport, std::mutex &statistics_mutex, const std::vector<WeatherReport> *history, std::mutex &history_mutex)
{
    DataBaseManager dbManager;
    sqlite3 *db = dbManager.openDatabase("./DataBase/StatisticsDB.db"); // edit the path to your \\ProjectWeek repo
    if (!db)
    {
        std::cerr << "Error opening database" << std::endl;
        return;
    }
    dbManager.createTable(db);

    LOG("Starting thread");
    int i = 0;
    while (1)
    {
        StatisticsReport currentReport;
        dbManager.readData(db); 
        {
            std::unique_lock<std::mutex> lock(statistics_mutex); // Attempt to lock the mutex
            currentReport = outReport;
        }
        std::vector<std::pair<float, std::chrono::time_point<std::chrono::system_clock>>> tempValues;
        std::vector<std::pair<float, std::chrono::time_point<std::chrono::system_clock>>> humValues;
        std::vector<std::pair<float, std::chrono::time_point<std::chrono::system_clock>>> pressValues;

        // Create structs that will make up the StatisticsReport (outReport)
        StatsData tempData;
        StatsData humData;
        StatsData pressData;
        int history_size;
        {
            std::unique_lock<std::mutex> lock(history_mutex);

            history_size = history->size();
            LOG("Locked mutex in stats manager");
            // Loop through history and save values
            for (const auto &report : *history)
            {
                tempValues.emplace_back(report.temperature, report.time);
                humValues.emplace_back(report.humidity, report.time);
                pressValues.emplace_back(report.pressure, report.time);
            }
            // Generate data: max,min,mean,timepoints
            if (!tempValues.empty())
            {
                tempData = generateData(tempValues);
            }
            if (!humValues.empty())
            {
                humData = generateData(humValues);
            }
            if (!pressValues.empty())
            {
                pressData = generateData(pressValues);
            }
        }

        std::unique_lock<std::mutex> lock(statistics_mutex, std::try_to_lock); // Attempt to lock the mutex
        if (lock.owns_lock())
        {
            outReport.tempData = tempData;
            outReport.humData = humData;
            outReport.pressData = pressData;
            printStats(currentReport, outReport);
            // Save the report to a SQL Database
            dbManager.insertData(db, outReport); // Updates dbManager.currentReport and writes to outReport
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    sqlite3_close(db);
}

// Generate StatsData based on weather values. Caluclates max,min,mean and associates timepoints to max,min values
StatsData StatisticsManager::generateData(const std::vector<std::pair<float, std::chrono::time_point<std::chrono::system_clock>>> &values)
{
    StatsData data;
    if (values.empty())
    {
        data.recordHigh = data.recordLow = data.mean = 0;
        return data;
    }

    data.recordHigh = values.front().first;
    data.recordLow = values.front().first;
    data.mean = 0;

    data.maxTime = values.front().second;
    data.minTime = values.front().second;

    float sum = 0;
    for (const auto &value : values)
    {
        sum += value.first;
        if (value.first > data.recordHigh)
        {
            data.recordHigh = value.first;
            data.maxTime = value.second;
        }
        if (value.first < data.recordLow)
        {
            data.recordLow = value.first;
            data.minTime = value.second;
        }
    }
    data.mean = sum / values.size();
    return data;
}

//print possible new record measurements.
void StatisticsManager::printStats(const StatisticsReport &currentReport, const StatisticsReport &newReport)
{
    // Check if new high scores. If so the stream reflects this
    bool newTempHigh = false, newHumHigh = false, newPressHigh = false;
    if (newReport.tempData.recordHigh > currentReport.tempData.recordHigh)
        newTempHigh = true;
    if (newReport.humData.recordHigh > currentReport.humData.recordHigh)
        newHumHigh = true;
    if (newReport.pressData.recordHigh > currentReport.pressData.recordHigh)
        newPressHigh = true;

    std::ostringstream stream;

    // Temperature report
    time_t tempTime = std::chrono::system_clock::to_time_t(newReport.tempData.maxTime);
    stream << "\nRecord high temperature: " << newReport.tempData.recordHigh
           << " measured at " << std::ctime(&tempTime);
    if (newTempHigh)
        stream << "This is a new record high temperature!\n\n";

    // Humidity report
    time_t humTime = std::chrono::system_clock::to_time_t(newReport.humData.maxTime);
    stream << "Record high humidity: " << newReport.humData.recordHigh
           << " measured at " << std::ctime(&humTime);
    if (newHumHigh)
        stream << "This is a new record high humidity!\n\n";

    // Pressure report
    time_t pressTime = std::chrono::system_clock::to_time_t(newReport.pressData.maxTime);
    stream << "Record high pressure: " << newReport.pressData.recordHigh
           << " measured at " << std::ctime(&pressTime);
    if (newPressHigh)
        stream << "This is a new record high pressure!\n\n";

    std::cout << stream.str();
}

