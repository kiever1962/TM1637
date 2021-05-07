#include "main.h"
#include "stm32f1xx_hal.h"

uint32_t CRC32_ForBytes(uint8_t *pData, uint32_t uLen);

#define UNUSED(x) ((void)(x))

/**
 * @brief  CRC functions
 */
#define __HAL_RCC_CRC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_CRCEN);\
                                        /* Delay after an RCC peripheral clock enabling */\
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_CRCEN);\
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_CRC_CLK_DISABLE()       (RCC->AHBENR &= ~(RCC_AHBENR_CRCEN))

#define CRC32_POLYNOMIAL                        ((uint32_t)0xEDB88320)
#define RCC_CRC_BIT                             ((uint32_t)0x00001000)

void CRC_ResetDR(void)
{
  /* Reset CRC generator */
  CRC->CR |= CRC_CR_RESET;
}


/**
 * @brief  Calc CRC32 for data in bytes
 * @param  pData Buffer pointer
 * @param  uLen  Buffer Length
 * @retval CRC32 Checksum
 */
uint32_t revbit(uint32_t uData)
{
    uint32_t uRevData = 0,uIndex = 0;
    uRevData |= ((uData >> uIndex) & 0x01);
    for(uIndex = 1;uIndex < 32;uIndex++)
    {
        uRevData <<= 1;
        uRevData |= ((uData >> uIndex) & 0x01);
    }
    return uRevData;
}

uint32_t CRC32_ForBytes(uint8_t *pData,uint32_t uLen)
{
    uint32_t uIndex= 0,uData = 0,i;
    uIndex = uLen >> 2;

    __HAL_RCC_CRC_CLK_ENABLE();

    /* Reset CRC generator */
    CRC_ResetDR();

    while(uIndex--)
    {
#ifdef USED_BIG_ENDIAN
        uData = __REV((uint32_t*)pData);
#else
        ((uint8_t *)&uData)[0] = pData[0];
        ((uint8_t *)&uData)[1] = pData[1];
        ((uint8_t *)&uData)[2] = pData[2];
        ((uint8_t *)&uData)[3] = pData[3];
#endif
        pData += 4;
        uData = revbit(uData);
        CRC->DR = uData;
    }
    uData = revbit(CRC->DR);
    uIndex = uLen & 0x03;
    while(uIndex--)
    {
        uData ^= (uint32_t)*pData++;
        for(i = 0;i < 8;i++)
          if (uData & 0x1)
            uData = (uData >> 1) ^ CRC32_POLYNOMIAL;
          else
            uData >>= 1;
    }

    __HAL_RCC_CRC_CLK_DISABLE();

    return uData^0xFFFFFFFF;
}


