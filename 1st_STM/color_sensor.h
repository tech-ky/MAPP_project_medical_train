#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include "mbed.h"

// I2C pins for TCS3472 sensors
#define SDA1_PIN PB_7
#define SCL1_PIN PB_6
#define SDA2_PIN PB_11
#define SCL2_PIN PB_10

// Function Prototypes
void tcs3472_init(I2C &i2c);
void tcs3472_read_data(I2C &i2c, uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c);
int detect_color_hue(float hue, float saturation, float value);
int color_tracker_1();  // Function for sensor 1
int color_tracker_2();  // Function for sensor 2

#endif // COLOR_SENSOR_H
