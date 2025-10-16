#include <Arduino.h>
#include <TimerOne.h>
// #include <TimerTwo.h>
#include <LineSensor.h>
#include <EncoderSensor.h>
#include <Constants.h>
#include <MyLED.h>

MyLED *myLed;
LineSensor *line_left;
LineSensor *line_right;
EncoderSensor *encoder_left;
EncoderSensor *encoder_right;

void setup()
{
  Serial.begin(9600);
  // Timer for line_sensors
  line_left = new LineSensor(LINESENSOR_LEFT);
  line_left->init();
  line_right = new LineSensor(LINESENSOR_RIGHT);
  line_right->init();
  Timer1.initialize(200);
  Serial.println(1 % 2);
  Serial.println(2 % 2);
  Timer1.attachInterrupt(LineSensor::timerChecker);
  // pinMode(LINESENSOR_LEFT, INPUT_PULLUP);
  // pinMode(LINESENSOR_RIGHT, INPUT_PULLUP);

  // Init encoder sensors

  encoder_left = new EncoderSensor(ENCODERSENSOR_LEFT);
  encoder_left->init();
  encoder_right = new EncoderSensor(ENCODERSENSOR_RIGHT);
  encoder_right->init();

  // pinMode(LED_BUILTIN, OUTPUT); // Initialize the digital pin as an output
  myLed = new MyLED(LED_BUILTIN);
}

void loop()
{
  myLed->turnOn();
  delay(150);
  myLed->turnOff();
  // Serial.println(line_left->isLineDetected());
  // Serial.println(line_right->isLineDetected());
  Serial.println(encoder_left->getCount());
  Serial.println(encoder_right->getCount());

  delay(150);

  /*
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
  delay(100);                      // Wait for 500 milliseconds
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
  delay(100);                      // Wait for 500 milliseconds
  */
}