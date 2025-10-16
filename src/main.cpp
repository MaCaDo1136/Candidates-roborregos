#include <Arduino.h>
// #include <TimerTwo.h>
#include <LineSensor.h>
#include <EncoderSensor.h>
#include <UltrasonicSensor.h>
#include <SoftTimer.h>
#include <Constants.h>
#include <MyLED.h>

// Easter egg

MyLED *myLed;
LineSensor *line_left;
LineSensor *line_right;
EncoderSensor *encoder_left;
EncoderSensor *encoder_right;
UltrasonicSensor *ultrasonic_frontUp;
UltrasonicSensor *ultrasonic_frontDown;
UltrasonicSensor *ultrasonic_left;
UltrasonicSensor *ultrasonic_right;

void printing()
{
  // Serial.println("line left:");
  // Serial.println(line_left->isLineDetected());
  // Serial.println("line right:");
  // Serial.println(line_right->isLineDetected());
  Serial.println("L: ");
  Serial.println(encoder_left->getCount());
  Serial.println("R: ");
  Serial.println(encoder_right->getCount());
  // Serial.println(digitalPinToInterrupt(ENCODERSENSOR_LEFT));
  // Serial.println(digitalPinToInterrupt(ENCODERSENSOR_RIGHT));
  //   Serial.println("Ultrasonic front up:");
  //   Serial.println(ultrasonic_frontUp->getDistance());
  //   Serial.println("Ultrasonic front down:");
  //   Serial.println(ultrasonic_frontDown->getDistance());
  //   Serial.println("Ultrasonic left:");
  //   Serial.println(ultrasonic_left->getDistance());
  //   Serial.println("Ultrasonic right:");
  //   Serial.println(ultrasonic_right->getDistance());
}

void setup()
{

  Serial.begin(9600);
  SoftTimer myTimer = SoftTimer();
  // Timer for line_sensors
  line_left = new LineSensor(LINESENSOR_LEFT);
  line_left->init();
  line_right = new LineSensor(LINESENSOR_RIGHT);
  line_right->init();
  myTimer.init(50);
  myTimer.createTimer(10, LineSensor::timerChecker, true);
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
  myTimer.createTimer(1, UltrasonicSensor::timerChecker, true);

  // pinMode(LED_BUILTIN, OUTPUT); // Initialize the digital pin as an output
  myLed = new MyLED(LED_BUILTIN);

  // Set up printing with soft timer
  myTimer.createTimer(100, printing, true);
  // Finally start all timers
  myTimer.startAllTimers();
}

void loop()
{
  myLed->turnOn();
  delay(200);
  myLed->turnOff();
  delay(200);
}