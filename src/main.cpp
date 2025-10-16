#include <Arduino.h>
#include <TimerOne.h>
// #include <TimerTwo.h>
#include <LineSensor.h>
#include <EncoderSensor.h>
#include <UltrasonicSensor.h>
#include <Constants.h>
#include <MyLED.h>

MyLED *myLed;
LineSensor *line_left;
LineSensor *line_right;
EncoderSensor *encoder_left;
EncoderSensor *encoder_right;
UltrasonicSensor *ultrasonic_frontUp;
UltrasonicSensor *ultrasonic_frontDown;
UltrasonicSensor *ultrasonic_left;
UltrasonicSensor *ultrasonic_right;

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

  // Init Ultrasonic sensors
  ultrasonic_frontUp = new UltrasonicSensor(ULTRASONIC_FRONTUP_TRIG, ULTRASONIC_FRONTUP_ECHO);
  ultrasonic_frontUp->init();
  ultrasonic_frontDown = new UltrasonicSensor(ULTRASONIC_FRONTDOWN_TRIG, ULTRASONIC_FRONTDOWN_ECHO);
  ultrasonic_frontDown->init();
  ultrasonic_left = new UltrasonicSensor(ULTRASONIC_LEFT_TRIG, ULTRASONIC_LEFT_ECHO);
  ultrasonic_left->init();
  ultrasonic_right = new UltrasonicSensor(ULTRASONIC_RIGHT_TRIG, ULTRASONIC_RIGHT_ECHO);
  ultrasonic_right->init();

  // pinMode(LED_BUILTIN, OUTPUT); // Initialize the digital pin as an output
  myLed = new MyLED(LED_BUILTIN);
}

void loop()
{
  myLed->turnOn();
  delay(150);
  myLed->turnOff();
  // Serial.println("line left:");
  // Serial.println(line_left->isLineDetected());
  // Serial.println("line right:");
  // Serial.println(line_right->isLineDetected());
  // Serial.println("Left encoder:");
  // Serial.println(encoder_left->getCount());
  // Serial.println("Right encoder:");
  // Serial.println(encoder_right->getCount());
  Serial.println("Ultrasonic front up:");
  Serial.println(ultrasonic_frontUp->getDistance());
  Serial.println("Ultrasonic front down:");
  Serial.println(ultrasonic_frontDown->getDistance());
  Serial.println("Ultrasonic left:");
  Serial.println(ultrasonic_left->getDistance());
  Serial.println("Ultrasonic right:");
  Serial.println(ultrasonic_right->getDistance());

  delay(150);

  /*
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
  delay(100);                      // Wait for 500 milliseconds
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
  delay(100);                      // Wait for 500 milliseconds
  */
}