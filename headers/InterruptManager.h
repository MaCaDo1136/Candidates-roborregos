#pragma once
#include "pico/stdlib.h"

// Function pointer type for GPIO handlers
typedef void (*gpio_handler_t)(uint gpio, uint32_t events);

class InterruptManager
{
public:
    // Attach a handler to a GPIO pin
    static void attach(uint gpio, uint32_t events, gpio_handler_t handler);

    // Detach a handler from a GPIO pin
    static void detach(uint gpio);

private:
    static gpio_handler_t handlers[30]; // Table of handlers
    static void global_callback(uint gpio, uint32_t events);
};
