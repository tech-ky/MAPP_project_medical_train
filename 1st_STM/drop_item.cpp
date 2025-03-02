#include "drop_item.h"

PwmOut motor(PA_1);  // Example pin for first servo
PwmOut servo(PA_6);  // Example pin for second servo

// Move servo based on patient position and rotation direction
void moveServo(int patient, bool rotation) {
    for (ServoConfig config : servoPositions) {
        if (config.patient == patient) {
            int pulseWidth = rotation ? config.pulseWidthCW : config.pulseWidthACW;
            motor.pulsewidth_us(pulseWidth);
            printf("Servo moving %s for Patient %d\n", rotation ? "CW" : "ACW", patient);
            thread_sleep_for(config.waitTime);
            motor.pulsewidth_us(STOP_PULSE_WIDTH);
            return;
        }
    }
    printf("Invalid patient ID: %d\n", patient);
}

// Test function to move each position CW and ACW
void testServo() {
    for (ServoConfig config : servoPositions) {
        moveServo(config.patient, true);  // Move CW
        thread_sleep_for(500);
        moveServo(config.patient, false); // Move ACW
        thread_sleep_for(500);
    }
    printf("Testing complete\n");
}

// Function to drop an item
void drop_item() {
    servo.period_ms(PERIOD_MS);  // Set PWM period

    // Open the servo (Clockwise rotation)
    servo.pulsewidth_us(TURN_TO_OPEN);
    printf("Dropping item: Servo opening\n");
    thread_sleep_for(WAIT_TIME_MS);

    // Close the servo (Counterclockwise rotation)
    servo.pulsewidth_us(GO_BACK);
    printf("Servo closing\n");
    thread_sleep_for(WAIT_TIME_MS);
}

// Reset function for the drop servo
void zero() {
    servo.pulsewidth_us(GO_BACK);
    printf("Servo closing\n");
    thread_sleep_for(1000);
}
