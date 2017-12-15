/**
  ******************************************************************************
  * @file    usart_functions.c
  * @author  Batuhan KINDAN
  * @version V1.0.0
  * @date    15 Aralik 2017
  * @brief   USART haberlesme protokolu ile ilgili fonksiyonlar burada bulunur
  ******************************************************************************
  */
	
#include "stm32f4xx.h" // Device header
#include "usart_functions.h" // usart fonksiyonlari icin header

	/**
  * @brief  Bu fonksiyon USART uzerinden istenilen bir karakter dizisini gonderir
  * @param  USARTx  	
	* @param  s
  * @retval none
  */
void USART_Puts(USART_TypeDef* USARTx, volatile char *s)
{
	while(*s)
	{
		while(!(USARTx ->SR & 0x00000040)); // usart kullanilabilir olana kadar bekle
		USART_SendData(USARTx,*s); // veriyi gönder
		*s++; // bir sonraki karaktere geç
	}
}
