/**
  ******************************************************************************
  * @file    usart_functions.h
  * @author  Batuhan KINDAN
  * @version V1.0.0
  * @date    15 Aralik 2017
  * @brief   USART haberlesme protokolu ile ilgili fonksiyonlarin prototipleri
  *    	     burada bulunur
  ******************************************************************************
  */
	
#ifndef USART_FUNCTIONS_H
#define USART_FUNCTIONS_H

#include "stm32f4xx_usart.h"

void USART_Puts(USART_TypeDef* USARTx, volatile char *s);

#endif
