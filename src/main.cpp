#include <Arduino.h>
#include <TimerOne.h>
// #include <TimerTwo.h>
#include <LineSensor.h>
#include <Constants.h>
#include <MyLED.h>

MyLED *myLed;
LineSensor *line_sensor1;
LineSensor *line_sensor2;

void setup()
{
  Serial.begin(9600);
  // Timer for line_sensors
  line_sensor1 = new LineSensor(LINESENSOR_LEFT);
  line_sensor1->init();
  line_sensor2 = new LineSensor(LINESENSOR_RIGHT);
  line_sensor2->init();
  Timer1.initialize(200);
  Timer1.attachInterrupt(LineSensor::timerChecker);

  // pinMode(LED_BUILTIN, OUTPUT); // Initialize the digital pin as an output
  myLed = new MyLED(LED_BUILTIN);
}

void loop()
{
  myLed->turnOn();
  delay(500);
  myLed->turnOff();
  Serial.println(line_sensor1->isLineDetected());
  Serial.println(line_sensor2->isLineDetected());
  delay(500);

  /*
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
  delay(100);                      // Wait for 500 milliseconds
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
  delay(100);                      // Wait for 500 milliseconds
  */
}