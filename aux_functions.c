/**
  ******************************************************************************
  * @file    aux_functions.c
  * @author  Batuhan KINDAN
  * @version V1.0.0
  * @date    15 Aralik 2017
  * @brief   Programa ait basit gecikme gibi yardimci fonksiyonlar bu dosyada bulunur	 
  ******************************************************************************
  */
	

#include "stm32f4xx.h" // Device header
#include "aux_functions.h" // aux functions header

/**
  * @brief  basit gecikme fonksiyonu ~milisaniye
  * @param  delayValue  	
  * @retval none
  */
void Simple_Delay_ms(uint32_t delayValue)
{		
	delayValue *= 42000;
	
  while(delayValue)
	{
		delayValue--;
	}
}

/**
  * @brief  basit gecikme fonksiyonu ~mikrosaniye
  * @param  delayValue  	
  * @retval none
  */
void Simple_Delay_us(uint32_t delayValue)
{		
	delayValue *= 42;
	
  while(delayValue)
	{
		delayValue--;
	}
}
