#include "color_sensor.h"
#include <cstdio>
#include <cstring>
#include <cmath>

// TCS3472 I2C address
#define TCS3472_ADDR (0x29 << 1)

// TCS3472 registers
#define ENABLE_REG 0x80
#define ATIME_REG 0x81
#define CONTROL_REG 0x8F
#define CDATA_REG 0x94

// I2C objects for two sensors
I2C i2c1(SDA1_PIN, SCL1_PIN);
I2C i2c2(SDA2_PIN, SCL2_PIN);

// Common function to track color using a given I2C sensor
int color_tracker(I2C &i2c) {
    uint16_t red, green, blue, clear;
    uint16_t red_sum = 0, green_sum = 0, blue_sum = 0;

    // Initialize the sensor
    tcs3472_init(i2c);
    printf("TCS3472 RGB sensor initialized.\n");

    // Take 3 readings with a short delay
    for (int i = 0; i < 3; i++) {
        tcs3472_read_data(i2c, red, green, blue, clear);
        
        // Accumulate values
        red_sum += red;
        green_sum += green;
        blue_sum += blue;
        
        // Print individual readings
        printf("Reading %d -> R: %u, G: %u, B: %u, Clear: %u\n", i + 1, red, green, blue, clear);
        
        thread_sleep_for(300);  // Short delay before next reading
    }

    // Compute average values
    float red_avg = red_sum / 3.0;
    float green_avg = green_sum / 3.0;
    float blue_avg = blue_sum / 3.0;

    // Normalize RGB values
    float sum = red_avg + green_avg + blue_avg;
    float r = red_avg / sum, g = green_avg / sum, b = blue_avg / sum;

    // Convert to HSV
    float hue, saturation, value;
    float min_val = fminf(r, fminf(g, b));
    float max_val = fmaxf(r, fmaxf(g, b));
    float delta = max_val - min_val;

    if (delta == 0) hue = 0;
    else if (max_val == r) hue = 60 * fmod(((g - b) / delta), 6);
    else if (max_val == g) hue = 60 * (((b - r) / delta) + 2);
    else hue = 60 * (((r - g) / delta) + 4);
    if (hue < 0) hue += 360;

    saturation = (max_val == 0) ? 0 : (delta / max_val);
    value = max_val;

    // Return color based on hue
    return detect_color_hue(hue, saturation, value);
}

// Function to track color using Sensor 1 (I2C1)
int color_tracker_1() {
    return color_tracker(i2c1);
}

// Function to track color using Sensor 2 (I2C2)
int color_tracker_2() {
    return color_tracker(i2c2);
}

// Initialize the TCS3472 sensor
void tcs3472_init(I2C &i2c) {
    char cmd[2];
    cmd[0] = ENABLE_REG;
    cmd[1] = 0x03;
    i2c.write(TCS3472_ADDR, cmd, 2);
    cmd[0] = ATIME_REG;
    cmd[1] = 0x00;
    i2c.write(TCS3472_ADDR, cmd, 2);
    cmd[0] = CONTROL_REG;
    cmd[1] = 0x01;
    i2c.write(TCS3472_ADDR, cmd, 2);
}

// Read RGB and clear data from TCS3472
void tcs3472_read_data(I2C &i2c, uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c) {
    char reg = CDATA_REG | 0x80;
    char data[8];
    i2c.write(TCS3472_ADDR, &reg, 1);
    i2c.read(TCS3472_ADDR, data, 8);
    c = data[1] << 8 | data[0];
    r = data[3] << 8 | data[2];
    g = data[5] << 8 | data[4];
    b = data[7] << 8 | data[6];
}

// Detect color based on hue
int detect_color_hue(float hue, float saturation, float value) {
    if (value < 0.1) return 7;  // Black (or very dark color)
    if (saturation < 0.2) return 7;  // Gray

    // Return the corresponding integer for each color
    if (hue >= 0 && hue < 30) return 1;  // Red
    if (hue >= 30 && hue < 90) return 2;  // Yellow
    if (hue >= 90 && hue < 150) return 3;  // Green
    if (hue >= 150 && hue < 210) return 4;  // Cyan
    if (hue >= 210 && hue < 270) return 5;  // Blue
    if (hue >= 270 && hue < 330) return 6;  // Magenta

    return 1;  // Default to Red (if hue is undefined)
}
