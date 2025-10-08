#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "InterruptManager.h"
#include "PrintTerminal.h"
#include "Sensors/Sensors.h"
#include "Motors/Motor.h"

#define WHEEL_RADIOUS 3.25
#define WHEEL_BASE 20

// Pin definitions

// Motor Drivers and IMU
// P0 → IMU_SDA
// P1 → IMU_SCL
// P2 → Motor_Left_ENA (PWM)
// P3 → Motor_Left_IN1
// P4 → Motor_Left_IN2
// P5 → Motor_Right_IN3
// P6 → Motor_Right_IN4
// P7 → Motor_Right_ENB (PWM)
#define I2C_PORT i2c0
#define I2C_SDA 22
#define I2C_SCL 26

// Sensores Ultrasonicos
#define ULTRASONIC1_TRIG 28 // Trigger pin
#define ULTRASONIC1_ECHO 27 // Echo pin

#define ULTRASONIC2_TRIG 21
#define ULTRASONIC2_ECHO 20

#define ULTRASONIC3_TRIG 19 // GPIO19 →
#define ULTRASONIC3_ECHO 18 // GPIO18 →

#define ULTRASONIC4_TRIG 17
#define ULTRASONIC4_ECHO 16

// Sensores para linea
#define LINE_SENSOR1 1 // GPIO1 → TCRT5000 #1 OUT
#define LINE_SENSOR2 2 // GPIO2 → TCRT5000 #2 OUT

// Sensor PIR
#define PIR_OUT 0 // GPIO0 → PIR sensor output

// Sensor de Color TCS3200
#define COLOR1_S0 3
#define COLOR1_S1 4
#define COLOR1_S2 5
#define COLOR1_S3 6
#define COLOR1_OUT 7

// Sensor de Color TCS230
#define COLOR2_S0 12
#define COLOR2_S1 11
#define COLOR2_S2 9
#define COLOR2_S3 8
#define COLOR2_OUT 14

// Encoders
#define ENCODER1 13 // GPIO13 → LM393 #1 OUT
#define ENCODER2 14 // GPIO15 → LM393 #2 OUT

// Led de estatus
#define STATUS_LED 25 // GPIO27 → LED indicator

#define MOTOR_INTAKE_ENA 2
#define MOTOR_INTAKE_IN1 3
#define MOTOR_INTAKE_IN2 4

// Setup callback function to blink status led

volatile bool led_state = false;
bool status_led_callback(repeating_timer_t *rt)
{
    led_state = !led_state;          // Toggle LED state
    gpio_put(STATUS_LED, led_state); // Apply new state to LED
    return true;                     // true = keep repeating, false = stop timer
}

// Setup PIR interrupts
volatile uint32_t motion = 0;
void pir_irq(uint gpio, uint32_t events)
{

    motion = gpio_get(gpio);

    printf("Motion has been detected.\n");
    // Terminal Printing for motion
}

bool print_to_terminal_callback(repeating_timer_t *rt)
{
    printf("Running...");
    return true;
};

int main()
{
    stdio_init_all();

    // Init Status Led and setup a repeating timer that blinks the main led of the Rasp Pico
    gpio_init(STATUS_LED);
    gpio_set_dir(STATUS_LED, GPIO_OUT);
    repeating_timer_t led_timer;
    add_repeating_timer_us(-100000, status_led_callback, NULL, &led_timer);

    // Init Encoder GPIOs and attach interrupts
    EncoderSensor encoder_sensor1 = EncoderSensor(ENCODER1);
    EncoderSensor encoder_sensor2 = EncoderSensor(ENCODER2);
    encoder_sensor1.init();
    encoder_sensor2.init();

    // Init Color GPIOs
    /*
    ColorSensor color_sensor1 = ColorSensor(COLOR1_S0, COLOR1_S1, COLOR1_S2, COLOR1_S3, COLOR1_OUT);
    ColorSensor color_sensor2 = ColorSensor(COLOR2_S0, COLOR2_S1, COLOR2_S2, COLOR2_S3, COLOR2_OUT);
    color_sensor1.init();
    color_sensor2.init();
    */
    ColorSensor::startTimerManager();

    // Init PIR Sensor
    gpio_init(PIR_OUT);
    gpio_set_dir(PIR_OUT, GPIO_IN);
    InterruptManager::attach(PIR_OUT, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, pir_irq);

    // Init Line Sensors
    LineSensor line_sensor1 = LineSensor(LINE_SENSOR1);
    LineSensor line_sensor2 = LineSensor(LINE_SENSOR2);
    line_sensor1.init();
    line_sensor2.init();

    // Init Ultrasonic Sensors
    UltrasonicSensor ultra_sensor1 = UltrasonicSensor(ULTRASONIC1_TRIG, ULTRASONIC1_ECHO);
    UltrasonicSensor ultra_sensor2 = UltrasonicSensor(ULTRASONIC2_TRIG, ULTRASONIC2_ECHO);
    UltrasonicSensor ultra_sensor3 = UltrasonicSensor(ULTRASONIC3_TRIG, ULTRASONIC3_ECHO);
    UltrasonicSensor ultra_sensor4 = UltrasonicSensor(ULTRASONIC4_TRIG, ULTRASONIC4_ECHO);
    ultra_sensor1.init();
    ultra_sensor2.init();
    ultra_sensor3.init();
    ultra_sensor4.init();
    UltrasonicSensor::startTimerManager();

    // Intake motor
    Motor motor_intake = Motor(MOTOR_INTAKE_ENA, MOTOR_INTAKE_IN1, MOTOR_INTAKE_IN2);
    motor_intake.init();
    motor_intake.forward();
    motor_intake.setSpeed(50);

    // Init print to terminal
    /*
    PrintTerminal printer = PrintTerminal();
    */
    /*
    printer.attachSensor(&encoder_sensor1);
    printer.attachSensor(&encoder_sensor2);
    */
    /*
    printer.attachSensor(&color_sensor1);
    printer.attachSensor(&color_sensor2);
    printer.attachSensor(&line_sensor1);
    printer.attachSensor(&line_sensor2);

    printer.attachSensor(&ultra_sensor1);
    printer.attachSensor(&ultra_sensor2);
    printer.attachSensor(&ultra_sensor3);
    printer.attachSensor(&ultra_sensor4);
    */
    /*
    printer.startTimer();
    */
    // Setup printing to terminal through timer
    /*
    repeating_timer_t terminal_timer;
    add_repeating_timer_ms(1000, print_to_terminal_callback, NULL, &terminal_timer);
    */

    printf("Program started.\n");
    while (true)
    {
        tight_loop_contents();
    }
}
