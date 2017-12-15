/**
  ******************************************************************************
  * @file    configurations.c
  * @author  Batuhan KINDAN
  * @version V1.0.0
  * @date    15 Aralik 2017
  * @brief   Mikrodenetleyici cevresel elemanlarinin konfigurasyonlarini iceren
  *          fonksiyonlar bu dosyada bulunmaktadir.
  ******************************************************************************
  */

#include "stm32f4xx.h" // Device header
#include "configurations.h" // configurations file header


/**
  * @brief  USART2 haberlesmesi icin gerekli konfigurasyonlar bu fonksiyonda yapilir 			
  * @param  none  	
  * @retval none
  */
void USART2_Init(void)																																				
{
	USART_InitTypeDef USART_InitStructure; // USART konfigurasyon yapi degiskeni	
	GPIO_InitTypeDef USART_GPIO_InitStructure; // GPIO konfigurasyon yapi degiskeni
	NVIC_InitTypeDef USART_NVIC_InitStructure; // NVIC konfigurasyon yapi degiskeni
	
	/* USART Pin Konfigurasyonu */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// Saat frekansini GPIOA icin aktif et
	
	USART_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // Pin modu alternatif fonksiyon
	USART_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // Output tipi push pull
	USART_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // USART2 Tx pin PA2 Rx Pin PA3
	USART_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // Pull up direnc
	USART_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO hizi 50MHz
	
	GPIO_Init(GPIOA, &USART_GPIO_InitStructure); // GPIO'yu gecerli konfigurasyonlar ici baslat
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); // PA2 pini USART2 icin alternate function modunda
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); // PA3 pini USART2 icin alternate function modunda
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART2 sistem saati aktif
	
	/* USART Baslatma Konfigurasyonu */
	USART_InitStructure.USART_BaudRate = 115200; // USART aktarim hizi bit/saniye
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // USART modu Tx Rx
	USART_InitStructure.USART_Parity = USART_Parity_No; // Parity biti yok
	USART_InitStructure.USART_StopBits = USART_StopBits_1; // Stop biti 1 adet
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // Kelime bit sayisi
	
	USART_Init(USART2,&USART_InitStructure); // USART Init
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // Kesme konfigurasyonu Usart Rx icin																
	
	/* Nested Vector Interrupt Controller konfigurasyonu */
	USART_NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		 
	USART_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	USART_NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		 
	USART_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 
	NVIC_Init(&USART_NVIC_InitStructure);							 
	
	USART_Cmd(USART2,ENABLE); // USART Aktif
}	
