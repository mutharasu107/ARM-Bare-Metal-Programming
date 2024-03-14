/************************************************/
/*   NAME        : R.MUTHARASU                  */
/*   DATE        :                              */
/*   DESCRIPTION : LED-BLINKING witu C13 pin    */
/************************************************/

volatile unsigned int *RCC_CR       = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR     = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR  = (volatile unsigned int *)0x40023830;

//portc
volatile unsigned int *GPIOC_MODER  = (volatile unsigned int *)0x40020800;
volatile unsigned int *GPIOC_ODR    = (volatile unsigned int *)0x40020814;

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

        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<2);  		// IO port C clock enable   
}

void gpioc_moder()
{
        *GPIOC_MODER  = *GPIOC_MODER | (1<<26);		// General purpose output Open-drain	
        *GPIOC_ODR    = *GPIOC_ODR | (1<<13);		// out put mode
}

void led_blinking()
{
        if(*GPIOC_ODR & 1<<13)				// condition checking
        {
                *GPIOC_ODR = *GPIOC_ODR & ~(1<<13);	// LED ON
        }
        else
        {
                *GPIOC_ODR = *GPIOC_ODR | (1<<13);	// LED OFF
        }
}
