#include "PrintTerminal.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

SensorBase *PrintTerminal::sensors[20] = {nullptr};
int PrintTerminal::sensor_count = 0;
repeating_timer_t PrintTerminal::print_timer;

void PrintTerminal::attachSensor(SensorBase *sensor)
{
    if (sensor_count < 20)
        sensors[sensor_count++] = sensor;
}

bool PrintTerminal::timerCallback(repeating_timer_t *rt)
{
    for (int i = 0; i < sensor_count; i++)
    {
        sensors[i]->printValue();
    }
    printf("----\n");
    return true;
}

void PrintTerminal::startTimer(uint interval_ms)
{
    add_repeating_timer_ms(interval_ms, PrintTerminal::timerCallback, NULL, &print_timer);
};