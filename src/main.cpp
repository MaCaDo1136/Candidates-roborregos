#include <Arduino.h>
#include <TimerOne.h>
// #include <Timers.h>
#include <LineSensor.h>
#include <EncoderSensor.h>
#include <UltrasonicSensor.h>
#include <Odometry.h>
#include <SoftTimer.h>
#include <Constants.h>
#include <MyLED.h>
#include <Motors.h>
#include <LineFollower.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <ColorSensor.h>
#include <PistaA.h>

// Easter egg

MyLED *myLed;
LineSensor *line_left;
LineSensor *line_right;
EncoderSensor *encoder_left;
EncoderSensor *encoder_right;
Motor *motor_intake;
Motor *motor_right;
Motor *motor_left;
LineFollower *lineFollower;
PistaA *pistaA;
ColorSensor *colorSensorFront;
ColorSensor *colorSensorBack;

Odometry *myOdom;
int current_time;

Adafruit_MPU6050 mpu;
sensors_event_t event;

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
  // Serial.println("up: ");
  // Serial.println(getUltrasonicDistance(ULTRASONIC_FRONTDOWN_TRIG, ULTRASONIC_FRONTDOWN_ECHO));

  Serial.print("Odometry: ");
  Serial.print("X: ");
  Serial.print(myOdom->getX());
  Serial.print(", Y: ");
  Serial.print(myOdom->getY());
  Serial.print(", Head: ");
  Serial.println(myOdom->getTheta());

  /*
  Serial.print("[");
  Serial.print(millis());
  Serial.print("] X: ");
  Serial.print(event.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(event.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(event.acceleration.z);
  Serial.println(" m/s^2");

  */

  /*
  Serial.print("[");
  Serial.print(millis());
  Serial.print("] X: ");
  Serial.print(event.gyro.x);
  Serial.print(", Y: ");
  Serial.print(event.gyro.y);
  Serial.print(", Z: ");
  Serial.print(event.gyro.z);
  Serial.println(" Gyro position");
*/

  // Serial.print("Heading: ");
  // Serial.println(event.gyro.heading);
  //  Serial.println("up:");
  //  Serial.println(ultrasonic_frontUp->getDistance());
  //  Serial.println("down:");
  //  Serial.println(ultrasonic_frontDown->getDistance());
  //  Serial.println("left:");
  //  Serial.println(ultrasonic_left->getDistance());
  //  Serial.println("right:");
  //  Serial.println(ultrasonic_right->getDistance());
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
  encoder_left = new EncoderSensor(ENCODERSENSOR_LEFT, ENCODERSENSOR_LEFT_ANALOG);
  encoder_left->init();
  encoder_right = new EncoderSensor(ENCODERSENSOR_RIGHT, ENCODERSENSOR_RIGHT_ANALOG);
  encoder_right->init();

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

  // Start Odometry
  myOdom = new Odometry(WHEEL_RADIUS, BASE_LENGTH, TICK_REV);
  //  pinMode(LED_BUILTIN, OUTPUT); // Initialize the digital pin as an output
  myLed = new MyLED(LED_BUILTIN);

  while (!mpu.begin())
  {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }

  // Set up printing with soft timer
  myTimer.createTimer(100, printing, true);
  // Finally start all timers

  myTimer.startAllTimers();

  // Color sensors setup
  colorSensorFront = new ColorSensor(COLOR_F_S0, COLOR_F_S1, COLOR_F_S2, COLOR_F_S3, COLOR_F_OUT);
  colorSensorFront->init();
  colorSensorBack = new ColorSensor(COLOR_B_S0, COLOR_B_S1, COLOR_B_S2, COLOR_B_S3, COLOR_B_OUT);
  colorSensorBack->init();

  // LineFollower setup
  lineFollower = new LineFollower(line_right, motor_right, motor_left);

  // PistaA setup
  // pistaA = new PistaA(motor_intake, motor_left, motor_right, colorSensorFront,
  //                    ULTRASONIC_FRONTDOWN_TRIG, ULTRASONIC_FRONTDOWN_ECHO,
  //                    ULTRASONIC_FRONTUP_TRIG, ULTRASONIC_FRONTUP_ECHO);

  current_time = micros();
}

// Modos para elegir la pista:
enum Mode
{
  MODE_LINE_FOLLOW,
  MODE_PISTA_A,
  MODE_TEST_INTAKE,
  PHOTO_MODE,
  MODE_MOTOR_TEST,
  MODE_COLOR_SENSOR_TEST
};

void loop()
{
  Mode currentMode = MODE_LINE_FOLLOW;

  // Update everything
  float distance = getUltrasonicDistance(ULTRASONIC_FRONTUP_TRIG, ULTRASONIC_FRONTUP_ECHO);
  float distance_down = getUltrasonicDistance(ULTRASONIC_FRONTDOWN_TRIG, ULTRASONIC_FRONTDOWN_ECHO);
  float distance_left = getUltrasonicDistance(ULTRASONIC_LEFT_TRIG, ULTRASONIC_LEFT_ECHO);
  float distance_right = getUltrasonicDistance(ULTRASONIC_RIGHT_TRIG, ULTRASONIC_RIGHT_ECHO);
  // mpu.getAccelerometerSensor()->getEvent(&event);
  // mpu.getGyroSensor()->getEvent(&event);
  // float gyroX = event.gyro.x + GYROX_CORRECTION;
  // float gyroY = event.gyro.y + GYROY_CORRECTION;
  // float gyroZ = event.gyro.z + GYROZ_CORRECTION;
  // myOdom->update(encoder_left->getCount(), encoder_right->getCount(), 0, gyroZ);
  // float distance_down = 0;

  switch (currentMode)
  {
  case MODE_LINE_FOLLOW:
    lineFollower->followLine();
    break;
  case MODE_PISTA_A:
    pistaA->runPistaCompleta();
    break;
  case MODE_TEST_INTAKE:
    if (distance_down > -1.1 && distance_down < 3)
    {
      motor_intake->setSpeed(0);
    }
    else
    {
      motor_intake->setSpeed(250);
    }
    break;
  case PHOTO_MODE:
    myLed->turnOn();
    break;
  case MODE_MOTOR_TEST:
    motor_left->setSpeed(-80);
    motor_right->setSpeed(-80);
    delay(900);
    motor_left->stop();
    motor_right->stop();
    delay(10000);
    break;
  case MODE_COLOR_SENSOR_TEST:
    int r, g, b;
    colorSensorFront->readColor(r, g, b);
    if (g < r && r < b)
    {
      // Color amarillo
      motor_intake->setSpeed(255);
      delay(700);
    }
    else if (b < r && g < r)
    {
      // Color celeste
      motor_intake->setSpeed(180);
      delay(500);
    }
    else
    {
      // No se reconocio nada
      motor_intake->setSpeed(-250);
      delay(500);
    }

    delay(500);
    break;
  default:
    break;
  }
}