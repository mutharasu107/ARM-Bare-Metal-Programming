/************************************************/
/*   NAME        : R.MUTHARASU                  */
/*   DATE        :                              */
/*   DESCRIPTION : PLL CONFIGURATION            */
/************************************************/

//select the configiration register
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

//        *RCC_CR   = *RCC_CR & (~0x00010000);            /* HSE clock enable clear bit */
//        *RCC_CR   = *RCC_CR | (1<<16);                  /* HSE clock enable set the bit */
//        while((*RCC_CR & 1<<17)==0);                    /* HSE clock ready flag */
        *RCC_PLLCFGR = *RCC_PLLCFGR & (~0X0000003F);    /* Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock clear bits*/
        *RCC_PLLCFGR = *RCC_PLLCFGR | (0X00000005);     /* Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock */
        *RCC_PLLCFGR = *RCC_PLLCFGR & (~0X00007FC0);    /* Main PLL (PLL) multiplication factor for VCO clear */
        *RCC_PLLCFGR = *RCC_PLLCFGR | (0X00000A00);     /* Main PLL (PLL) multiplication factor for VCO set  */
        *RCC_PLLCFGR = *RCC_PLLCFGR & (~0X00030000);    /* Main PLL (PLL) division factor for main system clock clear */
        *RCC_PLLCFGR = *RCC_PLLCFGR | (1<<17);          /* Main PLL (PLL) division factor for main system clock set */
        *RCC_PLLCFGR = *RCC_PLLCFGR | (1<<22);          /* Main PLL(PLL) and audio PLL (PLLI2S) entry clock source */
        *RCC_CR   = *RCC_CR | (1<<24);                  /* Main PLL (PLL) enable */
        *RCC_CFGR = *RCC_CFGR & (~0X00000003);          /* System clock switch clear */
        *RCC_CFGR = *RCC_CFGR | (1<<1);                 /* System clock switch set */
        while(!(*RCC_CFGR & 0X00000008)==8);            /* System clock switch status */
        *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<2);           /* IO port C clock enable */
}

void gpioc_moder()
{
        *GPIOC_MODER  = *GPIOC_MODER | (1<<26);         /* General purpose output mode */
        *GPIOC_OTYPER = *GPIOC_OTYPER | (1<<13);        /* Output open-drain */
        *GPIOC_ODR    = *GPIOC_ODR | (1<<13);           /* out put mode */
}

void led_blinking()
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
