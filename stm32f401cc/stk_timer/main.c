/************************************************************/
/*   NAME        : R.MUTHARASU                              */
/*   DATE        :                                          */
/*   DESCRIPTION : LED-BLINKING using sys tick timer C13 pin*/
/************************************************************/

volatile unsigned int *RCC_CR       = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR     = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR  = (volatile unsigned int *)0x40023830;

//portc
volatile unsigned int *GPIOC_MODER  = (volatile unsigned int *)0x40020800;
volatile unsigned int *GPIOC_ODR    = (volatile unsigned int *)0x40020814;

//sys tick timer
volatile unsigned int *STK_CTRL     = (volatile unsigned int *)0xE000E010;
volatile unsigned int *STK_LOAD     = (volatile unsigned int *)0xE000E014;

void rcc_config(void);
void gpioc_moder(void);
void sys_tick_timer(void);
void led_blinking(void);

int main()
{
        rcc_config();
        gpioc_moder();
        sys_tick_timer();
        while(1)
        {
                led_blinking();
        }
}

void rcc_config()
{

        *RCC_CR   = *RCC_CR & (~0x00010000);
        *RCC_CR   = *RCC_CR | (1<<16);
        while((*RCC_CR & 1<<17)==0);
        *RCC_CFGR = *RCC_CFGR & (~0X00000003);
        *RCC_CFGR = *RCC_CFGR | (1<<0);
        while(!(*RCC_CFGR & 0X00000004)==4);
        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<2);     
}

void gpioc_moder()
{
        *GPIOC_MODER  = *GPIOC_MODER | (1<<26);
}

void led_blinking()
{
                *GPIOC_ODR = *GPIOC_ODR & ~(1<<13);
		sys_tick_timer();
                
		*GPIOC_ODR = *GPIOC_ODR | (1<<13);
		sys_tick_timer();
}

void sys_tick_timer()
{
        *STK_CTRL = *STK_CTRL | (1<<0);
        *STK_CTRL = *STK_CTRL | (1<<2);
        *STK_LOAD = 2499999;
        while(!(*STK_CTRL & (1<<16)));
}
