
#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"

extern DigitalOut IN1;
extern DigitalOut IN2;

void move_forward();
void move_backward();
void stop_motor();

#endif 
