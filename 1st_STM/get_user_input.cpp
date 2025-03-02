#include "mbed.h"
#include "lcd.h"
#include "keypad.h"
#include "get_user_input.h"
#include <map>
#include <vector>
#include <string>
#include <cstdio> 
unsigned char userInputKey;
char NumPatientsMsg[] = "Enter No. Patients:";
char WardMsgTemplate[] = "P%d: Enter Ward No:";
char BedMsgTemplate[] = "P%d: Enter Bed No:";
char ConfirmMsg[] = "Press A to Confirm";
char CompleteMsg[] = "Details Saved!";

char wardNumbers[8][4];  
char bedNumbers[8][4];  
int ContainerPos[8];

std::map<int, std::vector<std::string>> list2;

std::vector<std::string> get_user_input() {
    int numPatients = 0;
    char dynamicMsg[20];

    lcd_init();
    while (true) {
        lcd_write_cmd(0x80);
        for (int i = 0; i < 20; i++) {
            lcd_write_data(i < sizeof(NumPatientsMsg) - 1 ? NumPatientsMsg[i] : ' ');
        }

        lcd_write_cmd(0xC0);
        userInputKey = getkey();
        numPatients = userInputKey - '0';
        lcd_write_data(userInputKey);
        thread_sleep_for(1000);
        lcd_write_cmd(0x01);

        if (numPatients >= 1 && numPatients <= 8) {
            break;
        }

        lcd_write_cmd(0x80);
        char errorMsg[] = "Invalid! Try Again";
        for (int i = 0; i < sizeof(errorMsg) - 1; i++) {
            lcd_write_data(errorMsg[i]);
        }
        thread_sleep_for(2000);
        lcd_write_cmd(0x01);
    }
    for (int patient = 0; patient < numPatients; patient++) {
        int index;

        while (true) {
            snprintf(dynamicMsg, sizeof(dynamicMsg), WardMsgTemplate, patient + 1);
            lcd_write_cmd(0x80);
            for (int i = 0; i < 20; i++) {
                lcd_write_data(i < strlen(dynamicMsg) ? dynamicMsg[i] : ' ');
            }

            lcd_write_cmd(0xC0);
            index = 0;
            while (true) {
                userInputKey = getkey();
                if (userInputKey == 'A') {
                    wardNumbers[patient][index] = '\0';
                    break;
                } else if (userInputKey == 'B' && index > 0) {
                    index--;
                    lcd_write_cmd(0xC0 + index);
                    lcd_write_data(' ');
                    lcd_write_cmd(0xC0 + index);
                } else if (index < 3 && userInputKey >= '0' && userInputKey <= '9') {
                    wardNumbers[patient][index++] = userInputKey;
                    lcd_write_data(userInputKey);
                }
            }
            break;
        }

        lcd_write_cmd(0x01);
        while (true) {
            snprintf(dynamicMsg, sizeof(dynamicMsg), BedMsgTemplate, patient + 1);
            lcd_write_cmd(0x80);
            for (int i = 0; i < 20; i++) {
                lcd_write_data(i < strlen(dynamicMsg) ? dynamicMsg[i] : ' ');
            }

            lcd_write_cmd(0xC0);
            index = 0;
            while (true) {
                userInputKey = getkey();
                if (userInputKey == 'A') {
                    bedNumbers[patient][index] = '\0';
                    break;
                } else if (userInputKey == 'B' && index > 0) {
                    index--;
                    lcd_write_cmd(0xC0 + index);
                    lcd_write_data(' ');
                    lcd_write_cmd(0xC0 + index);
                } else if (index < 3 && userInputKey >= '0' && userInputKey <= '9') {
                    bedNumbers[patient][index++] = userInputKey;
                    lcd_write_data(userInputKey);
                }
            }
            break;
        }

        lcd_write_cmd(0x01);
        ContainerPos[patient] = patient;
    }

    for (int i = 0; i < numPatients; i++) {
        int ward = atoi(wardNumbers[i]); 
        printf("Patient %d: Ward %d\n", i + 1, ward); 

        char patientBed[20];
        snprintf(patientBed, sizeof(patientBed), "p%d:bed%s", i + 1, bedNumbers[i]);

        list2[ward].push_back(patientBed);
    }

    std::vector<std::string> sortedPatients;
    for (auto& wardEntry : list2) {
        std::sort(wardEntry.second.begin(), wardEntry.second.end());  
        for (const std::string& patientInfo : wardEntry.second) {
            sortedPatients.push_back(patientInfo);
        }
    }

    printf("Sorted patient-bed information:\n");
    for (const std::string& patientInfo : sortedPatients) {
        printf("%s ", patientInfo.c_str());
    }
    printf("\n");

    return sortedPatients;  
}
