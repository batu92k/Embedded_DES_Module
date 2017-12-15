/**
  ******************************************************************************
  * @file    main.c
  * @author  Batuhan KINDAN
  * @version V1.0.0
  * @date    15 Aralik 2017
  * @brief   Program giris noktasini olusturan main dosyasi  
  ******************************************************************************
  */

#include "stm32f4xx.h" // Device header
#include "configurations.h" // configurations file header
#include "aux_functions.h" // aux functions header
#include "usart_functions.h" // usart fonksiyonlari icin header
#include "des_module.h" // des module file header
#include "stdio.h" // input output fonksiyonlari (sprintf gibi)
#include "stdint.h" // integer veri tipi icin header

DES_InitTypedef DES_InitStructure;
#define DataLenght 5

#define MAX_STRLEN 30 // max karakter uzunlugu
volatile char buffer_string[MAX_STRLEN+1]; // alinan UART verisi icin tutucu karakter dizisi
										 
/**
  * @brief  Programin giris noktasi olan main fonksiyonu
  * @param  none 	
  * @retval yok
  */
int main()
{
	int i = 0;
	uint8_t clean_cnt = 0;
	uint64_t plainText[DataLenght] = { 0x123456789ABCDEF, 0x023456789ABCDEF, 0x103456789ABCDEF, 0x120456789ABCDEF, 0x123456789ABCDEF};
	uint64_t cipherText[DataLenght];
	uint64_t decryptedText[DataLenght];

	DES_InitStructure.Key = 0x133457799BBCDFF1;
	DES_InitStructure.IV = 0xFFAABBCCEE234561;
	DES_InitStructure.Mode = ECB;	

	SystemInit();
	USART2_Init(); // USART2 baslatiliyor	
	
	
	/* program ana sonsuz dongusu */
	while(1)
	{
		
		DES_Encrypt_Data(&DES_InitStructure, plainText, cipherText, DataLenght);
		DES_Decrypt_Data(&DES_InitStructure, cipherText, decryptedText, DataLenght);	

		for(i = 0; i < DataLenght; i++)
		{
			sprintf((char*)buffer_string, "Plain Text: %llx\n", plainText[i]);
			USART_Puts(USART2, buffer_string);	
			
			/* gonderimin ardindan karakter dizisi temizleniyor */
			for(clean_cnt = 0; clean_cnt < MAX_STRLEN; clean_cnt++)
			{
				buffer_string[clean_cnt] = NULL; // karakter dizisinin ilgili sayaca ait elemani NULL yapiliyor
			}		
			
			Simple_Delay_ms(100);
		}


		Simple_Delay_ms(100);
		
		for(i = 0; i < DataLenght; i++)
		{
			sprintf((char*)buffer_string,"Cipher Text: %llx\n", cipherText[i]);
			USART_Puts(USART2, buffer_string);	
			
			/* gonderimin ardindan karakter dizisi temizleniyor */
			for(clean_cnt = 0; clean_cnt < MAX_STRLEN; clean_cnt++)
			{
				buffer_string[clean_cnt] = NULL; // karakter dizisinin ilgili sayaca ait elemani NULL yapiliyor
			}				
			
			Simple_Delay_ms(100);
		}
		
		Simple_Delay_ms(100);
		
		for(i = 0; i < DataLenght; i++)
		{
			if(plainText[i] == decryptedText[i])
			{
				sprintf((char*)buffer_string,"Decrypted Text: %llx - OK\n", decryptedText[i]);
				USART_Puts(USART2, buffer_string);	
				
				/* gonderimin ardindan karakter dizisi temizleniyor */
				for(clean_cnt = 0; clean_cnt < MAX_STRLEN; clean_cnt++)
				{
					buffer_string[clean_cnt] = NULL; // karakter dizisinin ilgili sayaca ait elemani NULL yapiliyor
				}	
				
				Simple_Delay_ms(100);
			}
			else
			{
				sprintf((char*)buffer_string,"Decrypted Text: %llx - F\n", decryptedText[i]);
				USART_Puts(USART2, buffer_string);
				
				/* gonderimin ardindan karakter dizisi temizleniyor */
				for(clean_cnt = 0; clean_cnt < MAX_STRLEN; clean_cnt++)
				{
					buffer_string[clean_cnt] = NULL; // karakter dizisinin ilgili sayaca ait elemani NULL yapiliyor
				}					
				
				Simple_Delay_ms(100);
			}
		}
		
		Simple_Delay_ms(1000);	
	}	// while(1)
	
} // int main()
