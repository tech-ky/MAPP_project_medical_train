
#undef __ARM_FP

#include "mbed.h"
#include "lcd.h"	

#define DISPLAY_LCD_MASK 0x00000F00 
#define DISPLAY_LCD_RESET 0x00000000

PortOut lcdPort(PortA, DISPLAY_LCD_MASK);
DigitalOut LCD_RS(PA_14); 
DigitalOut LCD_EN(PA_12);
DigitalOut LCD_WR(PA_13);  

void lcd_strobe(void);

void lcd_write_cmd(unsigned char cmd)
{
    unsigned char temp2;
    int tempLCDPort = 0;

    LCD_RS = 0;					
    wait_us(40);				
    temp2 = cmd;
    temp2 = temp2 >> 4;			
    temp2 = temp2 & 0x0F;
                        		
    tempLCDPort = (int) temp2;
    tempLCDPort =  tempLCDPort << 8;
    tempLCDPort = tempLCDPort & 0x00000F00;
    lcdPort = tempLCDPort;


    wait_us(10000);			
    lcd_strobe();
    
	wait_us(10000);			

    temp2 = cmd;			
    temp2 = temp2 & 0x0F;	
    
    tempLCDPort = (int) temp2;
    tempLCDPort =  tempLCDPort << 8;
    tempLCDPort = tempLCDPort & 0x00000F00;
    lcdPort = tempLCDPort;

    wait_us(10000);			
    lcd_strobe();
    wait_us(10000);			

}
void lcd_write_data(char data)
{
  	char temp1;
    int tempLCDPort = 0;

    LCD_RS = 1;					
    wait_us(40);				

    temp1 = data;
    temp1 = temp1 >> 4;
    temp1 = temp1 & 0x0F;

    tempLCDPort = (int) temp1;
    tempLCDPort =  tempLCDPort << 8;
    tempLCDPort = tempLCDPort & 0x00000F00;
    lcdPort = tempLCDPort;

	wait_us(10000); 
   	LCD_RS = 1;
    wait_us(10000);			

    lcd_strobe();
    wait_us(10000);

    temp1 = data;
    temp1 = temp1 & 0x0F;
    tempLCDPort = (int) temp1;  
    tempLCDPort =  tempLCDPort << 8;
    tempLCDPort = tempLCDPort & 0x00000F00;
    lcdPort = tempLCDPort;

    wait_us(10000);
	LCD_RS = 1;
    wait_us(10000); 			

    lcd_strobe();	
    wait_us(10000);
}

void lcd_strobe(void)			
{
    LCD_EN = 1;					
    wait_us(10000);		
    LCD_EN = 0;				
    wait_us(10000);			
}

void lcd_init(void)
{
    lcdPort = DISPLAY_LCD_RESET;			
    LCD_EN = 0;
    LCD_RS = 0;					
    LCD_WR = 0;				

    thread_sleep_for(1000);

    lcd_write_cmd(0x33);
    lcd_write_cmd(0x32);
      
    lcd_write_cmd(0x28);		
   
    lcd_write_cmd(0x0E);		
   
    lcd_write_cmd(0x06);	
   
    lcd_write_cmd(0x01);		
    thread_sleep_for(20);		

}

void lcd_Clear(void)
{
    lcd_write_cmd(0x01);		
 
    thread_sleep_for(20);			

}