/************************************************/
/*   NAME        : R.MUTHARASU                  */
/*   DATE        :                              */
/*   SUBJECT     :  ARMV7-SYS-TIMER             */
/*   DESCRIPTION : LED-BLINKING using portb     */
/************************************************/

//make sure all these pointers are assigned with correct address
volatile unsigned int *RCC_CR       = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_PLLCFGR  = (volatile unsigned int *)0x40023804;
volatile unsigned int *RCC_CFGR     = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR  = (volatile unsigned int *)0x40023830;
volatile unsigned int *RCC_APB2ENR  = (volatile unsigned int *)0x40023844;

//portB
volatile unsigned int *GPIOB_MODER  = (volatile unsigned int *)0x40020400;
volatile unsigned int *GPIOB_OTYPER = (volatile unsigned int *)0x40020404;
volatile unsigned int *GPIOB_ODR    = (volatile unsigned int *)0x40020414;

void RCC_CONFIG(void);
void GPIO_CONFIG(void);
void LED_BLINK(void);
void delay(void);

int main()
{
     
     RCC_CONFIG();
     GPIO_CONFIG();
     while(1)
     {    
     	LED_BLINK(); 
     }
}

void RCC_CONFIG(void)
{
      *RCC_CR	      = *RCC_CR &(~0x00010000); // HSE BIT SET AND CLEAR THE 16-BIT
      *RCC_CR         = *RCC_CR | (1<<16); //HSE-ON
      while((*RCC_CR &  1<<17)==0); //HSE FLAG CHECK ON AND OFF
      
      *RCC_CFGR	     =  *RCC_CFGR & (~0x00000003); //SYSTEM CLOCK SWITCH SET AND CLEAR
      *RCC_CFGR      =  *RCC_CFGR | (1<<0);   // SYSTEM CLOCK SWITCH SET
      while(!(*RCC_CFGR & 0x00000004)==4);   // SYSTEM CLOCK SWITCH STATUS
      *RCC_AHB1ENR   = *RCC_AHB1ENR | (1<<1);
    
}

void GPIO_CONFIG(void)
{
       *GPIOB_MODER    =  *GPIOB_MODER & (~0X00000030);
       *GPIOB_MODER    =  *GPIOB_MODER |(1<<4);	  
       *GPIOB_ODR      =  *GPIOB_ODR | (1<<2);
}

void delay(void)
{
	volatile unsigned int i=0;
	for(i=0;i<25000;i++);
}

void LED_BLINK(void)
{    	
	 if(*GPIOB_ODR & 1<<2)
  	 {
		*GPIOB_ODR  = *GPIOB_ODR &(~1<<2); // LED ON
  	 }
	 else
 	 {
        	*GPIOB_ODR  = *GPIOB_ODR |(1<<2); // LED OFF
 	 }
}
