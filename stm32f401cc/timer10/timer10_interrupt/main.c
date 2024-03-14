/************************************************/
/*   NAME        : R.MUTHARASU                  */
/*   DATE        :                              */
/*   DESCRIPTION : TIMER10 USING interrupt      */
/************************************************/
 
//select the  configiration register
volatile unsigned int *RCC_CR          = (volatile unsigned int *)0x40023800; /* RCC clock control register (RCC_CR) base address */
volatile unsigned int *RCC_CFGR        = (volatile unsigned int *)0x40023808; /* RCC clock configuration register (RCC_CFGR) base address */
volatile unsigned int *RCC_AHB1ENR     = (volatile unsigned int *)0x40023830; /* RCC AHB1 peripheral clock enable register (RCC_AHB1ENR) base address */
volatile unsigned int *RCC_APB2ENR     = (volatile unsigned int *)0x40023844; /* RCC APB2 peripheral clock enable register (RCC_APB2ENR) base address */

//portc
volatile unsigned int *GPIOC_MODER     = (volatile unsigned int *)0x40020800; /* GPIO port mode register (GPIOx_MODER) base address */
volatile unsigned int *GPIOC_OTYPER    = (volatile unsigned int *)0x40020804; /* GPIO port output type register (GPIOx_OTYPER) base address */
volatile unsigned int *GPIOC_ODR       = (volatile unsigned int *)0x40020814; /* GPIO port output data register (GPIOx_ODR) base address */

//timer10 register
volatile unsigned int *TIM10_CR1       = (volatile unsigned int *)0x40014400; /* TIM10/11 control register 1 (TIMx_CR1) basse address */
volatile unsigned int *TIM10_DIER      = (volatile unsigned int *)0x4001440c; /* TIM10/11 Interrupt enable register (TIMx_DIER) bae address */
volatile unsigned int *TIM10_SR        = (volatile unsigned int *)0x40014410; /* TIM status register (TIMx_SR) base address */
volatile unsigned int *TIM10_EGR       = (volatile unsigned int *)0x40014414; /* TIM event generation register (TIMx_EGR) base address */
volatile unsigned int *TIM10_CNT       = (volatile unsigned int *)0x40014424; /* TIM10/11 counter (TIMx_CNT) base address */
volatile unsigned int *TIM10_PSC       = (volatile unsigned int *)0x40014428; /* TIM10/11 prescaler (TIMx_PSC) bzse address */
volatile unsigned int *TIM10_ARR       = (volatile unsigned int *)0x4001442c; /* TIM10/11 prescaler (TIMx_PSC) base address */

//timer10 interrupt flage register
volatile unsigned int (*NVIC_ISER10)[8]  = (volatile unsigned int (*)[8])0xE000E100;  /* Interrupt set-enable register */  
volatile unsigned int (*NVIC_ICER10)[8]  = (volatile unsigned int (*)[8])0xE000E180;  /* Interrupt clear-enable register */  
volatile unsigned int (*NVIC_ISPR10)[8]  = (volatile unsigned int (*)[8])0xE000E200;  /* Interrupt set-pending register */
volatile unsigned int (*NVIC_ICPR10)[8]  = (volatile unsigned int (*)[8])0xE000E280;  /* Interrupt clear-pending register */  
volatile unsigned int (*NVIC_IABR0)[8]   = (volatile unsigned int (*)[8])0xE000E300;  /* Interrupt active bit register */  
volatile unsigned int (*NVIC_IPR10)[60]  = (volatile unsigned int (*)[60])0xE000E400; /* Interrupt priority register */  
  
void gpioc_moder(void);
void rcc_config(void);
void led_blinking(void);
void timer10_config(void);
void timer10_interrupt(void);
void TIM1_UP_TIM10_IRQHandler(void);
 
int main()
{
        rcc_config();
        gpioc_moder();
        TIM1_UP_TIM10_IRQHandler();
        timer10_interrupt(); 
        timer10_config();
        while(1)
        {
 //             led_blinking();
        }
}

void rcc_config(void)
{

        *RCC_CR   = *RCC_CR & (~0x00010000);            /* HSE clock enable clear bit */
        *RCC_CR   = *RCC_CR | (1<<16);                  /* HSE clock enable set the bit */
        while((*RCC_CR & 1<<17)==0);                    /* HSE clock ready flag */
        *RCC_CFGR = *RCC_CFGR & (~0X00000003);          /* System clock switch clear */
        *RCC_CFGR = *RCC_CFGR | (1<<0);                 /* System clock switch set */
        while(!(*RCC_CFGR & 0X00000004)==4);            /* System clock switch status */
        *RCC_APB2ENR = *RCC_APB2ENR | 1<<17;            /* TIM10 clock enable */
        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<2);           /* IO port C clock enable */
}

void gpioc_moder(void)
{
        *GPIOC_MODER  = *GPIOC_MODER | (1<<26);         /* General purpose output mode */
        *GPIOC_OTYPER = *GPIOC_OTYPER | (1<<13);        /* Output open-drain */
        *GPIOC_ODR    = *GPIOC_ODR | (1<<13);           /* Out put mode */
}

void timer10_config(void)
{
        *TIM10_CR1   = *TIM10_CR1 & (~1 <<0);           /* Counter disable */
        *TIM10_CR1   = *TIM10_CR1 | (1<<1);             /* Update disable */
        *TIM10_CR1   = *TIM10_CR1 & (~1<<2);            /* Update request source */
        *TIM10_CR1   = *TIM10_CR1 & (~1<<3);            /* One-pulse mode */
        *TIM10_CR1   = *TIM10_CR1 | (1<<7);             /* Auto-reload preload enable */
        *TIM10_DIER  =*TIM10_DIER | (1<<0);             /* update interrupt enabled */
        *TIM10_EGR   = *TIM10_EGR | (1<<0);             /* Update generation */
        *TIM10_CNT   = 0;                               /* Counter value */
        *TIM10_PSC   = 1000;                            /* Prescaler value */
        *TIM10_ARR   = 1250;                            /* Auto-reload value */
        *TIM10_SR    = *TIM10_SR & (~1<<0);             /* Update interrupt flag clear */
        *TIM10_CR1   = *TIM10_CR1 | (1<<0);             /* Counter enable */
}

void led_blinking(void)
{
        while(!(*TIM10_SR & (1<<0)));                  /* Update interrupt flag set */
        *TIM10_SR    = *TIM10_SR & (~1<<0);            /* Update interrupt flag clear */
        if(*GPIOC_ODR & 1<<13)                         /* condition cheking */
        {
                *GPIOC_ODR = *GPIOC_ODR & (~1<<13);    /* LED ON */
        }
        else
        {
                *GPIOC_ODR = *GPIOC_ODR | (1<<13);     /* LED OFF */
        }
}


void TIM1_UP_TIM10_IRQHandler(void)
{
        *TIM10_SR    = *TIM10_SR & (~1<<0);            /* Update interrupt flag clear */
        if(*GPIOC_ODR & 1<<13)                         /* condition cheking */
        {
                *GPIOC_ODR = *GPIOC_ODR & (~1<<13);    /* LED ON */
        }
        else
        {
                *GPIOC_ODR = *GPIOC_ODR | (1<<13);     /* LED OFF */
        }
}

void timer10_interrupt(void)
{
        (*NVIC_ICPR10)[0]   = (*NVIC_ICPR10)[0] | 1<<26;    /* Interrupt set-enable register */
        (*NVIC_ISER10)[0]   = (*NVIC_ISER10)[0] | 1<<26;    /* Interrupt set-pending register */
}
