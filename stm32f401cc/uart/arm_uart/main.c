/************************************************************/
/*   NAME        : R.MUTHARASU                              */
/*   DATE        :                                          */
/*   DESCRIPTION : UART                                     */
/************************************************************/

#include "arm_config.h"

void rcc_config(void);
void port_config(void);
void usart_config(void);
void data_tx(void);
void usar_data(unsigned char *u_data);

int main()
{
	unsigned char *a="School of linux";

	rcc_config();
	port_config();
        usart_config();

	data_tx();	
	usar_data(a);

	while(1);
}

void rcc_config(void)
{
	//USART1 clock enable
	RCC_APB2ENR |=0x10; // IO port C clock enable

	//GPIOAEN: IO port A clock enable
	RCC_AHB1ENR |= 0x1; 
}

void port_config(void)
{
	//10: Alternate function mode PIN A9 TX 
	GPIOA_MODER &= ~(3<<18);
	GPIOA_MODER |= 1<<19;

	//GPIO port output speed register
	GPIOA_OSPEEDR &= ~(3<<18);
	GPIOA_OSPEEDR |= 1<<19;   // set as 10: fast speed

	// 0111: AF7 GPIO alternate function high register 
	GPIOA_AFRH |= 0x70;
}

void usart_config(void)
{
	//Enable UE bit,M: Word length
	USART_CR1 &= ~(3<<12);	
	USART_CR1 |= 1<<13;

	//number of stop bit 
	USART_CR2 &= ~(3<<12);
	USART_CR2 |= (1<<13);

	//select baud rate 9600
	USART_BRR |= 4;  // fraction value = 4
	USART_BRR |= 103<<4; // mantissa value = 103

	//enable TE bit 
	USART_CR1 |= 0x8;

	//cheak sattus 			
	while(!(USART_SR & (1<<7)));
//	USART_SR &= ~(1<<6); //clear TC bit
}

void data_tx(unsigned char val)
{
	//Data register
	USART_DR = val;   // character a

	//cheak sattus 			
	while(!(USART_SR & (1<<6)));
	USART_SR &= ~(1<<6); //clear TC bit
}

void usar_data(unsigned char *u_data)
{
	while(*u_data)
	{
		data_tx(*u_data);
		u_data++;
	}
}
