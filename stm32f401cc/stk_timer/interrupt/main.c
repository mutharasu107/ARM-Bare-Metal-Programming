/************************************************/
/*   NAME        : R.MUTHARASU                  */
/*   DATE        :                              */
/*   DESCRIPTION : INTERRUPT PROGRAM            */
/************************************************/

//select the configiration register
volatile unsigned int *RCC_CR       = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR     = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR  = (volatile unsigned int *)0x40023830;

//portc
volatile unsigned int *GPIOC_MODER  = (volatile unsigned int *)0x40020800;
volatile unsigned int *GPIOC_OTYPER = (volatile unsigned int *)0x40020804;
volatile unsigned int *GPIOC_ODR    = (volatile unsigned int *)0x40020814;

//sys tick timer
volatile unsigned int *STK_CTRL     = (volatile unsigned int *)0xE000E010;
volatile unsigned int *STK_LOAD     = (volatile unsigned int *)0xE000E014;
volatile unsigned int *STK_VAL      = (volatile unsigned int *)0xE000E018; 

void rcc_config(void);
void gpioc_moder(void);
void led_blinking(void);
void SysTick_Handler(void);
void sys_tick_timer(void);

int main()
{
        rcc_config();
        gpioc_moder();
        sys_tick_timer();
        SysTick_Handler();
        while(1)
        {
//                led_blinking();
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
        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<2);           /* IO port C clock enable */
}

void gpioc_moder(void)
{
        *GPIOC_MODER  = *GPIOC_MODER | (1<<26);         /* General purpose output mode */
        *GPIOC_OTYPER = *GPIOC_OTYPER | (1<<13);        /* Output open-drain */
        *GPIOC_ODR    = *GPIOC_ODR | (1<<13);           /* Out put mode */
}
	
void SysTick_Handler()
{

        if(*GPIOC_ODR & 1<<13)                          /* condition cheking */
        {
                *GPIOC_ODR = *GPIOC_ODR & ~(1<<13);     /* LED ON */
        }
        else
        {
                *GPIOC_ODR = *GPIOC_ODR | (1<<13);      /* LED OFF */
        }
}

void sys_tick_timer()
{
        *STK_CTRL = *STK_CTRL | (1<<0);
        *STK_CTRL = *STK_CTRL | (1<<2);
        *STK_LOAD = 25000000-1;
        *STK_VAL  = 0;
}
