////////////////////////////////////////////////////////////////////////////
//                                                                         //
//   Authore Name : Sivaprakasam.v                                         //
//   Title : ARM - PWM 1ms to 2ms                                          //
//   Date  : 05/08/2023                                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "arm_config.h"
void rcc_config(void);
void timer10_pwm_config(void);
void port_config(void);
void delay_time11(unsigned int p,unsigned int a);

int main()
{
				rcc_config();
				port_config();
				timer10_pwm_config();
				while(1)
				{
					while(1)
					{
						// 2ms on 18ms off
					for(int i=320;i;i--)
					{
						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = i;			// assign the ccr1 value
							delay_time11(1600,100);
							TIM10_PSC = 100;        // assign pse value
					}
					for(int i=0;i<=320;i++)
					{
						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = i;			// assign the ccr1 value
							delay_time11(1600,100);
							TIM10_PSC = 100;        // assign pse value
					}

				}
						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = 160;			// assign the ccr1 value
							TIM10_PSC = 50;        // assign pse value
							delay_time11(16000,50);
						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = 0;			// assign the ccr1 value
							delay_time11(16000,50);


			while(1);	
					   //1ms on 19ms off

						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = 16;			// assign the ccr1 value
							delay_time11(16000,10000);

							for(int i=16;i<24;i++)
							{
								while(!(TIM10_SR & 0X1));
								TIM10_SR &= ~0X3;               // clear UIF,CC1IF
								TIM10_CCR1 = i;			// assign the ccr1 value
								delay_time11(1600,10000);
							}
							
					   //1ms on 19ms off

						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = 24;			// assign the ccr1 value
							delay_time11(16000,10000);
							delay_time11(16000,10000);
							delay_time11(16000,10000);

							for(int i=24;i>=16;i--)
							{
								while(!(TIM10_SR & 0X1));
								TIM10_SR &= ~0X3;               // clear UIF,CC1IF
								TIM10_CCR1 = i;			// assign the ccr1 value
								delay_time11(1600,10000);
							}

						while(!(TIM10_SR & 0X1));
							TIM10_SR &= ~0X3;               // clear UIF,CC1IF
							TIM10_CCR1 = 0;			// assign the ccr1 value
							while(1);


				} 
}

void rcc_config(void)
{
             //   RCC_CR |= 1<<16 ; // HSE Enable

				while(!(RCC_CR & 1<<1));     // waiting for HSI ready flag

				//GPIOBEN: IO port B clock enable
				RCC_AHB1ENR |= 1<<1;

				//Enable TIMER10 and TIMER11
				RCC_APB2ENR |= 1<<17;
				RCC_APB2ENR |= 1<<18;
}

void port_config(void)
{

                GPIOB_MODER   &= ~(0x3<<16);        // clear bit
                GPIOB_MODER   |=  1<<17;            // 10: Alternate function mode
				GPIOB_OSPEEDR |=  0x3<<16;            // 10: high speed
				GPIOB_PUPDR   |=  1<<17;            // 10: Pull-down

				//GPIO alternate function high register (GPIOx_AFRH)
				GPIOB_AFRH &= ~0x0f;   //clear the values
				GPIOB_AFRH |= 0x3;    // load the 0011: AF3 values
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
	TIM10_CCR1 |= 160;        // assign the ccr1 value

	//ARPE: Auto-reload preload enable
	TIM10_CR1 |= 1<<7;

	// ARR[15:0]: Auto-reload value = 30
	TIM10_ARR  &= ~ 0xffff;  // clear the register
	TIM10_ARR  |= 320;       // assign arr value

	// PSC[15:0]: Prescaler value = 0
	TIM10_PSC &= ~0xffff;   // clear the register
	TIM10_PSC |= 1000;        // assign pse value

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
//	TIM11_PSC = 16000;            
//	TIM11_PSC = 32;            

	//Set reload value
	TIM11_ARR = a;
//	TIM11_ARR = 10000;
//	TIM11_ARR = 1000;

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
