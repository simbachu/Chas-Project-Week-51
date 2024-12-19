#ifndef TIME_WEATHER_DISPLAY_H
#define TIME_WEATHER_DISPLAY_H

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include "SensorManager.h"

extern std::mutex mtx;

void displayTime();

void displayWeather(WeatherReport &wr);

#endif