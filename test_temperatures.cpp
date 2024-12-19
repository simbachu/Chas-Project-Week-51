/* #define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"

TEST_CASE( "Measure values" ){

}

TEST_CASE( "Read out value with poll()"){
    float expected[4] {0.01, 0.05};
    TempSensor t;
    REQUIRE(t.poll() = expected[0]);
}

TEST_CASE( "Read out random values"){
    TempSensor t;
    REQUIRE_THAT(t.poll() < 15.0);
}

SCENARIO("We want to measure the temperature"){
    WeatherReport expected {15.0, 87.4, 1004.0};

    GIVEN("We have a temperature sensor and a data manager"){
        TempSensor t;
        DataManager d;
        std::vector<WeatherReport> q;
        WHEN("The sensor is polled,"){
            t.poll();
            THEN ("Result"){
                REQUIRE(q.back() = expected);
            }
        }
    }
} */