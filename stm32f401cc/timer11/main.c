//select the  configiration register
volatile unsigned int *RCC_CR       = (volatile unsigned int *)0x40023800; /* RCC clock control register (RCC_CR) base address */
volatile unsigned int *RCC_CFGR     = (volatile unsigned int *)0x40023808; /* RCC clock configuration register (RCC_CFGR) base address */
volatile unsigned int *RCC_AHB1ENR  = (volatile unsigned int *)0x40023830; /* RCC AHB1 peripheral clock enable register (RCC_AHB1ENR) base address */
volatile unsigned int *RCC_APB2ENR  = (volatile unsigned int *)0x40023844; /* RCC APB2 peripheral clock enable register (RCC_APB2ENR) base address */

//porta
volatile unsigned int *GPIOA_MODER  = (volatile unsigned int *)0x40020000; /* GPIO port mode register (GPIOx_MODER) base address */
volatile unsigned int *GPIOA_ODR    = (volatile unsigned int *)0x40020014; /* GPIO port output data register (GPIOx_ODR) base address */

//timer11 register
volatile unsigned int *TIM11_CR1    = (volatile unsigned int *)0x40014800; /* TIM10/11 control register 1 (TIMx_CR1) basse address */
volatile unsigned int *TIM11_SR     = (volatile unsigned int *)0x40014810; /* TIM status register (TIMx_SR) base address */
volatile unsigned int *TIM11_CNT    = (volatile unsigned int *)0x40014824; /* TIM10/11 counter (TIMx_CNT) base address */
volatile unsigned int *TIM11_PSC    = (volatile unsigned int *)0x40014828; /* TIM10/11 prescaler (TIMx_PSC) bzse address */
volatile unsigned int *TIM11_ARR    = (volatile unsigned int *)0x4001482c; /* TIM10/11 prescaler (TIMx_PSC) base address */

void rcc_config(void);
void gpioa_moder(void);
void led_blinking(void);
void timer11_config(void);

int main()
{
        rcc_config();
        gpioa_moder();
        timer11_config();
        while(1)
        {
                led_blinking();
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
        *RCC_APB2ENR = *RCC_APB2ENR | 1<<18;            /* TIM11 clock enable */
        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);           /* IO port A clock enable */
}

void gpioa_moder(void)
{
        *GPIOA_MODER  = *GPIOA_MODER | (1<<0);         /* General purpose output mode */
}

void timer11_config(void)
{
        *TIM11_CR1   = *TIM11_CR1 & (~1 <<0);           /* Counter disable */
        *TIM11_CR1   = *TIM11_CR1 | (1<<1);             /* Update disable */
        *TIM11_CR1   = *TIM11_CR1 & (~1<<2);            /* Update request source */
        *TIM11_CR1   = *TIM11_CR1 | (1<<7);             /* Auto-reload preload enable */
        *TIM11_CNT   = 0;                               /* Counter value */
        *TIM11_PSC   = 1000;                            /* Prescaler value */
        *TIM11_ARR   = 2500;                            /* Auto-reload value */
        *TIM11_SR    = *TIM11_SR & (~1<<0);             /* Update interrupt flag clear */
        *TIM11_CR1   = *TIM11_CR1 | (1<<0);             /* Counter enable */
}

void led_blinking(void)
{
        while(!(*TIM11_SR & (1<<0)));                  /* Update interrupt flag set */
        *TIM11_SR    = *TIM11_SR & (~1<<0);            /* Update interrupt flag clear */
        if(*GPIOA_ODR & 1<<0)                         /* condition cheking */
        {
                *GPIOA_ODR = *GPIOA_ODR & (~1<<0);    /* LED ON */
        }
        else
        {
                *GPIOA_ODR = *GPIOA_ODR | (1<<0);     /* LED OFF */
        }
}
