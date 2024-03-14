// timer2_3_4_5 program

// select the congiguration register
volatile unsigned int *RCC_CR		= (volatile unsigned int *)0x40023800; // RCC clock control register (RCC_CR)
volatile unsigned int *RCC_CFGR		= (volatile unsigned int *)0x40023808; // RCC clock configuration register (RCC_CFGR)
volatile unsigned int *RCC_APB1ENR	= (volatile unsigned int *)0x40023840; // RCC APB1 peripheral clock enable register (RCC_APB1ENR)
volatile unsigned int *RCC_AHB1ENR	= (volatile unsigned int *)0x40023830; // RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)

// portc register
volatile unsigned int *GPIOC_MODER 	= (volatile unsigned int *)0x40020800; // GPIO port mode register (GPIOx_MODER) base address
volatile unsigned int *GPIOC_ODR	= (volatile unsigned int *)0x40020814; // GPIO port output data register (GPIOx_ODR) base address

// timer register
volatile unsigned int *TIM2_CR1		= (volatile unsigned int *)0x40000000; // TIM2 control register 1 (TIM2_CR1)
volatile unsigned int *TIM2_PSC		= (volatile unsigned int *)0x40000028; // TIMx prescaler (TIMx_PSC)
volatile unsigned int *TIM2_CNT 	= (volatile unsigned int *)0x40000024; // TIMx counter (TIMx_CNT)
volatile unsigned int *TIM2_ARR		= (volatile unsigned int *)0x4000002C; // TIMx auto-reload register (TIMx_ARR)
volatile unsigned int *TIM2_SR		= (volatile unsigned int *)0x40000010; // TIMx status register (TIMx_SR)

void rcc_con(void);
void gpio_con(void);
void led_blink(void);
void timer_con(void);

int main()
{
   rcc_con();
   gpio_con();
   timer_con();

   while(1)
   {
      led_blink();
   }
}

void rcc_con(void)
{
   *RCC_CR = *RCC_CR & (~0x00010000);
   *RCC_CR = *RCC_CR | (1<<16);
   while((*RCC_CR & 1<<17)==0);
   *RCC_CFGR = *RCC_CFGR & (~0x00000003);
   *RCC_CFGR = *RCC_CFGR | (1<<0);
   while(!(*RCC_CFGR & 0x00000004)==4);
   *RCC_APB1ENR = *RCC_APB1ENR | 1<<0;
   *RCC_AHB1ENR = *RCC_AHB1ENR | 1<<2;
}

void gpio_con(void)
{
   *GPIOC_MODER = *GPIOC_MODER | 1<<26;
}

void timer_con(void)
{
   *TIM2_CR1	= *TIM2_CR1 & (~1<<0);
   *TIM2_CR1	= *TIM2_CR1 | (1<<1);
   *TIM2_CR1	= *TIM2_CR1 & (~1<<2);
   *TIM2_CR1	= *TIM2_CR1 | (1<<7);
   *TIM2_CNT	= 0;
   *TIM2_PSC	= 1000;
   *TIM2_ARR	= 2500;
   *TIM2_SR	= *TIM2_SR & (~1<<0);
   *TIM2_CR1	= *TIM2_CR1 | 1<<0;
}

void led_blink(void)
{
   while(!(*TIM2_SR & 1<<0));
   *TIM2_SR	= *TIM2_SR & (~1<<0);
   if(*GPIOC_ODR & 13)
   {
      *GPIOC_ODR 	= *GPIOC_ODR & (~1<<13);
   }
   else
   {
      *GPIOC_ODR 	= *GPIOC_ODR | (1<<13);
   }
}
