/************************************************/
/*   NAME        : R.MUTHARASU                  */
/*   DATE        :                              */
/*   DESCRIPTION : Using PIR sensor blink led   */
/************************************************/
#include "config.h"

#define PIR 0<<0

void delay(void);
void rcc_config(void);
void gpioc_moder(void);
void led_blinking(void);

int main()
{
        rcc_config();
        gpioc_moder();
        while(1)
        {
                led_blinking();
        }
}

void rcc_config()
{

   	// Clock control register (RCC_CR)
        *RCC_CR   = *RCC_CR & (~0x00010000);      	// HSEON: HSE clock diable
        *RCC_CR   = *RCC_CR | (1<<16);			// HSEON: HSE clock enable        
        
	while((*RCC_CR & 1<<17)==0);			// External high-speed clock ready flag

	// Clock configuration register (RCC_CFGR)
        *RCC_CFGR = *RCC_CFGR & (~0X00000003);    	// clear System clock switch
        *RCC_CFGR = *RCC_CFGR | (1<<0);			// set HSE selected as system clock
        while(!(*RCC_CFGR & 0X00000004)==4);		// HSE oscillator used as system clock


//	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<1);  		// IO port B clock enable   
        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);  		// IO port A clock enable   
}

void gpioc_moder()
{
	   	*GPIOA_MODER  = *GPIOA_MODER | (1<<0);		// General purpose output Open-drain	
//	   	*GPIOA_ODR    = *GPIOA_ODR | (1<<0);		// out put mode

	   	*GPIOA_MODER  = *GPIOA_MODER | (1<<2);		// General purpose output Open-drain	
//	   	*GPIOA_ODR    = *GPIOA_ODR | (1<<1);		// out put mode
		
}

void led_blinking()
{
   	while( *GPIOA_IDR )
	{
   		if( *GPIOA_ODR & (1<<0) )				// condition checking
		{
			      *GPIOA_ODR = *GPIOA_ODR | (1<<1);   //led on	
        	}
        	else
        	{
			       *GPIOA_ODR = *GPIOA_ODR & ~(1<<1);  //led off	
        	}
	}
}

void delay()
{
        volatile unsigned int i =1;
        for(i=1;i<25000;i++);
}
