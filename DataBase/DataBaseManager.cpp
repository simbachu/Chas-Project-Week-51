#include "DataBaseManager.h"

/*
A c_cpp_properties.json file like this needs to be added to .vscode
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/sqlite"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "intelliSenseMode": "windows-gcc-x64",
            "compilerPath": "C:/msys64/ucrt64/bin/g++.exe",
            "cppStandard": "c++17",
            "cStandard": "c17"
        }
    ],
    "version": 4
}
and the following lines needs to be added to task.json
"-I", "C:/sqlite",
"-L", "C:/sqlite",
"-lsqlite3"
*/
void updateCurrentReport(std::string weatherMeasurement, StatsData &Data, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        if (std::string(azColName[i]) == "weatherDataType")
        {
            if (std::string(azColName[i]) != weatherMeasurement)
            {
                break;
            }
        }
        else if (std::string(azColName[i]) == "recordHigh")
        {
            Data.recordHigh = argv[i] ? std::stof(argv[i]) : 0.0f;
        }
        else if (std::string(azColName[i]) == "recordLow")
        {
            Data.recordLow = argv[i] ? std::stof(argv[i]) : 0.0f;
        }
        else if (std::string(azColName[i]) == "mean")
        {
            Data.mean = argv[i] ? std::stof(argv[i]) : 0.0f;
        }
    }
}

// Callback function for SELECT queries
int DataBaseManager::callback(void *data, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        //Prints the table content
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;

    //check if the data is of type StatisticsReport
    StatisticsReport *weatherDataList = static_cast<StatisticsReport *>(data);
    //Loop through the table and add relevant table_data to "data(currentReport)"
    StatsData t,h,w;
    updateCurrentReport("TemperatureData", t, argc, argv, azColName);
    weatherDataList->tempData = t;

    updateCurrentReport("HumidityData", h, argc, argv, azColName);
    weatherDataList->humData = h;

    updateCurrentReport("PressureData", w, argc, argv, azColName);
    weatherDataList->pressData = w;

    return 0;
}

// Function to open SQLite database
sqlite3 *DataBaseManager::openDatabase(const char *dbName)
{
    sqlite3 *db;
    int rc = sqlite3_open(dbName, &db);
    if (rc)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    else
    {
        std::cout << "Opened database successfully!" << std::endl;
        return db;
    }
}

// Function to create table
void DataBaseManager::createTable(sqlite3 *db)
{
    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS Statistics ("
                                 "weatherDataType TEXT, "
                                 "recordHigh FLOAT, "
                                 "recordLow FLOAT, "
                                 "mean FLOAT);";
    char *errMsg = 0;
    int rc = sqlite3_exec(db, createTableSQL, nullptr, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "Table created successfully" << std::endl;
    }
}

// Function to insert a WeatherReport
void DataBaseManager::insertData(sqlite3 *db, StatisticsReport &Data)
{
    currentReport = Data;
    char *errMsg = 0;
    std::string insertSQL = 
    "DELETE FROM Statistics; " //Clear the table from previous entries
    "INSERT INTO Statistics (weatherDataType, recordHigh, recordLow, mean) VALUES "
    "('TemperatureData', " + std::to_string(Data.tempData.recordHigh) + ", " + 
    std::to_string(Data.tempData.recordLow) + ", " + std::to_string(Data.tempData.mean) + "), "
    "('HumidityData', " + std::to_string(Data.humData.recordHigh) + ", " + 
    std::to_string(Data.humData.recordLow) + ", " + std::to_string(Data.humData.mean) + "), "
    "('PressureData', " + std::to_string(Data.pressData.recordHigh) + ", " + 
    std::to_string(Data.pressData.recordLow) + ", " + std::to_string(Data.pressData.mean) + ");";

    int rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "Data added successfully!" << std::endl;
    }
}

// Function to read statistics
void DataBaseManager::readData(sqlite3 *db)
{
    const char *selectSQL = "SELECT * FROM Statistics;";
    char *errMsg = 0;
    int rc = sqlite3_exec(db, selectSQL, callback, &currentReport, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "Statsistics data fetched successfully!" << std::endl;
    }
}

// int main() {
//     sqlite3* db = openDatabase("C:\\Chas\\ProjectWeek\\StatisticsDB.db"); //edit the path to your \\ProjectWeek repo
//     if (!db) return 1;

//     createTable(db);

//     // Example of inserting a contact
//     //insertData(db, Data);

//     // Example of reading all contacts
//     readData(db);

//     sqlite3_close(db);
//     return 0;
// }
