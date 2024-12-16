#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "StatisticsManager.h"

//
//To be able to run this test you need to comment out the main func in StatisticsManager.cpp!!!!!!
//
TEST_CASE( "Simple Stats test" ){

    std::vector<WeatherReport> history;
    //Populate history with test values
    WeatherReport wr1 = {0.6,1.2,100.0};
    WeatherReport wr2 = {0.1,26.2,99.0};
    history.push_back(wr1);
    history.push_back(wr2);

    //Declare variables
    StatisticsReport statisticsReport;
    std::mutex history_mutex;
    std::mutex statistics_mutex;

    //Run thread for x(defined in StatisticsManager) iterations with 5 sec wait
    std::thread stats_making(StatisticsManager::make_StatsReport, std::ref(statisticsReport), std::ref(statistics_mutex), &history, std::ref(history_mutex));
    stats_making.join();

    //test output
    REQUIRE(statisticsReport.tempData.recordHigh == Approx(0.6));
    REQUIRE(statisticsReport.tempData.recordLow == Approx(0.1));
    REQUIRE(statisticsReport.tempData.mean == Approx(0.35));
    REQUIRE(statisticsReport.humData.recordHigh == Approx(26.2));
    REQUIRE(statisticsReport.humData.recordLow == Approx(1.2));
    REQUIRE(statisticsReport.humData.mean == Approx(13.7));
}