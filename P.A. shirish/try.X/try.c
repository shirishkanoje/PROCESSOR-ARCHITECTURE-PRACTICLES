#include<pic18f4550.h>
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000  // Assuming 8MHz oscillator frequency
#endif
void main() {
    TRISD = 0x00; // Set PORTD as output

    while(1) {
   PORTD=1;

   
  
    }}