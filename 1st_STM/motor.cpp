#include "motor.h"

DigitalOut IN1(PB_0); 
DigitalOut IN2(PB_1); 
void move_forward() {
    IN1 = 1; 
    IN2 = 0;  
}

void move_backward() {
    IN1 = 0;  
    IN2 = 1;  
}

void stop_motor() {
    IN1 = 0;  
    IN2 = 0;  
}
