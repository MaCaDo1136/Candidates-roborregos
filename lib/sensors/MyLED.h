#include <Arduino.h>

class MyLED {
public:
    // Constructor: Initializes the LED with a specific pin
    MyLED(int pin);

    // Method to turn the LED on
    void turnOn();

    // Method to turn the LED off
    void turnOff();

    // Method to toggle the LED state
    void toggle();

private:
    int _ledPin; // Private member variable to store the LED pin number
};