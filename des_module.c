/**
  ******************************************************************************
  * @file    des_module.c
  * @author  Batuhan KINDAN
  * @version V1.0.0
  * @date    02 Aralik 2017
  * @brief   Bu dosya DES (Data Encryption Standart) algoritmasinin gerceklenmesine
  * 		 		 ait fonksiyonlari icerir
  ******************************************************************************
  */

#include <stdint.h> // integer veri tipi icin header
#include "des_module.h" // des module file header

// DES - Permuted choice 1 matrix
const uint8_t PC_1[56] = {	57,   49,    41,   33,    25,    17,    9,
														 1,   58,    50,   42,    34,    26,   18,
														10,    2,    59,   51,    43,    35,   27,
														19,   11,     3,   60,    52,    44,   36,
														63,   55,    47,   39,    31,    23,   15,
														 7,   62,    54,   46,    38,    30,   22,
														14,    6,    61,   53,    45,    37,   29,
														21,   13,     5,   28,    20,    12,    4	};

// DES - Permuted choice 2 matrix
const uint8_t PC_2[48] = { 	14,    17,   11,    24,     1,    5,
														 3,    28,   15,     6,    21,   10,
														23,    19,   12,     4,    26,    8,
														16,     7,   27,    20,    13,    2,
														41,    52,   31,    37,    47,   55,
														30,    40,   51,    45,    33,   48,
														44,    49,   39,    56,    34,   53,
														46,    42,   50,    36,    29,   32   };

// DES - Initial Permutation Matrix
const uint8_t IP[64] = {  58,    50,   42,   34,   26,   18,   10,   2,
													60,    52,   44,   36,   28,   20,   12,   4,
													62,    54,   46,   38,   30,   22,   14,   6,
													64,    56,   48,   40,   32,   24,   16,   8,
													57,    49,   41,   33,   25,   17,    9,   1,
													59,    51,   43,   35,   27,   19,   11,   3,
													61,    53,   45,   37,   29,   21,   13,   5,
													63,    55,   47,   39,   31,   23,   15,   7  };

// IP^-1 Permutation Matrix
const uint8_t IP_[64] = {  40,    8,   48,   16,   56,   24,   64,  32,
													 39,    7,   47,   15,   55,   23,   63,  31,
													 38,    6,   46,   14,   54,   22,   62,  30,
													 37,    5,   45,   13,   53,   21,   61,  29,
													 36,    4,   44,   12,   52,   20,   60,  28,
													 35,    3,   43,   11,   51,   19,   59,  27,
													 34,    2,   42,   10,   50,   18,   58,  26,
													 33,    1,   41,    9,   49,   17,   57,  25  };


// DES - Sub Keys 0-15 (Kn)
uint64_t Sub_Keys[16];

// 28 bits left part of subkeys
uint32_t Cn[17];

// 28 bits right part of subkeys
uint32_t Dn[17];

// DES - Bit Shift Table
const uint8_t Shift_Table[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

// DES - E bit selection table
const uint8_t E_Bit_Selection_Table[48] = { 32,     1,    2,     3,     4,    5,
																						 4,     5,    6,     7,     8,    9,
																						 8,     9,   10,    11,    12,   13,
																						12,    13,   14,    15,    16,   17,
																						16,    17,   18,    19,    20,   21,
																						20,    21,   22,    23,    24,   25,
																						24,    25,   26,    27,    28,   29,
																						28,    29,   30,    31,    32,    1 };

// S-Box S 1 to 8
const uint8_t SBox[8][4][16] =
{
	{
			{  14,  4,  13,  1,   2, 15,  11,  8,   3,  10,   6,  12,  5,  9,  0,  7 },
			{		0, 15,   7,  4,  14,  2,  13,  1,  10,   6,  12,  11,  9,  5,  3,  8 },
			{		4,  1,  14,  8,  13,  6,   2, 11,  15,  12,   9,   7,  3, 10,  5,  0 },
			{  15, 12,   8,  2,   4,  9,   1,  7,   5,  11,   3,  14, 10,  0,  6, 13 }
	},

	{
			{  15,  1,   8, 14,   6, 11,   3,  4,   9,   7,   2,  13, 12,  0,  5, 10 },
			{		3, 13,   4,  7,  15,  2,   8, 14,  12,   0,   1,  10,  6,  9, 11,  5 },
			{		0, 14,   7, 11,  10,  4,  13,  1,   5,   8,  12,   6,  9,  3,  2, 15 },
			{  13,  8,  10,  1,   3, 15,   4,  2,  11,   6,   7,  12,  0,  5, 14,  9 }
	},

	{
			{  10,  0,   9, 14,   6,  3,  15,  5,   1,  13,  12,   7,  11,  4,  2,  8 },
			{  13,  7,   0,  9,   3,  4,   6, 10,   2,   8,   5,  14,  12, 11, 15,  1 },
			{  13,  6,   4,  9,   8, 15,   3,  0,  11,   1,   2,  12,   5, 10, 14,  7 },
			{   1, 10,  13,  0,   6,  9,   8,  7,   4,  15,  14,   3,  11,  5,  2, 12 }

	},

	{
			{   7, 13,  14,  3,   0,  6,   9, 10,   1,   2,   8,   5,  11, 12,  4, 15 },
			{  13,  8,  11,  5,   6, 15,   0,  3,   4,   7,   2,  12,   1, 10, 14,  9 },
			{  10,  6,   9,  0,  12, 11,   7, 13,  15,   1,   3,  14,   5,  2,  8,  4 },
			{   3, 15,   0,  6,  10,  1,  13,  8,   9,   4,   5,  11,  12,  7,  2, 14 }
	},

	{
			{   2, 12,   4,  1,   7, 10,  11,  6,   8,   5,   3,  15,  13,  0, 14,  9 },
			{  14, 11,   2, 12,   4,  7,  13,  1,   5,   0,  15,  10,   3,  9,  8,  6 },
			{   4,  2,   1, 11,  10, 13,   7,  8,  15,   9,  12,   5,   6,  3,  0, 14 },
			{  11,  8,  12,  7,   1, 14,   2, 13,   6,  15,   0,   9,  10,  4,  5,  3 }
	},

	{
			{  12,  1,  10, 15,   9,  2,   6,  8,   0,  13,   3,   4,  14,  7,  5, 11 },
			{  10, 15,   4,  2,   7, 12,   9,  5,   6,   1,  13,  14,   0, 11,  3,  8 },
			{   9, 14,  15,  5,   2,  8,  12,  3,   7,   0,   4,  10,   1, 13, 11,  6 },
			{   4,  3,   2, 12,   9,  5,  15, 10,  11,  14,   1,   7,   6,  0,  8, 13 }
	},

	{
			{   4, 11,   2, 14,  15,  0,   8, 13,   3,  12,   9,   7,   5, 10,  6,  1 },
			{  13,  0,  11,  7,   4,  9,   1, 10,  14,   3,   5,  12,   2, 15,  8,  6 },
			{   1,  4,  11, 13,  12,  3,   7, 14,  10,  15,   6,   8,   0,  5,  9,  2 },
			{   6, 11,  13,  8,   1,  4,  10,  7,   9,   5,   0,  15,  14,  2,  3, 12 }
	},

	{
			{   13,  2,   8,  4,   6, 15,  11,  1,  10,   9,   3,  14,   5,  0, 12,  7 },
			{    1, 15,  13,  8,  10,  3,   7,  4,  12,   5,   6,  11,   0, 14,  9,  2 },
			{    7, 11,   4,  1,   9, 12,  14,  2,   0,   6,  10,  13,  15,  3,  5,  8 },
			{    2,  1,  14,  7,   4, 10,   8, 13,  15,  12,   9,   0,   3,  5,  6, 11 }
	}

};

// The Permutation
const uint8_t P[32] = {  16,   7,  20,  21,
												 29,  12,  28,  17,
													1,  15,  23,  26,
													5,  18,  31,  10,
													2,   8,  24,  14,
												 32,  27,   3,   9,
												 19,  13,  30,   6,
												 22,  11,   4,  25 };


/**
  * @brief  Bu fonksiyon; istenilen bir DES (Data Encryption Standart) modunda, kendisine verilen
  *         veri dizisini sifreler ve sifrelenmis veriyi kendisine verilen adrese yerlestirir
  * @param  DESx
  * @param 	plainData
  * @param  cipherText
  * @retval none
  */
void DES_Encrypt_Data(DES_InitTypedef* DESx, uint64_t* plainData, uint64_t* cipherData, uint16_t lenght)
{
	uint16_t i = 0;
	uint64_t chainBuffer = 0x0000000000000000;

	switch(DESx->Mode)
	{
		// Electronic Code Book
		case ECB:

			DES_Get_Subkeys(DESx);

			for(i = 0; i < lenght; i++)
			{
				cipherData[i] = DES_Encode_BlockData(plainData[i]);
			}

			break;

		// Cipher Block Chaining
		case CBC:

			DES_Get_Subkeys(DESx);

			chainBuffer = (DESx->IV ^ plainData[0]);

			cipherData[0] = DES_Encode_BlockData(chainBuffer);

			for(i = 1; i < lenght; i++)
			{
				chainBuffer = (cipherData[i-1] ^ plainData[i]);

				cipherData[i] = DES_Encode_BlockData(chainBuffer);
			}

			break;

		default:

			break;
	}

}

/**
  * @brief  Bu fonksiyon; istenilen bir DES (Data Encryption Standart) modunda, kendisine verilen
  *         sifrelenmis veriyi cozerek kendisine verilen adrese yerlestirir
  * @param  DESx
  * @param 	plainData
  * @param  cipherText
  * @retval none
  */
void DES_Decrypt_Data(DES_InitTypedef* DESx, uint64_t* cipherData, uint64_t* plainData, uint16_t lenght)
{
	uint16_t i = 0;
	uint64_t chainBuffer = 0x0000000000000000;

	switch(DESx->Mode)
	{
		// Electronic Code Book
		case ECB:

			DES_Get_Subkeys(DESx);

			for(i = 0; i < lenght; i++)
			{
				plainData[i] = DES_Decode_BlockData(cipherData[i]);
			}

			break;

		// Cipher Block Chaining
		case CBC:

			DES_Get_Subkeys(DESx);

			chainBuffer = DES_Decode_BlockData(cipherData[0]);

			plainData[0] = (chainBuffer ^ DESx->IV);

			for(i = 1; i < lenght; i++)
			{
				chainBuffer = DES_Decode_BlockData(cipherData[i]);

				plainData[i] = (chainBuffer ^ cipherData[i-1]);
			}

			break;

		default:

			break;

	}

}

/**
  * @brief  Bu fonksiyon; parametre olarak verilen DES anahtarini kullanarak DES sifreleme ve sifre cozme
  *         isleminde kullanilacak alt anahtarlari uretir ve Sub_Keys dizisine yerlestirir
  * @param  DESx
  * @retval none
  */
void DES_Get_Subkeys(DES_InitTypedef* DESx)
{
	uint64_t permutedKey = 0x0000000000000000;
	uint64_t bitShift_Buffer = 0x0000000000000000;
	uint64_t cnBuffer = 0x0000000000000000;
	uint64_t dnBuffer = 0x0000000000000000;
	uint64_t preSubKey = 0x0000000000000000;
	uint8_t i = 0;
	uint8_t j = 0;

	/* alt anahtar dizisini temizle */
	for(i = 0; i < 16; i++)
	{
		Sub_Keys[i] = 0x0000000000000000;
	}

	/* DES anahtari K'nin PC_1 matrisi ile permute edilerek K+ permute edilmis anahtarin bulunmasi */
	for(i = 0; i < 56; i++)
	{
		/* PC_1 matrisinin anahtar uzerinde adresledigi bit degeri bulunarak sira ile permute 
     * edilmis anahtar degiskenine ekleniyor */
		bitShift_Buffer = 0x8000000000000000;
		bitShift_Buffer = (bitShift_Buffer >> (PC_1[i]-1));
		bitShift_Buffer = bitShift_Buffer & (DESx->Key);
		bitShift_Buffer = (bitShift_Buffer << (PC_1[i]-1));
		bitShift_Buffer = (bitShift_Buffer >> i);

		permutedKey = (permutedKey | bitShift_Buffer);
	}

  /* bit kaydirma islemi icin baslangic degerleri olan, alt anahtarlarin 0 numarali 
   * sol ve sag parcalarinin elde edilmesi */
	Cn[0] = (0xFFFFFFF000000000 & permutedKey) >> 32;
	Dn[0] = (0x0000000FFFFFFF00 & permutedKey) >> 4;

	/* permutasyon oncesi alt anahtar parcalarinin hesaplanmasi */
	for(i = 0; i < 16; i++)
	{
		/* her bir alt anahtar parcasi bir onceki parcanin bit kaydirma tablosuna gore
     * dairesel bit kaydirmasiyla hesaplaniyor */
		Cn[i+1] = DES_Subkey_BitShifter(Cn[i], Shift_Table[i]);
		Dn[i+1] = DES_Subkey_BitShifter(Dn[i], Shift_Table[i]);
	}

	/* 16 adet alt anahtar hesaplaniyor */
	for(i = 0; i < 16; i++)
	{
		cnBuffer = Cn[i+1];
		cnBuffer = (cnBuffer << 32);
		dnBuffer = Dn[i+1];
		dnBuffer = (dnBuffer << 4);
		preSubKey = (cnBuffer | dnBuffer);

		/* ilgili alt anahtar PC_2 matrisi ile permute edilerek son halini aliyor */
		for(j = 0; j < 48; j++)
		{
			bitShift_Buffer = 0x8000000000000000;
			bitShift_Buffer = (bitShift_Buffer >> (PC_2[j]-1));
			bitShift_Buffer = bitShift_Buffer & (preSubKey);
			bitShift_Buffer = (bitShift_Buffer << (PC_2[j]-1));
			bitShift_Buffer = (bitShift_Buffer >> j);

			Sub_Keys[i] = (Sub_Keys[i] | bitShift_Buffer);
		}

	}

} // Get_Subkeys Function



/**
  * @brief  Bu fonksiyon; DES hesaplama rutini sirasinda kullanilan dairesel sola kaydirma islemini gerceklestirir        
  * @param  value
  * @param  shiftValue
  * @retval buffer
  */
uint32_t DES_Subkey_BitShifter(uint32_t value, uint8_t shiftValue)
{
	uint32_t buffer = 0x00000000;

	buffer = (value << shiftValue) | ((value >> (32 - shiftValue)) << 4);

	return buffer;
} // DES_Subkey_BitShifter Function


/**
  * @brief  Bu fonksiyon 64 bitlik bir blok veriyi sifreler
  * @param  plainData
  * @retval encodedData
  */
uint64_t DES_Encode_BlockData(uint64_t plainData)
{
	uint64_t encodedData = 0x0000000000000000; // geri dondurulecek sifre blok metin degiskeni
	uint64_t bitShift_Buffer = 0x0000000000000000; // bit kaydirma islemi icin tutucu degisken
	uint64_t permutedData = 0x0000000000000000; // permutasyon islemine tabi tutulacak veri icin tutucu degisken
	uint64_t pre_PermutedData = 0x0000000000000000; // permutasyon oncesi islemler icin veri tutucu degiskeni
	uint32_t ln = 0x00000000; // mesajin ilgili iterasyona air 32 bitlik sol parcasi
	uint32_t ln_Old = 0x00000000; // mesajin ilgili iterasyondan bir onceki duruma ait 32 bitlik sol parcasi
	uint32_t rn = 0x00000000; // mesajin ilgili iterasyona air 32 bitlik sag parcasi
	uint32_t rn_Old = 0x00000000; // mesajin ilgili iterasyondan bir onceki duruma ait 32 bitlik sag parcasi
	uint8_t i = 0; // genel maksat sayac


	/* IP (Initial Permutation) matrisi ile mesaj (M) verisinin ilk permutasyon islemi yapililarak
   * permute edilmis mesaj (M+) elde ediliyor */
	for(i = 0; i < 64; i++)
	{
		bitShift_Buffer = 0x8000000000000000;
		bitShift_Buffer = (bitShift_Buffer >> (IP[i]-1));
		bitShift_Buffer = bitShift_Buffer & (plainData);
		bitShift_Buffer = (bitShift_Buffer << (IP[i]-1));
		bitShift_Buffer = (bitShift_Buffer >> i);

		encodedData = (encodedData | bitShift_Buffer);
	}


	/* permute edilmis mesajin 32 bitlik sag ve sol parcalara ayrilmasi */
	ln_Old = (0xFFFFFFFF00000000 & encodedData) >> 32;
	rn_Old = (0x00000000FFFFFFFF & encodedData);

	/* permute edilmis ve parcalara ayrilmis mesaj verisinin 16 kez F fonksiyonu yardimi ile
   * yer degistirerek sifreleme rutini iterasyonunun yapilmasi */
	for(i = 0; i < 16; i++)
	{
		ln = rn_Old;
		rn = ln_Old ^ DES_F_Function(rn_Old, i);

		rn_Old = rn;
		ln_Old = ln;
	}

  /* R16 ve L16 parcalari birlestirilerek permutasyon oncesi sifrelenmis data elde ediliyor */
	pre_PermutedData = rn_Old;
	pre_PermutedData = (pre_PermutedData << 32);
	pre_PermutedData = (pre_PermutedData | ln_Old);

	/* IP^-1 matrisi ile sifrelenecek dataya son permutasyon islemi de uygulaniyor */
	for(i = 0; i < 64; i++)
	{
		bitShift_Buffer = 0x8000000000000000;
		bitShift_Buffer = (bitShift_Buffer >> (IP_[i]-1));
		bitShift_Buffer = bitShift_Buffer & (pre_PermutedData);
		bitShift_Buffer = (bitShift_Buffer << (IP_[i]-1));
		bitShift_Buffer = (bitShift_Buffer >> i);

		permutedData = (permutedData | bitShift_Buffer);
	}

	encodedData = permutedData;

	return encodedData;
} // DES_Encode_BlockData function


/**
  * @brief  Bu fonksiyon 64 bitlik sifrelenmis bir blok datanin sifresini cozer
  * @param  plainData
  * @retval encodedData
  */
uint64_t DES_Decode_BlockData(uint64_t plainData)
{
	uint64_t decodedData = 0x0000000000000000; // geri dondurulecek sifre blok metin degiskeni
	uint64_t bitShift_Buffer = 0x0000000000000000; // bit kaydirma islemi icin tutucu degisken
	uint64_t permutedData = 0x0000000000000000; // permutasyon islemine tabi tutulacak veri icin tutucu degisken
	uint64_t pre_PermutedData = 0x0000000000000000; // permutasyon oncesi islemler icin veri tutucu degiskeni
	uint32_t ln = 0x00000000; // mesajin ilgili iterasyona air 32 bitlik sol parcasi
	uint32_t ln_Old = 0x00000000; // mesajin ilgili iterasyondan bir onceki duruma ait 32 bitlik sol parcasi
	uint32_t rn = 0x00000000; // mesajin ilgili iterasyona air 32 bitlik sag parcasi
	uint32_t rn_Old = 0x00000000; // mesajin ilgili iterasyondan bir onceki duruma ait 32 bitlik sag parcasi
	uint8_t i = 0; // genel maksat sayac


	/* IP (Initial Permutation) matrisi ile mesaj (M) verisinin ilk permutasyon islemi yapililarak
   * permute edilmis mesaj (M+) elde ediliyor */
	for(i = 0; i < 64; i++)
	{
		bitShift_Buffer = 0x8000000000000000;
		bitShift_Buffer = (bitShift_Buffer >> (IP[i]-1));
		bitShift_Buffer = bitShift_Buffer & (plainData);
		bitShift_Buffer = (bitShift_Buffer << (IP[i]-1));
		bitShift_Buffer = (bitShift_Buffer >> i);

		decodedData = (decodedData | bitShift_Buffer);
	}

	/* permute edilmis mesajin 32 bitlik sag ve sol parcalara ayrilmasi */
	ln_Old = (0xFFFFFFFF00000000 & decodedData) >> 32;
	rn_Old = (0x00000000FFFFFFFF & decodedData);

	/* permute edilmis ve parcalara ayrilmis mesaj verisinin 16 kez F fonksiyonu yardimi ile
   * yer degistirerek sifreleme rutininin aynisi olan sifre cozme iterasyonunun yapilmasi */
	for(i = 0; i < 16; i++)
	{
		ln = rn_Old;
		rn = ln_Old ^ DES_F_Function(rn_Old, (15-i));

		rn_Old = rn;
		ln_Old = ln;
	}

  /* R16 ve L16 parcalari birlestirilerek permutasyon oncesi data elde ediliyor */
	pre_PermutedData = rn_Old;
	pre_PermutedData = (pre_PermutedData << 32);
	pre_PermutedData = (pre_PermutedData | ln_Old);

	decodedData = pre_PermutedData;

	/* IP^-1 matrisi ile sifresi cozulecek dataya son permutasyon islemi de uygulaniyor */
	for(i = 0; i < 64; i++)
	{
		bitShift_Buffer = 0x8000000000000000;
		bitShift_Buffer = (bitShift_Buffer >> (IP_[i]-1));
		bitShift_Buffer = bitShift_Buffer & (decodedData);
		bitShift_Buffer = (bitShift_Buffer << (IP_[i]-1));
		bitShift_Buffer = (bitShift_Buffer >> i);

		permutedData = (permutedData | bitShift_Buffer);
	}

	decodedData = permutedData;

	return decodedData;
}

/**
  * @brief  DES (Data Encryption Standart) F fonksiyonu rutini
  * @param  input
  * @param  iterationNumber
  * @retval result
  */
uint32_t DES_F_Function(uint32_t input, uint8_t iterationNumber)
{
  uint32_t result = 0x00000000; // geri dondurulecek fonksiyon cikis degeri
  uint32_t resultBuffer = 0x00000000; // cikis degeri ustunde yapilacak islemler icin tutucu degisken
  uint32_t bitShift_Buffer32 = 0x00000000; // 32 bitlik datalar icin bit kaydirma tutucu degiskeni
  uint64_t expandedInput = 0x0000000000000000; // 48 bite genisletilmis veri icin tutucu degisken
  uint64_t inputBuffer = 0x0000000000000000; // giris verisi uzerindeki islemler icin tutucu degisken
  uint64_t bitShift_Buffer = 0x0000000000000000; // bit kaydirma icin tutucu degisken
  uint64_t B_Buffer = 0x0000000000000000; // 6 bitlik B verisi icin tutucu degisken

	uint8_t i = 0;
	uint8_t S_Row = 0;
	uint8_t S_Column = 0;

	/* 32 bitlik giris verisinin E-bit secim tablosu yardimi ile 48 bite genisletilmesi */
	for(i = 0; i < 48; i++)
	{
		bitShift_Buffer = 0x8000000000000000;
		inputBuffer = input;
		inputBuffer = (inputBuffer << 32);
		bitShift_Buffer = (bitShift_Buffer >> (E_Bit_Selection_Table[i]-1));
		bitShift_Buffer = (bitShift_Buffer & inputBuffer);
		bitShift_Buffer = (bitShift_Buffer << (E_Bit_Selection_Table[i]-1));
		bitShift_Buffer = (bitShift_Buffer >> i);

		expandedInput = (expandedInput | bitShift_Buffer);
	}

  /* genisletilen verinin iterasyon numarasina gore ilgili alt anahtar ile XOR'lanmasi */
	expandedInput = (expandedInput ^ Sub_Keys[iterationNumber]);


	/* 48 bitlik genisletilmis veriden 8 adet 6 bitlik B degeri elde edilmesi ve bu B verileri ile
   * S-Box'lar kullanilarak 8 adet 6 bitlik verinin 4 bite dusurulup 8x4 = 32 bitlik
   * permutasyon oncesi F fonksiyon ciktisinin elde edilmesi */
	for(i = 0; i < 8; i++)
	{
		B_Buffer = 0xFC; // 0b11111100
		B_Buffer = (B_Buffer << 56);
		B_Buffer = (B_Buffer >> (i*6));
		B_Buffer = ((expandedInput & B_Buffer) >> (56 - (i*6)));

		/* S-Box sutun adresi 6 bitlik B verisinin ilk ve son bitine bakilarak adresleniyor */
		switch(B_Buffer & 0x84) // 0b10000100
		{
			case 0x00: // 0b00000000
					S_Row = 0;
				break;

			case 0x04: // 0b00000100
					S_Row = 1;
				break;

			case 0x80: // 0b10000000
					S_Row = 2;
				break;

			case 0x84: // 0b10000100
					S_Row = 3;
				break;

			default:
					// bu satir bilerek bos birakildi
				break;
		}

		/* ilgili 6 bitlik B datasinin ortadaki 4 biti ile de S-Box dizisinin kolon adresi elde ediliyor */
		S_Column = ((B_Buffer & 0x78) >> 3); // 0b01111000

		/* ilgili dongu iterasyonuna ait S kutusunda adreslenen veri bulunarak 32 bitlik cikis
     * tutucu degiskeni uzerinde ilgili noktaya kaydiriliyor ve cikis verisine ekleniyor*/
		resultBuffer = 0x00000000;
		resultBuffer = SBox[i][S_Row][S_Column];
		resultBuffer = (resultBuffer << (28 - (4*i)));

		result = (result | resultBuffer);

	}
	
  /* cikis verisinin permute edilmek uzere tutucu degiskene atilmasi */
	resultBuffer = result;
	result = 0x00000000;

	/* P matrisi ile yapilan son permutasyon isleminin ardindan F fonksiyon ciktisinin elde edilmesi */
	for(i = 0; i < 32; i++)
	{
		bitShift_Buffer32 = 0x80000000;
		bitShift_Buffer32 = (bitShift_Buffer32 >> (P[i]-1));
		bitShift_Buffer32 = (bitShift_Buffer32 & resultBuffer);
		bitShift_Buffer32 = (bitShift_Buffer32 << (P[i]-1));
		bitShift_Buffer32 = (bitShift_Buffer32 >> i);

		result = (result | bitShift_Buffer32);
	}

	return result;
}

