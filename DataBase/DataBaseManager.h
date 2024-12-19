#pragma once

#include <iostream>
#include <sqlite3.h>
#include "../Statistics/StatisticsManager.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <map>

class DataBaseManager
{

public:
    StatisticsReport currentReport;
    static int callback(void *data, int argc, char **argv, char **azColName);
    sqlite3 *openDatabase(const char *dbName);
    void createTable(sqlite3 *db);
    void insertData(sqlite3 *db, StatisticsReport &Data);
    void readData(sqlite3 *db);
    //std::string convertToString(std::chrono::system_clock::time_point tp);
    //static std::chrono::system_clock::time_point convertToTimePoint(const std::string &time_str);
};
