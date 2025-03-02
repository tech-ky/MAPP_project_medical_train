#undef __ARM_FP

#include "mbed.h"

BusIn Keypad_Data(PC_8, PC_9, PC_10, PC_11);

DigitalIn Keypad_DA (PC_13); //74C922 DA output

const unsigned char lookupTable[] = {'1', '2','3','F','4','5','6','E','7','8','9','D','A','0','B','C'};

char getkey(void)
{	
    Keypad_Data.mode(PullNone);

    char keycode = 0;
     
    while (Keypad_DA == 0);  
    keycode = Keypad_Data & Keypad_Data.mask();    

    printf("Key pressed: 0x%04X \n", keycode);

    while (Keypad_DA == 1); 
    return (lookupTable[keycode]);
    
}
