#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

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
#define I2C_SDA 22
#define I2C_SCL 26

// Sensores Ultrasonicos
#define ULTRASONIC1_TRIG 28 // Trigger pin
#define ULTRASONIC1_ECHO 27 // Echo pin

#define ULTRASONIC2_TRIG 21
#define ULTRASONIC2_ECHO 20

#define ULTRASONIC3_TRIG 19
#define ULTRASONIC3_ECHO 18

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
#define ENCODER2 15 // GPIO15 → LM393 #2 OUT

// Led de estatus
#define STATUS_LED 25 // GPIO27 → LED indicator

// Setup callback function to blink status led
volatile bool led_state = false;
bool status_led_callback(repeating_timer_t *rt)
{
    led_state = !led_state;          // Toggle LED state
    gpio_put(STATUS_LED, led_state); // Apply new state to LED
    return true;                     // true = keep repeating, false = stop timer
}

// Setup counters for encoders and their interrupt handlers.
volatile uint32_t counter1 = 0;
volatile uint32_t counter2 = 0;
void sensor1_irq(uint gpio, uint32_t events)
{
    counter1++;
}
void sensor2_irq(uint gpio, uint32_t events)
{
    counter2++;
}

// Setup color sensor measurement functions.
volatile uint32_t color_count1 = 0;
volatile uint32_t red_val1 = 0, green_val1 = 0, blue_val1 = 0;
void color_irq1(uint gpio, uint32_t events)
{
    color_count1++;
}
volatile uint32_t color_count2 = 0;
volatile uint32_t red_val2 = 0, green_val2 = 0, blue_val2 = 0;
void color_irq2(uint gpio, uint32_t events)
{
    color_count2++;
}
volatile int current_filter = 0;

// Timer that callbacks the values of both color sensors
bool color_timer_callback(repeating_timer_t *rt)
{
    uint32_t freq1 = color_count1 * 10; // convert pulses/100ms → Hz
    uint32_t freq2 = color_count2 * 10;
    color_count1 = 0;
    color_count2 = 0;
    if (current_filter == 0)
    {
        // Setup next color filter (Blue) in sensor 1
        red_val1 = freq1;
        gpio_put(COLOR1_S2, 0);
        gpio_put(COLOR1_S3, 1);

        // Check next color filter (Blue) in sensor 2
        red_val2 = freq2;
        gpio_put(COLOR2_S2, 0);
        gpio_put(COLOR2_S3, 1);

        current_filter = 1;
    }
    else if (current_filter == 1)
    {
        blue_val1 = freq1;
        gpio_put(COLOR1_S2, 1);
        gpio_put(COLOR1_S3, 1);

        blue_val2 = freq2;
        gpio_put(COLOR2_S2, 1);
        gpio_put(COLOR2_S3, 1);
        current_filter = 2;
    }
    else
    {
        green_val1 = freq1;
        gpio_put(COLOR1_S2, 0);
        gpio_put(COLOR1_S3, 0);

        green_val2 = freq2;
        gpio_put(COLOR2_S2, 0);
        gpio_put(COLOR2_S3, 0);
        current_filter = 0;

        // Terminal printing of the full RGB measurement
        printf("First Color Sensor: R: %lu  G: %lu  B: %lu\n", red_val1, green_val1, blue_val1);
        printf("Second Color Sensor: R: %lu  G: %lu  B: %lu\n", red_val2, green_val2, blue_val2);
    }

    return true; // repeat
}

// Setup PIR interrupts
volatile uint32_t motion = 0;
void pir_irq(uint gpio, uint32_t events)
{
    motion = gpio_get(gpio);
    // Terminal Printing for motion
    if (motion)
    {
        printf("Motion detected!\n");
    }
    else
    {
        printf("Motion ended.\n");
    }
}

void setup()
{
}

int main()
{
    stdio_init_all();

    // Init Status Led and setup a repeating timer that blinks the main led of the Rasp Pico
    gpio_init(STATUS_LED);
    gpio_set_dir(STATUS_LED, GPIO_OUT);
    repeating_timer_t led_timer;
    add_repeating_timer_us(-100000, status_led_callback, NULL, &led_timer);

    // Init Encoder GPIOs and attach interrupts
    gpio_init(ENCODER1);
    gpio_set_dir(ENCODER1, GPIO_IN);
    gpio_pull_up(ENCODER1);
    gpio_init(ENCODER2);
    gpio_set_dir(ENCODER2, GPIO_IN);
    gpio_pull_up(ENCODER2);
    gpio_set_irq_enabled_with_callback(ENCODER1, GPIO_IRQ_EDGE_FALL, true, &sensor1_irq);
    gpio_set_irq_enabled_with_callback(ENCODER2, GPIO_IRQ_EDGE_FALL, true, &sensor2_irq);

    // Init Color GPIOs and attach interrupts
    gpio_init(COLOR1_S0);
    gpio_set_dir(COLOR1_S0, GPIO_OUT);
    gpio_init(COLOR1_S1);
    gpio_set_dir(COLOR1_S1, GPIO_OUT);
    gpio_init(COLOR1_S2);
    gpio_set_dir(COLOR1_S2, GPIO_OUT);
    gpio_init(COLOR1_S3);
    gpio_set_dir(COLOR1_S3, GPIO_OUT);
    gpio_put(COLOR1_S0, 1);
    gpio_put(COLOR1_S1, 0);
    gpio_init(COLOR1_OUT);
    gpio_set_dir(COLOR1_OUT, GPIO_IN);
    gpio_set_irq_enabled_with_callback(COLOR1_OUT, GPIO_IRQ_EDGE_RISE, true, &color_irq1);
    gpio_init(COLOR2_S0);
    gpio_set_dir(COLOR2_S0, GPIO_OUT);
    gpio_init(COLOR2_S1);
    gpio_set_dir(COLOR2_S1, GPIO_OUT);
    gpio_init(COLOR2_S2);
    gpio_set_dir(COLOR2_S2, GPIO_OUT);
    gpio_init(COLOR2_S3);
    gpio_set_dir(COLOR2_S3, GPIO_OUT);
    gpio_put(COLOR2_S0, 1);
    gpio_put(COLOR2_S1, 0);
    gpio_init(COLOR2_OUT);
    gpio_set_dir(COLOR2_OUT, GPIO_IN);
    gpio_set_irq_enabled_with_callback(COLOR2_OUT, GPIO_IRQ_EDGE_RISE, true, &color_irq2);
    repeating_timer_t color_timer;
    add_repeating_timer_ms(100, color_timer_callback, NULL, &color_timer);

    // Init PIR Sensor
    gpio_init(PIR_OUT);
    gpio_set_dir(PIR_OUT, GPIO_IN);
    gpio_set_irq_enabled_with_callback(PIR_OUT, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &pir_irq);

    // Init Line Sensors
    gpio_init(LINE_SENSOR1);
    gpio_set_dir(LINE_SENSOR1, GPIO_IN);

    gpio_init(LINE_SENSOR2);
    gpio_set_dir(LINE_SENSOR2, GPIO_IN);

    printf("Program started.\n");
    while (true)
    {
        tight_loop_contents();
    }
}
