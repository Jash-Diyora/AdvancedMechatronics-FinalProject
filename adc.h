/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H_
#define __ADC_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_mort2.h"
#include "main.h"

/* Macros ------------------------------------------------------------------*/



/* Function Definitions-------------------------------------------------------*/
void initADC3_5_NoInterrupt();
uint32_t checkNewValueInSoftware();
void startADCConversion();
void initGpioF7AsAnalog();
void initGpioB0AsOutput();
void toggleGPIOB0();
void initGpioB0AsOutput();
void setGPIOB0();
void clearGPIOB0();
void initADC3_567_withDMA();
void initDMAForAdc3_3channels();
void enableDMAForAdc3_3channels();
void initGpioF789AsAnalog();
uint16_t returnADC3StoredValue(uint8_t index);

#ifdef __cplusplus
}
#endif

#endif /*__TIMER3_H */