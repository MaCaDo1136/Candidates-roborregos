#include "Sensors/SensorBase.h"
#include "pico/stdlib.h"
class PrintTerminal
{
public:
    void attachSensor(SensorBase *sensor);
    void startTimer(uint interval_ms = 1000);

private:
    static SensorBase *sensors[20];
    static int sensor_count;
    static repeating_timer_t print_timer;
    static bool timerCallback(repeating_timer_t *rt);
};