#include "InterruptManager.h"
#include "hardware/gpio.h"

// Static member initialization
gpio_handler_t InterruptManager::handlers[30] = {nullptr};

void InterruptManager::attach(uint gpio, uint32_t events, gpio_handler_t handler)
{
    if (gpio < 30)
    {
        handlers[gpio] = handler;
        gpio_set_irq_enabled_with_callback(gpio, events, true, &global_callback);
    }
}

void InterruptManager::detach(uint gpio)
{
    if (gpio < 30)
    {
        handlers[gpio] = nullptr;
        gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
    }
}

void InterruptManager::global_callback(uint gpio, uint32_t events)
{
    if (gpio < 30 && handlers[gpio] != nullptr)
    {
        handlers[gpio](gpio, events);
    }
}