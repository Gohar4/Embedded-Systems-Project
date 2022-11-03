#include <avr/io.h> //downloading librarires 
#include <util/delay.h>
#include "adc.h"
//uart included within code not as a library


#define ledadc 5 // define constant variables
#define ledadc1 4
#define ledadc2 2
#define ledsw 1
#define swi 0


int main(void){
  init();
  Uart_Init(); // call uart function
  int adc_reading; // call adc function
  unsigned char buffer[4]; //convertrs values to range from 0 - 255 in a 8 bit data register

  
  while (1) //infinite loop
  {
   activation(); //calls void activation 
   adc_reading = Adc_ReadChannel(0); 
   itoa(adc_reading,buffer,10); // Uart library used instead of  serial monitor
   Uart_SendChar(buffer); 
  }
  return 0; 
}

void activation(){
  DDRC = 0x00; // C ports are all inputs

  DDRB = DDRB | (1 << ledsw); // led for switch set as output
  
  DDRB = DDRB | (1 << ledadc); // ledadc for POT set as output
  DDRB = DDRB | (1 << ledadc1); // ledadc1 for POT set as output
  DDRB = DDRB | (1 << ledadc2); // ledadc2 for POT set as output

  DDRB = DDRB & ~ (1 << swi); // switch set as input
  PORTB = PORTB | (1 << swi); // pull up resistance

  adc_Init(); 

    unsigned short a = Adc_ReadChannel(0); //saves ADC values from  channel 0 () in variable a
    
    if (a == 0) //if statement for POT values
    {
      PORTB = PORTB & ~ (1 << ledadc);    // All LEDs are off
      PORTB = PORTB & ~ (1 << ledadc1);
      PORTB = PORTB & ~ (1 << ledadc2);
    }

    else if(a < 300)
    {
      PORTB = PORTB | (1 << ledadc);    // Switch on ledadc while turning off ledadc1 and ledadc2
      PORTB = PORTB & ~ (1 << ledadc1); 
      PORTB = PORTB & ~ (1 << ledadc2); 
    }
    else if (a < 600 )
    {
      PORTB = PORTB & ~ (1 << ledadc);    // Switch on ledadc1 while turning off ledadc and ledadc2
      PORTB = PORTB | (1 << ledadc1);    
      PORTB = PORTB & ~ (1 << ledadc2);   
      
    }
    else if (a < 900 )
    {
      PORTB = PORTB & ~ (1 << ledadc1);   // Switch on ledadc2 while turning off ledadc1 and ledadc
      PORTB = PORTB | (1 << ledadc2);
      PORTB = PORTB & ~ (1 << ledadc);
    }
    else
    {
      PORTB = PORTB & ~ (1 << ledadc2); // turn off ledadc2
    }

    int s = (PINB & (1 << swi) != 0); // check if switch is not flipped
     if(s)   // if switch not flipped
    {
      PORTB = PORTB & ~(1 << ledsw);    // Switch OFF lED
    }  
    else 
    {
      PORTB = PORTB | (1 << ledsw);      // Switch ON lED
    }
  
}