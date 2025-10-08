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