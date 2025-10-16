#include "SoftTimer.h"
#include <TimerOne.h>

// Static member initialization
SoftTimer::TimerData SoftTimer::timers[MAX_TIMERS];
unsigned long SoftTimer::base_period_us = 100;
int SoftTimer::active_timer_count = 0;
bool SoftTimer::initialized = false;

SoftTimer::SoftTimer()
{
    // Constructor - nothing special needed
}

void SoftTimer::init(unsigned long base_period_us)
{
    if (initialized)
        return;

    SoftTimer::base_period_us = base_period_us;

    // Initialize all timer slots
    for (int i = 0; i < MAX_TIMERS; i++)
    {
        timers[i].used = false;
        timers[i].active = false;
        timers[i].callback = nullptr;
        timers[i].interval_ticks = 0;
        timers[i].current_ticks = 0;
        timers[i].repeat = false;
    }

    // Setup TimerOne
    Timer1.initialize(base_period_us);
    Timer1.attachInterrupt(timerInterruptHandler);

    initialized = true;

    Serial.println("SoftTimer initialized");
    Serial.print("Base period: ");
    Serial.print(base_period_us);
    Serial.println(" microseconds");
}

int SoftTimer::createTimer(unsigned long interval_ms, void (*callback)(), bool repeat)
{
    if (!initialized || callback == nullptr)
        return -1;

    int slot = findFreeSlot();
    if (slot == -1)
        return -1; // No free slots

    timers[slot].used = true;
    timers[slot].active = false;
    timers[slot].callback = callback;
    timers[slot].interval_ticks = msToTicks(interval_ms);
    timers[slot].current_ticks = 0;
    timers[slot].repeat = repeat;

    Serial.print("Timer created - ID: ");
    Serial.print(slot);
    Serial.print(", Interval: ");
    Serial.print(interval_ms);
    Serial.print("ms, Repeat: ");
    Serial.println(repeat ? "Yes" : "No");

    return slot;
}

bool SoftTimer::startTimer(int timer_id)
{
    if (timer_id < 0 || timer_id >= MAX_TIMERS || !timers[timer_id].used)
        return false;

    timers[timer_id].active = true;
    timers[timer_id].current_ticks = 0; // Reset counter
    active_timer_count++;

    return true;
}

bool SoftTimer::startAllTimers()
{
    bool checking = false;
    for (int i = 0; i < MAX_TIMERS; i++)
    {
        if (timers[i].used)
        {
            startTimer(i);
            checking = true;
        }
    }
    return checking;
}

bool SoftTimer::stopTimer(int timer_id)
{
    if (timer_id < 0 || timer_id >= MAX_TIMERS || !timers[timer_id].used)
        return false;

    if (timers[timer_id].active)
    {
        timers[timer_id].active = false;
        active_timer_count--;
    }

    return true;
}

bool SoftTimer::resetTimer(int timer_id)
{
    if (timer_id < 0 || timer_id >= MAX_TIMERS || !timers[timer_id].used)
        return false;

    timers[timer_id].current_ticks = 0;
    return true;
}

bool SoftTimer::deleteTimer(int timer_id)
{
    if (timer_id < 0 || timer_id >= MAX_TIMERS || !timers[timer_id].used)
        return false;

    if (timers[timer_id].active)
        active_timer_count--;

    timers[timer_id].used = false;
    timers[timer_id].active = false;
    timers[timer_id].callback = nullptr;

    return true;
}

bool SoftTimer::isTimerActive(int timer_id)
{
    if (timer_id < 0 || timer_id >= MAX_TIMERS || !timers[timer_id].used)
        return false;

    return timers[timer_id].active;
}

unsigned long SoftTimer::getTimeLeft(int timer_id)
{
    if (timer_id < 0 || timer_id >= MAX_TIMERS || !timers[timer_id].used)
        return 0;

    if (!timers[timer_id].active)
        return 0;

    unsigned long remaining_ticks = timers[timer_id].interval_ticks - timers[timer_id].current_ticks;
    return ticksToMs(remaining_ticks);
}

unsigned long SoftTimer::getInterval(int timer_id)
{
    if (timer_id < 0 || timer_id >= MAX_TIMERS || !timers[timer_id].used)
        return 0;

    return ticksToMs(timers[timer_id].interval_ticks);
}

bool SoftTimer::setInterval(int timer_id, unsigned long new_interval_ms)
{
    if (timer_id < 0 || timer_id >= MAX_TIMERS || !timers[timer_id].used)
        return false;

    timers[timer_id].interval_ticks = msToTicks(new_interval_ms);
    timers[timer_id].current_ticks = 0; // Reset counter

    return true;
}

int SoftTimer::getActiveTimerCount()
{
    return active_timer_count;
}

void SoftTimer::printTimerStatus()
{
    Serial.println("=== SoftTimer Status ===");
    Serial.print("Active timers: ");
    Serial.println(active_timer_count);

    for (int i = 0; i < MAX_TIMERS; i++)
    {
        if (timers[i].used)
        {
            Serial.print("Timer ");
            Serial.print(i);
            Serial.print(": ");
            Serial.print(timers[i].active ? "ACTIVE" : "STOPPED");
            Serial.print(", Interval: ");
            Serial.print(ticksToMs(timers[i].interval_ticks));
            Serial.print("ms, Time left: ");
            Serial.print(ticksToMs(timers[i].interval_ticks - timers[i].current_ticks));
            Serial.print("ms, Repeat: ");
            Serial.println(timers[i].repeat ? "Yes" : "No");
        }
    }
    Serial.println("========================");
}

// Private helper functions
void SoftTimer::timerInterruptHandler()
{
    for (int i = 0; i < MAX_TIMERS; i++)
    {
        if (timers[i].used && timers[i].active)
        {
            timers[i].current_ticks++;

            // Check if timer has expired
            if (timers[i].current_ticks >= timers[i].interval_ticks)
            {
                // Call the callback function
                if (timers[i].callback != nullptr)
                {
                    timers[i].callback();
                }

                // Handle repeat logic
                if (timers[i].repeat)
                {
                    timers[i].current_ticks = 0; // Reset for next cycle
                }
                else
                {
                    timers[i].active = false; // One-shot timer stops
                    active_timer_count--;
                }
            }
        }
    }
}

int SoftTimer::findFreeSlot()
{
    for (int i = 0; i < MAX_TIMERS; i++)
    {
        if (!timers[i].used)
            return i;
    }
    return -1; // No free slots
}

unsigned long SoftTimer::msToTicks(unsigned long ms)
{
    return (ms * 1000) / base_period_us;
}

unsigned long SoftTimer::ticksToMs(unsigned long ticks)
{
    return (ticks * base_period_us) / 1000;
}