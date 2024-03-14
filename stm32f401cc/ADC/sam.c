
#define RCC_APB2ENR 		(*(volatile unsigned int *)0x40023844)		// RCC APB2 peripheral clock enable register (RCC_APB2ENR)
#define RCC_AHB1ENR		(*(volatile unsigned int *)0x40023830)		// RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)

#define GPIOA_MODER		(*(volatile unsigned int *)0x40020000)		// GPIO port mode register 
#define GPIOA_OSPEEDR		(*(volatile unsigned int *)0x40020008)		// GPIO port output speed register 
#define GPIOA_AFRH		(*(volatile unsigned int *)0x40020024)		// GPIO alternate function high register 

#define GPIOB_MODER		(*(volatile unsigned int *)0x40020400)		// GPIO port mode register
#define GPIOB_OSPEEDR		(*(volatile unsigned int *)0x40020408)		// GPIO port output speed register
#define GPIOB_AFRH		(*(volatile unsigned int *)0x40020424)		// GPIO alternate function high register

#define ADC_SR			(*(volatile unsigned int *)0x40012000)		// ADC status register
#define ADC_CR1			(*(volatile unsigned int *)0x40012004)		// ADC control register 1
#define ADC_CR2			(*(volatile unsigned int *)0x40012008)		// ADC control register 2
#define ADC_SMPR2		(*(volatile unsigned int *)0x40012010)		// ADC sample time register 2
#define ADC_SQR3		(*(volatile unsigned int *)0x40012034)		// ADC regular sequence register 3
#define ADC_DR 		        (*(volatile unsigned int *)0x4001204C)		// ADC regular data register
#define ADC_CCR 		(*(volatile unsigned int *)0x40012304)		// ADC common control register

void rcc_config(void);
void port_config(void);
void adc_config(void);
unsigned int adc_converssion(void);

int main()
{
}

void rcc_config()
{
   RCC_AHB1ENR	= RCC_AHB1ENR | 1<<0; 		// portA enable

   RCC_AHB1ENR	= RCC_AHB1ENR | 1<<1;		// portB enable

   RCC_APB2ENR 	= RCC_APB2ENR | 1<<8;		// ADC1 clock enable
}

void port_config()
{
   GPIOB_MODER 	 = GPIOB_MODER & ~(0x03<<16);
   GPIOB_MODER	 = GPIOB_MODER | (1<<17);

   GPIOB_OSPEEDR = GPIOB_OSPEEDR & ~(0x03<<16);
   GPIOB_OSPEEDR = GPIOB_OSPEEDR | (1<<17);
}

void adc_config()
{
   // ADC status register
   ADC_SR 	= ADC_SR & ~(0x3F);		// clear the all ADC status register
 
   // control register 2
   ADC_CR2	= ADC_CR2 | (1<<0);     	// ADON: A/D Converter ON
 
   // ADC sample time register 2
   ADC_SMPR2	= ADC_SMPR2 | (1<<1);		// Channel 28  sampling time selection
 
   // ADC control register 1
   ADC_CR1	= ADC_CR1 & ~(3<<24);		// clear the ADC clock cycle
   ADC_CR1	= ADC_CR1 | (1<<25);		// 8-bit (11 ADCCLK cycles)
   ADC_CR1	= ADC_CR1 & ~(7<<13); 		// Bits 15:13 Discontinuous mode channel count
 
   // ADC regular sequence register 3
   ADC_SQR3	= ADC_SQR3 & ~(0x1F<<0);	// clear the 1st conversion in regular sequence
 
   // ADC common control register
   ADC_CCR	= ADC_CCR & ~(0x3<<16);		// ADC prescale
}

unsigned int adc_converssion()
{
   unsigned int data =0;

   // control register 2
   ADC_CR2	= ADC_CR2 | (1<<30);		// Start conversion of regular channels

   while(!(ADC_SR & 1<<1));			// wait for Regular channel end of conversion
   data = ADC_DR;
   return data;
}
