/************************************************/
/*   NAME        : R.MUTHARASU                  */
/*   DATE        :                              */
/*   DESCRIPTION : LED-BLINKING witu C13 pin    */
/************************************************/

volatile unsigned int *RCC_CR       = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR     = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR  = (volatile unsigned int *)0x40023830;

//porta
volatile unsigned int *GPIOA_MODER  = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR    = (volatile unsigned int *)0x40020014;

//portb
volatile unsigned int *GPIOB_MODER  = (volatile unsigned int *)0x40020400;
volatile unsigned int *GPIOB_ODR    = (volatile unsigned int *)0x40020414;

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

	*RCC_AHB1ENR = *RCC_AHB1ENR | (1<<1);  		// IO port B clock enable   
        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);  		// IO port A clock enable   
}

void gpioc_moder()
{
   	int i,j;
   	for(i=0,j=0; i<=10, j<20; i++, j++)
	{
	   	*GPIOA_MODER  = *GPIOA_MODER | (1<<j);		// General purpose output Open-drain	
	   	*GPIOA_ODR    = *GPIOA_ODR | (1<<i);		// out put mode

		*GPIOB_MODER  = *GPIOB_MODER | (1<<j);		// General purpose output Open-drain
	   	*GPIOB_ODR    = *GPIOB_ODR | (1<<i);		// out put mode
		j++;
	}
}

void led_blinking()
{
   	int i,k,a=0,b=200,j;
	while(a<b)
	{
   		for( i=0; i<=10; i++)
		{
		   	if(i==3 || i==4)
			{
			   continue;
			}
	   		for(k=0;k<20;k++)
			{
   		   		if(a >= k)
				{
	   				delay();
				}
			}
	   		if(*GPIOA_ODR & 1<<i || *GPIOB_ODR & 1<<i)				// condition checking
        		{
                		*GPIOA_ODR = *GPIOA_ODR & ~(1<<i);	// LED OFF
				*GPIOB_ODR = *GPIOB_ODR & ~(1<<i);	// LED OFF
        		}
        		else
        		{
                		*GPIOA_ODR = *GPIOA_ODR | (1<<i);	// LED ON
				*GPIOB_ODR = *GPIOB_ODR | (1<<i);	// LED ON
        		}
		}
		a++;
		a++;
	}
}

void delay()
{
        volatile unsigned int i =1;
        for(i=1;i<25000;i++);
}
