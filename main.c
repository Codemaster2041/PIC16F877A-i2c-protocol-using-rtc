#include <16F877A.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES PUT                      //Power Up Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=4MHz)
#use i2c(Master,Fast,sda=PIN_C4,scl=PIN_C3)


#include <main.h>                     //initializing lcd pins
#define LCD_ENABLE_PIN PIN_D2
#define LCD_RS_PIN PIN_D0
#define LCD_RW_PIN PIN_D1
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7

#include <lcd.c>

byte sec=0x12,min=0x01,hr=0x03,info; //initializing time

byte read_operation(byte address)
{
   i2c_start();               //start of i2c
   i2c_write(0xd0);           //initial address of rtc
   i2c_write(address);        //calling function for pheripheral address
   i2c_start();                //restarting
   i2c_write(0xd1);             //reading from seconds register
   info =i2c_read();            //saving the result in variable
   i2c_stop();                  //stopping read operation and return the value
   return info;
}

void write_operation(byte address,byte data)
{
   i2c_start();
   i2c_write(0xd0);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();
   
}


void main()
{

   lcd_init();
      write_operation(0x00,sec);         //calling the writing function to write the time
      write_operation(0x01,min);
      write_operation(0x00,hr);
   

   while(TRUE)
   {
      sec=read_operation(0);            //the values of rtc are read and stored 
      min=read_operation(1);
      hr=read_operation(2);
      printf(lcd_putc,"%x:%x:%x",hr,min,sec);  //printing the time
      lcd_gotoxy(1,1);
      delay_ms(300);
   }

}
