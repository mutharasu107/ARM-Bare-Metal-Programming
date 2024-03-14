/************************************************/
/*   NAME        : R.MUTHARASU                  */
/*   DATE        :                              */
/*   DESCRIPTION : LED-BLINKING USING DELAY C13 */
/************************************************/

volatile unsigned int *RCC_CR       = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_PLLCFGR  = (volatile unsigned int *)0x40023804;
volatile unsigned int *RCC_CFGR     = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR  = (volatile unsigned int *)0x40023830;

//portc
volatile unsigned int *GPIOC_MODER  = (volatile unsigned int *)0x40020800;
volatile unsigned int *GPIOC_OTYPER = (volatile unsigned int *)0x40020804;
volatile unsigned int *GPIOC_ODR    = (volatile unsigned int *)0x40020814;

void rcc_config(void);
void gpioc_moder(void);
void led_blinking(void);
void delay(void);

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

        *RCC_CR   = *RCC_CR & (~0x00010000);
        *RCC_CR   = *RCC_CR | (1<<16);
        while((*RCC_CR & 1<<17)==0);
        *RCC_PLLCFGR = *RCC_PLLCFGR & (~0X00030000);
        *RCC_PLLCFGR = *RCC_PLLCFGR | (1<<16);
        *RCC_CFGR = *RCC_CFGR & (~0X00000003);
        *RCC_CFGR = *RCC_CFGR | (1<<0);
        while(!(*RCC_CFGR & 0X00000004)==4);
        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<2);     
}

void gpioc_moder()
{
        *GPIOC_MODER  = *GPIOC_MODER | (1<<26);
        *GPIOC_OTYPER = *GPIOC_OTYPER | (1<<13);
        *GPIOC_ODR    = *GPIOC_ODR | (1<<13);
}

void led_blinking()
{
        delay();
        if(*GPIOC_ODR & 1<<13)
        {
                *GPIOC_ODR = *GPIOC_ODR & ~(1<<13);
        }
        else
        {
                *GPIOC_ODR = *GPIOC_ODR | (1<<13);
        }
}

void delay()
{
        volatile unsigned int i =1;
        for(i=1;i<2500000;i++);
}
