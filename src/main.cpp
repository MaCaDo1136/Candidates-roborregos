#include <Arduino.h>
#include <TimerOne.h>

#include <MyLED.h>

MyLED *myLed;

void setup()
{

  // pinMode(LED_BUILTIN, OUTPUT); // Initialize the digital pin as an output
  myLed = new MyLED(LED_BUILTIN);
}

void loop()
{
  myLed->turnOn();
  delay(500);
  myLed->turnOff();
  delay(500);

  /*
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
  delay(100);                      // Wait for 500 milliseconds
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
  delay(100);                      // Wait for 500 milliseconds
  */
}