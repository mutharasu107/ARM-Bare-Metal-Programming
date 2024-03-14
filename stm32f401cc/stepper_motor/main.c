/************************************************/
/*   NAME        : R.MUTHARASU                  */
/*   DATE        :                              */
/*   DESCRIPTION : stepper motor                */
/************************************************/

volatile unsigned int *RCC_CR       = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR     = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR  = (volatile unsigned int *)0x40023830;

//portb
volatile unsigned int *GPIOB_MODER  = (volatile unsigned int *)0x40020400;
volatile unsigned int *GPIOB_OTYPER = (volatile unsigned int *)0x40020404; /* GPIO port output type register (GPIOx_OTYPER) base address */
volatile unsigned int *GPIOB_ODR    = (volatile unsigned int *)0x40020414;

//porta
volatile unsigned int *GPIOA_MODER  = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_OTYPER = (volatile unsigned int *)0x40020004; /* GPIO port output type register (GPIOx_OTYPER) base address */
volatile unsigned int *GPIOA_ODR    = (volatile unsigned int *)0x40020014;

//timer10 register
volatile unsigned int *TIM10_CR1    = (volatile unsigned int *)0x40014400; /* TIM10/11 control register 1 (TIMx_CR1) basse address */
volatile unsigned int *TIM10_DIER   = (volatile unsigned int *)0x4001440c; /* TIM10/11 Interrupt enable register (TIMx_DIER) bae address */
volatile unsigned int *TIM10_SR     = (volatile unsigned int *)0x40014410; /* TIM status register (TIMx_SR) base address */
volatile unsigned int *TIM10_EGR    = (volatile unsigned int *)0x40014414; /* TIM event generation register (TIMx_EGR) base address */
volatile unsigned int *TIM10_CNT    = (volatile unsigned int *)0x40014424; /* TIM10/11 counter (TIMx_CNT) base address */
volatile unsigned int *TIM10_PSC    = (volatile unsigned int *)0x40014428; /* TIM10/11 prescaler (TIMx_PSC) bzse address */
volatile unsigned int *TIM10_ARR    = (volatile unsigned int *)0x4001442c; /* TIM10/11 prescaler (TIMx_PSC) base address */

void rcc_config(void);
void gpioc_moder(void);
void led_blinking(void);
void timer10_config(void);

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
        *RCC_CR   = *RCC_CR | (1<<16);				// HSEON: HSE clock enable        
        
	while((*RCC_CR & 1<<17)==0);			// External high-speed clock ready flag

	// Clock configuration register (RCC_CFGR)
        *RCC_CFGR = *RCC_CFGR & (~0X00000003);    	// clear System clock switch
        *RCC_CFGR = *RCC_CFGR | (1<<0);				// set HSE selected as system clock
        while(!(*RCC_CFGR & 0X00000004)==4);		// HSE oscillator used as system clock

        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);  		// IO port A clock enable   
        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<1);  			// IO port B clock enable   
}

void gpioc_moder()
{
        *GPIOA_MODER  = *GPIOA_MODER | (1<<26);		// General purpose output Open-drain	
        *GPIOA_ODR    = *GPIOA_OTYPER | (1<<13);		// out put mode

        *GPIOB_MODER  = *GPIOB_MODER | (1<<26);		// General purpose output Open-drain	
        *GPIOB_ODR    = *GPIOB_OTYPER | (1<<13);		// out put mode
}

void timer10_config(void)
{
        *TIM10_CR1   = *TIM10_CR1 & (~1 <<0);           /* Counter disable */
        *TIM10_CR1   = *TIM10_CR1 | (1<<1);      	       /* Update disable */
        *TIM10_CR1   = *TIM10_CR1 & (~1<<2);            /* Update request source */
        *TIM10_CR1   = *TIM10_CR1 & (~1<<3);            	/* One-pulse mode */
        *TIM10_CR1   = *TIM10_CR1 | (1<<7);             /* Auto-reload preload enable */
        *TIM10_CNT   = 0;                               	/* Counter value */
        *TIM10_PSC   = 1;                               /* Prescaler value */
        *TIM10_ARR   = 224999;                          	/* Auto-reload value */
        *TIM10_SR    = *TIM10_SR & (~1<<0);             /* Update interrupt flag clear */
        *TIM10_CR1   = *TIM10_CR1 | (1<<0);             	/* Counter enable */
}

void led_blinking()
{
        timer10_config();
        if(*GPIOC_ODR & 1<<13)
        {
                *GPIOC_ODR = *GPIOC_ODR & ~(1<<13);
        }
        else
        {
                *GPIOC_ODR = *GPIOC_ODR | (1<<13);
        }
}

