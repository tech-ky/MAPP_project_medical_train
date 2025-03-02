#include "mbed.h"
#include "lcd.h"
#include "keypad.h"
#include "get_user_input.h"
#include "color_sensor.h"
#include "ultrasonic_sensor.h"
#include "drop_item.h"
#include "motor.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

unsigned char key;
char Message1[] = "Schedule a Delivery?";
char Message2[] = "Press A(Yes)/B(No):";
char Response[] = "Response: ";
char ConfirmYes[] = "Input: Yes";
char ConfirmNo[] = "Input: No ";
char SendDeviceMsg[] = "Send Device?";
char SendDeviceConfirm[] = "Confirm? (A)";
char bedreached[]="bed detected";
char wardreached[]="ward detected";
char endpoint[]="going back ";
char startpoint[]="stopping";
int main() {
    
    lcd_init();
    zero();  

    while (1) {
        lcd_write_cmd(0x80);
        for (int i = 0; i < 20; i++) {
            if (i < sizeof(Message1) - 1) lcd_write_data(Message1[i]);
            else lcd_write_data(' ');
        }

        lcd_write_cmd(0xC0);
        for (int i = 0; i < 20; i++) {
            if (i < sizeof(Message2) - 1) lcd_write_data(Message2[i]);
            else lcd_write_data(' ');
        }

        key = getkey();
        lcd_write_cmd(0x01);
        lcd_write_cmd(0x80);

        for (int i = 0; i < 20; i++) {
            if (i < sizeof(Response) - 1) lcd_write_data(Response[i]);
            else lcd_write_data(' ');
        }

        lcd_write_cmd(0xC0);
        lcd_write_data(key);

        if (key == 'A' || key == 'a') {
            lcd_write_cmd(0xC0);
            for (int i = 0; i < 20; i++) {
                if (i < sizeof(ConfirmYes) - 1) lcd_write_data(ConfirmYes[i]);
                else lcd_write_data(' ');
            }

            get_user_input();
            printf("Contents of list2:\n");
            for (const auto &wardEntry : list2) {
                printf("Ward %d: ", wardEntry.first);
                for (const std::string &patientInfo : wardEntry.second) {
                    printf("%s ", patientInfo.c_str());
                }
                printf("\n");
            }

            lcd_write_cmd(0x01);
            lcd_write_cmd(0x80);
            for (int i = 0; i < sizeof(SendDeviceMsg) - 1; i++) lcd_write_data(SendDeviceMsg[i]);

            lcd_write_cmd(0xC0);
            for (int i = 0; i < sizeof(SendDeviceConfirm) - 1; i++) lcd_write_data(SendDeviceConfirm[i]);

            key = getkey();
            lcd_write_cmd(0x01);
            lcd_write_cmd(0x80);
            lcd_write_data(key);

            if (key == 'A' || key == 'a') {
                float front_distance = -1.0, back_distance = -1.0;
                int colour = -1;
                bool forward_flag = true;

                while (true) {
                    if (forward_flag) {
                        front_distance = measure_distance_1();
                        printf("motor moving forward"); 
                        move_forward();

                        printf("Front Sensor Distance: %.2f cm\n", front_distance);

                        if (front_distance > 15 || front_distance == -1) {
                            thread_sleep_for(1000);
                            colour = color_tracker_1();
                            printf("Sensor 1 - Detected colour value: %d\n", colour);

                            auto wardIter = list2.find(colour);
                            if (wardIter != list2.end()) {
                                printf("Found ward %d. Processing beds...\n", colour);
                                    lcd_write_cmd(0xC0);
                                    for (int i = 0; i < 20; i++) {
                                        if (i < sizeof(wardreached) - 1) lcd_write_data(wardreached[i]);
                                        else lcd_write_data(' ');
                                    }
                                std::map<int, int> bedToPatientMap;
                                std::vector<std::string> &wardPatients = wardIter->second;

                                for (const std::string &patientInfo : wardPatients) {
                                    int patientNum = std::stoi(patientInfo.substr(1, patientInfo.find(":") - 1));  
                                    int bedNum = std::stoi(patientInfo.substr(patientInfo.find(":bed") + 4));     
                                    bedToPatientMap[bedNum] = patientNum;
                                }

                                while (!bedToPatientMap.empty()) {
                                    printf("Searching for patient beds...\n");

                                    int detected_bed_colour = color_tracker_2();
                                    printf("Detected bed color: %d\n", detected_bed_colour);

                                    auto it = bedToPatientMap.begin();
                                    while (it != bedToPatientMap.end()) {
                                        int bed = it->first;
                                        int patient = it->second;

                                        if (detected_bed_colour == colour) {
                                            lcd_write_cmd(0xC0);
                                            for (int i = 0; i < 20; i++) {
                                                if (i < sizeof(bedreached) - 1) lcd_write_data(bedreached[i]);
                                                else lcd_write_data(' ');
                                            }
                                            printf("Patient %d's bed %d found. Stopping to deliver medication.\n", patient, bed);
                                            stop_motor(); 
                                            printf("motor stop");

                                           
                                            printf("Moving servo to patient %d position.\n", patient);
                                            moveServo(patient, true);
                                            thread_sleep_for(1000);  
                                            printf("Opening servo to drop item.\n");
                                            drop_item();
                                            thread_sleep_for(2000);

                                            printf("Closing drop servo.\n");
                                            zero();
                                            thread_sleep_for(2000);

                                            printf("Returning servo to original position.\n");
                                            moveServo(patient, false);
                                            thread_sleep_for(2000);

                                            it = bedToPatientMap.erase(it);
                                        } else {
                                            printf(" Bed %d not found yet. Continuing search...\n", bed);
                                            ++it;
                                        }
                                    }
                                    thread_sleep_for(1000);
                                }
                                printf(" All beds processed for Ward %d. Moving to the next ward.\n", colour);
                            } else {
                                printf("Ward %d not found. Skipping...\n", colour);
                            }
                        } else {
                            lcd_write_cmd(0xC0);
                            for (int i = 0; i < 20; i++) {
                                if (i < sizeof(endpoint) - 1) lcd_write_data(endpoint[i]);
                                else lcd_write_data(' ');
                            }
                            printf(" Object detected within 5cm. Stopping forward movement.\n");
                            stop_motor();
                            forward_flag = false;
                        }
                    } else {
                        back_distance = measure_distance_2();
                        printf("Back Sensor Distance: %.2f cm\n", back_distance);
                        lcd_write_cmd(0xC0);
                        for (int i = 0; i < 20; i++) {
                            if (i < sizeof(startpoint) - 1) lcd_write_data(startpoint[i]);
                            else lcd_write_data(' ');
                        }
                        if (back_distance > 10 || back_distance == -1) {
                            printf("Device moving back\n");
                            move_backward();
                        } else {
                            printf(" Device stopped moving\n");
                            stop_motor();
                            break;
                        }
                    }
                }
            } else if (key == 'B' || key == 'b') {
                lcd_write_cmd(0x01);
                lcd_write_cmd(0x80);
                for (int i = 0; i < sizeof(ConfirmNo) - 1; i++) lcd_write_data(ConfirmNo[i]);
                thread_sleep_for(1000);
            }
            thread_sleep_for(2000);
            lcd_write_cmd(0x01);
        }
    }
}
