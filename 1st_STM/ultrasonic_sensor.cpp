#include "mbed.h"

DigitalOut Trig1(PC_1);
DigitalIn Echo1(PC_2);

DigitalOut Trig2(PC_5);
DigitalIn Echo2(PC_6);

Timer timer;

float measure_distance_1() {
    float totalDistance = 0.0f;
    int validReadings = 0;
    
    for (int i = 0; i < 3; i++) {
        Trig1 = 0;
        thread_sleep_for(30);
        
        Trig1 = 1;
        wait_us(10);
        Trig1 = 0;
        
        timer.reset();
        timer.start();
        while (Echo1 == 0) {
            if (timer.read_us() > 30000) {
                printf("Measure 1: Echo timeout on attempt %d\n", i + 1);
                break;
            }
        }
        
        timer.reset();
        while (Echo1 == 1) {
            if (timer.read_us() > 30000) {
                printf("Measure 1: Echo pulse too long on attempt %d\n", i + 1);
                break;
            }
        }
        timer.stop();
        
        float duration = timer.read_us();
        float objDistance = (duration * 0.03432) / 2;
        
        if (objDistance > 0) {
            totalDistance += objDistance;
            validReadings++;
        }
        
        thread_sleep_for(50);
    }
    
    if (validReadings == 0) {
        printf("Measure 1: No valid readings\n");
        return -1.0f;
    }
    
    return totalDistance / validReadings;
}

float measure_distance_2() {
    float totalDistance = 0.0f;
    int validReadings = 0;
    
    for (int i = 0; i < 3; i++) {
        Trig2 = 0;
        thread_sleep_for(30);
        
        Trig2 = 1;
        wait_us(10);
        Trig2 = 0;
        
        timer.reset();
        timer.start();
        while (Echo2 == 0) {
            if (timer.read_us() > 30000) {
                printf("Measure 2: Echo timeout on attempt %d\n", i + 1);
                break;
            }
        }
        
        timer.reset();
        while (Echo2 == 1) {
            if (timer.read_us() > 30000) {
                printf("Measure 2: Echo pulse too long on attempt %d\n", i + 1);
                break;
            }
        }
        timer.stop();
        
        float duration = timer.read_us();
        float objDistance = (duration * 0.03432) / 2;
        
        if (objDistance > 0) {
            totalDistance += objDistance;
            validReadings++;
        }
        
        thread_sleep_for(50);
    }
    
    if (validReadings == 0) {
        printf("Measure 2: No valid readings\n");
        return -1.0f;
    }
    
    return totalDistance / validReadings;
}


