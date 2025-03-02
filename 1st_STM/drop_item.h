#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include "mbed.h"

// Constants for Servo Motor - Adjust these values as needed
#define PERIOD_MS 20       
#define TURN_TO_OPEN 2500 
#define STOP_PULSE_WIDTH 1500  
#define GO_BACK 1300    
#define WAIT_TIME_MS 3000  

struct ServoConfig {
    int patient;
    int waitTime;
    int pulseWidthCW;
    int pulseWidthACW;
};

// Define movement settings for 8 patient positions
const ServoConfig servoPositions[] = {
    {1, 240, 1550, 1430},
    {2, 450, 1550, 1430},
    {3, 690, 1550, 1430},
    {4, 1020, 1550, 1430},
    {5, 1200, 1550, 1430},
    {6, 1440, 1550, 1430},
    {7, 1680, 1550, 1430},
};

// Declare the PWM pin for servo control
extern PwmOut motor;
extern PwmOut servo;  

// Function declarations
void moveServo(int patient, bool rotation);  
void testServo();                            
void drop_item();                           
void zero();                              

#endif 
