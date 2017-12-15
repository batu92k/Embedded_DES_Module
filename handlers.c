/**
  ******************************************************************************
  * @file    handlers.c
  * @author  Batuhan KINDAN
  * @version V1.0.0
  * @date    15 Aralik 2017
  * @brief   Programda konfigure edilmis her turlu kesme rutinine ait fonksiyon 
  *          bu dosya icerisindedir
  ******************************************************************************
  */
	
#include "stm32f4xx.h" // Device header
#include "handlers.h" // handlers file header

	/**
  * @brief  USART2 icin receive data (Rx) kesme fonksiyonu
  * @param  none  	
  * @retval none
  */
void USART2_IRQHandler(void)
{
	
	/* ilgili USART bayraginin Rx kesme bayraginin set edilip edilmedigi kontrol ediliyor */
	if( USART_GetITStatus(USART2, USART_IT_RXNE) )
	{
    // bu satir bilerek bos birakildi
	}
	else
	{
    // bu satir bilerek bos birakildi
	}
	
}
