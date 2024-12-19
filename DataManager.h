#ifndef DATAMANAGER_H
#define DATAMANAGER_H
/* 
#include <mutex>
#include <thread>
#include <vector>

// For testing
struct WeatherReport{
    std::chrono::time_point<std::chrono::system_clock> logged_time {std::chrono::system_clock::now()};
    float temperature;
    float humidity;
    float pressure;
};

class SensorClass{
    public:
        virtual ~SensorClass();
        virtual void set_last_measurement(float incoming_measurement) = 0;
        virtual float poll() const = 0;
};

class TempSensor : public SensorClass{
    private:
        float last_measurement{};
    public:
        virtual float poll() const override;
        void set_last_measurement(float incoming_measurement) override;
};

class HumSensor : public SensorClass{
    private:
        float last_measurement{};
    public:
        virtual float poll () const override;
        void set_last_measurement(float incoming_measurement) override;
};

class WindSensor : public SensorClass{
    private:
        float last_measurement{};
    public:
        virtual float poll () const override;
        void set_last_measurement(float incoming_measurement) override;
};


class DataManager{
    private:
    std::unique_ptr<SensorClass> sensorptr;
    public:
    static void make_report(std::vector<WeatherReport>*out, std::mutex&);
};
 */
#endif