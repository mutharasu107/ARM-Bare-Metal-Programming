
#include "arm_config.h"

void rcc_config(void);
void port_config(void);
void usart_config(void);
void data_tx(unsigned char data);
unsigned char data_rx(void);
void delay_time11(unsigned int p,unsigned int a);
void timer10_pwm_config(void);
void pwm_run(void);
void adc_config(void);
unsigned int  adc_conversion(void);


int main()
{
	unsigned char *a="School of linux\n\r";
	unsigned char r_data;
	unsigned int adc_data,pwm_in=0;

	rcc_config();
    usart_config();
	adc_config();
	timer10_pwm_config();
	port_config();

	while(*a)
	{
		data_tx(*a++);
	}
	while(1)
	{
		adc_data = adc_conversion();
		pwm_in = adc_data+(adc_data >>2);

		while(!(TIM10_SR & 0X1));
			TIM10_SR &= ~0X3;               // clear UIF,CC1IF
			TIM10_CCR1 = pwm_in;			// assign the ccr1 value

		for(int i=16;i>=0;i--)
		{
			if(adc_data & 1<<i)
			{
				data_tx('1');

			}
			else 
				data_tx('0');
		}
		data_tx('\r');
		data_tx('\n');
	}

	while(1);

}

void rcc_config(void)
{
	//USART1 clock enable
	RCC_APB2ENR |=0x10;

	//GPIOAEN: IO port A clock enable
	RCC_AHB1ENR |= 0x1; 

	//GPIOBEN: IO port B clock enable
	RCC_AHB1ENR |= 1<<1;

	//Enable TIMER10 and TIMER11
	RCC_APB2ENR |= 1<<17;
	RCC_APB2ENR |= 1<<18;

	//ADC1 clock enable
	RCC_APB2ENR |= 1<<8;
}
void port_config(void)
{
	//10: Alternate function mode PIN A9 TX 
	GPIOA_MODER &= ~(3<<18);
	GPIOA_MODER |= 1<<19;

	//10: Alternate function mode PIN A10 RX 
	GPIOA_MODER &= ~(3<<20);
	GPIOA_MODER |= 1<<21;

	//GPIO port output speed register
	GPIOA_OSPEEDR &= ~(3<<18);
	GPIOA_OSPEEDR |= 1<<19;   // set as 10: High speed

	//GPIO port output speed register
	GPIOA_OSPEEDR &= ~(3<<20);
	GPIOA_OSPEEDR |= 1<<21;   // set as 10: High speed

	// 0111: AF7 GPIO alternate function high register 
	GPIOA_AFRH |= 0x70;

	// 0111: AF7 GPIO alternate function high register 
	GPIOA_AFRH |= 0x700;


    GPIOB_MODER   &= ~(0x3<<16);        // clear bit
    GPIOB_MODER   |=  1<<17;            // 10: Alternate function mode
	GPIOB_OSPEEDR &=  0x3<<16;            // 10: high speed
	GPIOB_PUPDR   |=  1<<17;            // 10: Pull-down

	//GPIO alternate function high register (GPIOx_AFRH)
	GPIOB_AFRH &= ~0x0f;   //clear the values
	GPIOB_AFRH |= 0x3;    // load the 0011: AF3 values
	
	//GPIOx_MODER
	GPIOA_MODER |= 0x3; //11: Analog mode
}
void usart_config(void)
{
	//Enable UE bit,M: Word length
	USART_CR1 &= ~(3<<12);
	USART_CR1 |= 1<<13;

	//number of stop bit 
	USART_CR2 &= ~(3<<12);
	USART_CR2 |= (1<<13);


	//select baud rate
	USART_BRR |= 0x3;  // fraction value = 3
	USART_BRR |= 104<<4; // mantissa value = 104


	//enable TE bit 
	USART_CR1 |= 0x8;

	//Enable RE
	USART_CR1 |= 1<<2;

	//cheak sattus 			
	while(!(USART_SR & (1<<7)));
//	USART_SR &= ~(1<<6); //clear TC bit

}
void data_tx(unsigned char data)
{

	//Data register
	USART_DR =data;   // character a

	//cheak sattus 			
	while(!(USART_SR & (1<<6)));
	USART_SR &= ~(1<<6); //clear TC bit

}

unsigned char data_rx(void)
{
	while(!(USART_SR & (1<<5)));
	return USART_DR;
}

void timer10_pwm_config(void)
{
	//  CC1E:0: Off - OC1 is not active
	TIM10_CCER &= ~(1<<0);

	// OC1PE:0 Preload register on TIMx_CCR1 disabled. TIMx_CCR1 can be written at anytime, the new value is taken in account immediately.
	TIM10_CCMR1 &= ~ (1<<3);

	// CC1S:00 channel is configured as output
	TIM10_CCMR1 &= ~ 0x3;

	// OC1M: 110: PWM mode 1 - Channel 1 is active as long as TIMx_CNT < TIMx_CCR1 else inactive
	TIM10_CCMR1 |= 0x60;

	// OC1FE: Output compare 1 fast enable
	TIM10_CCMR1 |= 1<<2;

	// CCR1[15:0]: Capture/Compare value = 8
	TIM10_CCR1 &= ~0xffff;  // clear the register
	TIM10_CCR1 |= 12;        // assign the ccr1 value

	//ARPE: Auto-reload preload enable
	TIM10_CR1 |= 1<<7;

	// ARR[15:0]: Auto-reload value = 30
	TIM10_ARR  &= ~ 0xffff;  // clear the register
	TIM10_ARR  |= 3200;       // assign arr value

	// PSC[15:0]: Prescaler value = 0
	TIM10_PSC &= ~0xffff;   // clear the register
	TIM10_PSC |= 100;        // assign pse value

	//CNT[15:0]: Counter value = 0
	TIM10_CNT |= 0;

	// CC1P: Capture/Compare 1 output Polarity = 0: OC1 active high
	TIM10_CCER &= ~0x2;

	// CC1NP: Capture/Compare 1 complementary output Polarity = 0:CC1 channel configured as output: CC1NP must be kept cleared.
	TIM10_CCER &= ~0x8;

	// CC1E: Capture/Compare 1 output enable.
	TIM10_CCER |= 0x1;

	// OC1PE: Output compare 1 fast enable
	TIM10_CCMR1 |=  1<<3;

	// UG: Update generation 1
	TIM10_EGR |= 0x3;

	// CEN: Counter enable
	TIM10_CR1 |= 0x1;
}

void delay_time11(unsigned int p,unsigned int a)
{
	//Enable Update disable
	TIM11_CR1 |= 1<<1;

	//clear UIF status
	TIM11_SR &= ~(1<<0);

	//Set count value
	TIM11_CNT = 0;

	//Set prescaler value
	TIM11_PSC = p;

	//Set reload value
	TIM11_ARR = a;

	//Update disable OFF
	TIM11_CR1 &= ~(1<<1);

	//Update request source
	TIM11_CR1 |= (1<<7);

	//Auto-reload preload enable
	TIM11_CR1 |= 1<<2;

	// UG: Update generation 1
	TIM11_EGR |= 0x3;

	//Counter enable
	TIM11_CR1 |= 1;

	// condition
	while((TIM11_SR & 1)==0);
}

void pwm_run(void)

{
					unsigned char data=0;
					unsigned int val=0;

						// 2ms on 18ms off

						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = 320;			// assign the ccr1 value
							delay_time11(16000,10000);

					   //1ms on 19ms off

						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = 160;			// assign the ccr1 value
							delay_time11(16000,10000);

							val = 160;
							
				while(1)
				{
					data = data_rx();

/*					if(data == '1')
					{
						
						// 2ms on 18ms off

						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = 320;			// assign the ccr1 value
							delay_time11(16000,10000);

					   //1ms on 19ms off

						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = 160;			// assign the ccr1 value
							delay_time11(16000,10000);

							val = 160;
					} */

					if(data == '0')
					{
						val = 0;
						while(!(TIM10_SR & 0X1));
						TIM10_SR &= ~0X3;               // clear UIF,CC1IF
						TIM10_CCR1 = val;			// assign the ccr1 value
						break;

					}

					if(data == 'a')
					{
						val++;
						if(val <=320)
						{
								while(!(TIM10_SR & 0X1));
								TIM10_SR &= ~0X3;               // clear UIF,CC1IF
								TIM10_CCR1 = val;			// assign the ccr1 value
						}
						else 
							val = 320;
					}

					if(data == 'b')
					{
						val--;
						if(val >=160)
						{
								while(!(TIM10_SR & 0X1));
								TIM10_SR &= ~0X3;               // clear UIF,CC1IF
								TIM10_CCR1 = val;			// assign the ccr1 value
						}
						else 
							val = 160;
					}

				}

}

void adc_config(void)
{

	// clear status register
	ADC_SR &= ~(0x3F);

	//ADC on
	ADC_CR2 |= 1;

	//ADC sample time register 2
	ADC_SMPR2 |= 2;

	//10: 8-bit (11 ADCCLK cycles)
	ADC_CR1 &= ~(0x3<<24);
	ADC_CR1 |= 1<<25;

	//15:13DISCNUM[2:0]: Discontinuous mode channel count
	ADC_CR1 &= ~(0x7<<13);

	//Bits 4:0 SQ1[4:0]: 1st conversion in regular sequence
	ADC_SQR3 &= ~(0x1F<<0);

	//coman register
	ADC_CCR &= ~(0x3<<16);

}

unsigned int adc_conversion(void)
{
	unsigned int adc_data=0;
	
	// SWSTART: Start conversion of regular channels
	ADC_CR2 |= 1<<30;

	while(!(ADC_SR & 0x2));  //waiting for adc conversion update EOC flag 
	adc_data = ADC_DR;
	return adc_data;
}
