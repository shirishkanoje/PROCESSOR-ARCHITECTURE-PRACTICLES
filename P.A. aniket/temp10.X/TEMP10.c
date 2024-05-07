#include <pic18f4550.h>
#include <stdio.h>
#define LCD_EN LATAbits.LA1
#define LCD_RS LATAbits.LA0
#define LCDPORT LATB
unsigned char str[16];
void lcd_delay(unsigned int time)
{
unsigned int i , j ;
 for(i = 0; i < time; i++)
 {
 for(j=0;j<100;j++);
 }
}
void SendInstruction(unsigned char command)
{
 LCD_RS = 0; // RS low : Instruction
 LCDPORT = command;
 LCD_EN = 1; // EN High
 lcd_delay(10);
 LCD_EN = 0; // EN Low; command sampled at EN falling edge
 lcd_delay(10);
}
void SendData(unsigned char lcddata)
{
 LCD_RS = 1; // RS HIGH : DATA
 LCDPORT = lcddata;
 LCD_EN = 1; // EN High
 lcd_delay(10);
 LCD_EN = 0; // EN Low; data sampled at EN falling edge
 lcd_delay(10);
}
void InitLCD(void)
{
 ADCON1 = 0x0F;
 TRISB = 0x00; //set data port as output
 TRISAbits.RA0 = 0; //RS pin
 TRISAbits.RA1 = 0; // EN pin
 SendInstruction(0x38); //8 bit mode, 2 line,5x7 dots
 SendInstruction(0x06); //entry mode
 SendInstruction(0x0C); //Display ON cursor OFF
 SendInstruction(0x01); //Clear display
 SendInstruction(0x80); //set address to 0
}
void LCD_display(unsigned int row, unsigned int pos, unsigned char *ch)
{
 if(row==1)
 SendInstruction(0x80 | (pos-1));
 else
 SendInstruction(0xC0 | (pos-1));

 while(*ch)
 SendData(*ch++);
}
void ADCInit(void)
{
 TRISEbits.RE2 = 1; //ADC channel 7 input
 ADCON1 = 0b00000111; //Ref voltages Vdd & Vss; AN0 - AN7 channels Analog
 ADCON2 = 0b10101110; //Right justified; Acquisition time 4T; Conversion clock

}
unsigned short Read_Temp(void)
{
 ADCON0 = 0b00011101; //ADC on; Select channel;
 GODONE = 1; //Start Conversion
 while(GO_DONE == 1 ); //Wait till A/D conversion is complete
 return ADRES; //Return ADC result
}
int main(void)
{
 unsigned int temp;
 InitLCD();
 ADCInit();
 LCD_display(1,1,"Temperature:");
 while(1)
 {
 temp = Read_Temp();
 temp = ((temp * 500) / 1023);
 sprintf(str,"%d'C ",temp);
 LCD_display(2,1,str);
 lcd_delay(9000);
 }
 return 0;
}
