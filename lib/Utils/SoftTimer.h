#pragma once
#include <Arduino.h>

class SoftTimer
{
public:
    // Constructor
    SoftTimer();

    // Initialize the timer system
    static void init(unsigned long base_period_us = 100);

    // Timer management
    int createTimer(unsigned long interval_ms, void (*callback)(), bool repeat = true);
    bool startTimer(int timer_id);
    bool stopTimer(int timer_id);
    bool resetTimer(int timer_id);
    bool deleteTimer(int timer_id);

    // Timer info
    bool isTimerActive(int timer_id);
    unsigned long getTimeLeft(int timer_id);
    unsigned long getInterval(int timer_id);
    bool setInterval(int timer_id, unsigned long new_interval_ms);

    // System info
    static int getActiveTimerCount();
    static void printTimerStatus();

private:
    static const int MAX_TIMERS = 10;

    struct TimerData
    {
        unsigned long interval_ticks; // Interval in timer ticks
        unsigned long current_ticks;  // Current tick count
        void (*callback)();           // Function to call
        bool active;                  // Is timer running
        bool repeat;                  // Should timer repeat
        bool used;                    // Is slot used
    };

    static TimerData timers[MAX_TIMERS];
    static unsigned long base_period_us;
    static int active_timer_count;
    static bool initialized;

    // Static interrupt handler
    static void timerInterruptHandler();

    // Helper functions
    static int findFreeSlot();
    static unsigned long msToTicks(unsigned long ms);
    static unsigned long ticksToMs(unsigned long ticks);
};