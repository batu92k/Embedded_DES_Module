/**
  ******************************************************************************
  * @file    des_module.h
  * @author  Batuhan KINDAN
  * @version V1.0.0
  * @date    02 Aralik 2017
  * @brief   Bu dosya DES (Data Encryption Standart) algoritmasinin gerceklenmesine
  * 		 		 ait fonksiyonlarin prototiplerini icerir
  ******************************************************************************
  */

#ifndef DES_MODULE_H
#define DES_MODULE_H

#include <stdint.h>

/* DES Modes of Operation
 * ECB ( Electronic Code Book )
 * CBC ( Cipher Block Chaining )
 * */
typedef enum
{
	ECB = 0x01,
	CBC = 0x02,
	DEFAULT = 0x00

} DES_ModeTypedef;

/* DES Main Struct */
typedef struct
{
	uint64_t IV;
	uint64_t Key;
	uint8_t Mode;

} DES_InitTypedef;

void DES_Encrypt_Data(DES_InitTypedef* DESx, uint64_t* plainData, uint64_t* cipherData, uint16_t lenght);
void DES_Decrypt_Data(DES_InitTypedef* DESx, uint64_t* cipherData, uint64_t* plainData, uint16_t lenght);
void DES_Get_Subkeys(DES_InitTypedef* DESx);
uint32_t DES_Subkey_BitShifter(uint32_t value, uint8_t shiftValue);
uint64_t DES_Encode_BlockData(uint64_t plainData);
uint64_t DES_Decode_BlockData(uint64_t plainData);
uint32_t DES_F_Function(uint32_t input, uint8_t iterationNumber);

#endif // SRC_DES_MODULE_H_
