#include <Arduino.h>
#include <TimerOne.h>
// #include <Timers.h>
#include <LineSensor.h>
#include <EncoderSensor.h>
#include <UltrasonicSensor.h>
#include <SoftTimer.h>
#include <Constants.h>
#include <MyLED.h>
#include <Motors.h>
#include <LineFollower.h>

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
Motor *motor_intake;
Motor *motor_right;
Motor *motor_left;
LineFollower *lineFollower;

// lectura de sensores ultrasonicos

float getUltrasonicDistance(int trigPin, int echoPin)
{
  // Asegurar que el TRIG esté en LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Enviar pulso de 10 microsegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leer duración del pulso de respuesta
  long duration = pulseIn(echoPin, HIGH, 30000); // timeout de 30 ms

  // Calcular distancia en centímetros
  float distance = duration * 0.01715f;

  // Si no hubo lectura válida (timeout)
  if (duration == 0)
    return -1.0;

  return distance;
}

void printing()
{
  // Serial.println("line left:");
  // Serial.println(line_left->isLineDetected());
  // Serial.println("line right:");
  // Serial.println(line_right->isLineDetected());
  // Serial.println("L: ");
  // Serial.println(encoder_left->getCount());
  // Serial.println("R: ");
  // Serial.println(encoder_right->getCount());
  // Serial.println(digitalPinToInterrupt(ENCODERSENSOR_LEFT));
  // Serial.println(digitalPinToInterrupt(ENCODERSENSOR_RIGHT));
  Serial.println("up: ");
  Serial.println(getUltrasonicDistance(ULTRASONIC_FRONTDOWN_TRIG, ULTRASONIC_FRONTDOWN_ECHO));
  // Serial.println("up:");
  // Serial.println(ultrasonic_frontUp->getDistance());
  // Serial.println("down:");
  // Serial.println(ultrasonic_frontDown->getDistance());
  // Serial.println("left:");
  // Serial.println(ultrasonic_left->getDistance());
  // Serial.println("right:");
  // Serial.println(ultrasonic_right->getDistance());
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
  // myTimer.createTimer(1, UltrasonicSensor::timerChecker, true);
  // Timer2.attachInterrupt(UltrasonicSensor::timerChecker);
  // Timer2.init(100);

  // Init motors
  motor_right = new Motor(MOTOR_RIGHT_PWM, MOTOR_RIGHT_IN1, MOTOR_RIGHT_IN2);
  motor_right->init();
  motor_intake = new Motor(MOTOR_INTAKE_PWM, MOTOR_INTAKE_IN1, MOTOR_INTAKE_IN2);
  motor_intake->init();
  motor_left = new Motor(MOTOR_LEFT_PWM, MOTOR_LEFT_IN1, MOTOR_LEFT_IN2);
  motor_left->init();

  // pinMode(LED_BUILTIN, OUTPUT); // Initialize the digital pin as an output
  myLed = new MyLED(LED_BUILTIN);

  // Set up printing with soft timer
  myTimer.createTimer(100, printing, true);
  // Finally start all timers
  myTimer.startAllTimers();

  // LineFollower setup
  lineFollower = new LineFollower(line_left, line_right, motor_left, motor_right);
  lineFollower->setInverted(false, true);
  lineFollower->setSpeeds(120, 80, 100); // base, turn, spin
  lineFollower->setDebounce(4);
  lineFollower->setTimeout(400);
  lineFollower->init();
}

// Modos para elegir la pista:
enum Mode
{
  MODE_LINE_FOLLOW,
  MODE_TEST_INTAKE
};

void loop()
{
  Mode currentMode = MODE_TEST_INTAKE;

  float distance = getUltrasonicDistance(ULTRASONIC_FRONTUP_TRIG, ULTRASONIC_FRONTUP_ECHO);
  float distance_down = getUltrasonicDistance(ULTRASONIC_FRONTDOWN_TRIG, ULTRASONIC_FRONTDOWN_ECHO);
  float distance_left = getUltrasonicDistance(ULTRASONIC_LEFT_TRIG, ULTRASONIC_LEFT_ECHO);
  float distance_right = getUltrasonicDistance(ULTRASONIC_RIGHT_TRIG, ULTRASONIC_RIGHT_ECHO);

  switch (currentMode)
  {
  case MODE_LINE_FOLLOW:
    // lineFollower->update();
    break;
  case MODE_TEST_INTAKE:
    if (distance_down > -1.1 && distance_down < 4)
    {
      motor_intake->setSpeed(0);
    }
    else
    {
      motor_intake->setSpeed(-254);
    }
    break;
  default:
    break;
  }
  delay(10);
}