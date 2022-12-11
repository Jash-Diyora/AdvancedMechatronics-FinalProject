
#include "debug_mort.h"
#include "stdint.h"
#include "stm32f4xx_rcc_mort.h"


/* DMA Registers */
#define DMA_BASE_ADDRESS ((uint32_t)0x40026400)
#define DMA2_LISR_ADDRESS (DMA_BASE_ADDRESS + 0x00)
#define DMA2_HISR_ADDRESS (DMA_BASE_ADDRESS + 0x04)
#define DMA2_LIFCR_ADDRESS (DMA_BASE_ADDRESS + 0x08)
#define DMA2_HIFCR_ADDRESS (DMA_BASE_ADDRESS + 0x0C)
#define DMA2_S0CR_ADDRESS (DMA_BASE_ADDRESS + 0x10)
#define DMA2_S0NDTR_ADDRESS (DMA_BASE_ADDRESS + 0x14)
#define DMA2_S0PAR_ADDRESS (DMA_BASE_ADDRESS + 0x18)
#define DMA2_S0M0AR_ADDRESS (DMA_BASE_ADDRESS + 0x1C)
#define DMA2_S0M1AR_ADDRESS (DMA_BASE_ADDRESS + 0x20)
#define DMA2_S0FCR_ADDRESS (DMA_BASE_ADDRESS + 0x24)

/* Bit masks */
#define DMA_SxCR_CHANNEL_2_SELECT (((uint32_t)2) << 25)
#define DMA_SxCR_MSIZE_HALF_WORD (((uint32_t)1) << 13)
#define DMA_SxCR_PSIZE_HALF_WORD (((uint32_t)1) << 11)
#define DMA_SxCR_MINC_INCREMENT (((uint32_t)1) << 10)
#define DMA_SxCR_CIRC_ENABLE (((uint32_t)1) << 8)
#define DMA_SxCR_DIR_PERTOMEM 0
#define DMA_SxCR_STREAM_ENABLE 1

/* ADC Macros */
#define ADC_BASE_ADDRESS ((uint32_t)0x40012000)
#define ADC_3_BASE_ADDRESS (ADC_BASE_ADDRESS + 0x200)
#define ADC_3_SR_REGISTER (ADC_3_BASE_ADDRESS + 0x00)
#define ADC_3_CR1_REGISTER (ADC_3_BASE_ADDRESS + 0x04)
#define ADC_3_CR2_REGISTER (ADC_3_BASE_ADDRESS + 0x08)
#define ADC_3_SMPR1_REGISTER (ADC_3_BASE_ADDRESS + 0x0C)
#define ADC_3_SMPR2_REGISTER (ADC_3_BASE_ADDRESS + 0x10)
#define ADC_3_SQR1_REGISTER (ADC_3_BASE_ADDRESS + 0x2C)
#define ADC_3_SQR2_REGISTER (ADC_3_BASE_ADDRESS + 0x30)
#define ADC_3_SQR3_REGISTER (ADC_3_BASE_ADDRESS + 0x34)
#define ADC_3_DR_REGISTER (ADC_3_BASE_ADDRESS + 0x4C)

#define ADC_COMMON_CCR_REGISTER (ADC_BASE_ADDRESS + 0X300 + 0X04)

#define ADC_1_CONVERSIONS (0 << 20) | (0 << 21) | (0 << 22) | (0 << 23)
#define ADC_PREESCALER_4 (1 << 16) | (0 << 17)
#define ADC_EOCS (1 << 10)
#define ADC_EOC (1 << 1)
#define ADC_SMP_5_MX (1 << 17) | (1 << 16) | (1 << 15)
#define ADC_ADON (1 << 0)
#define ADC_CHANNEL_5 (1 << 0) | (0 << 1) | (1 << 2) | (0 << 3)
#define ADC_SWSTART (1 << 30)

#define ADC_3_CONVERSIONS (0 << 20) | (1 << 21) | (0 << 22) | (0 << 23)
#define ADC_SMP_6_MX (1 << 18) | (1 << 19) | (1 << 20)
#define ADC_SMP_7_MX (1 << 21) | (1 << 22) | (1 <<23)
#define ADC_SCAN (1 << 8)
#define ADC_DMA (1 << 8)
#define ADC_DDS (1 << 9)
#define ADC_CONT (1 << 1)
#define ADC_CHANNEL_6 (0 << 5) | (1 << 6) | (1 << 7) | (0 << 8)
#define ADC_CHANNEL_7 (1 << 10) | (1 << 11) | (1 << 12) | (0 << 13)

// PORTB Macros
#define PORTB_BASE_ADDRESS ((uint32_t)0x40020400)
#define PORTB_MODER_REGISTER (PORTB_BASE_ADDRESS + 0x00)
#define PORTB_OTYPER_REGISTER (PORTB_BASE_ADDRESS + 0x04)
#define PORTB_OSPEEDR_REGISTER (PORTB_BASE_ADDRESS + 0x08)
#define PORTB_PUPDR_REGISTER (PORTB_BASE_ADDRESS + 0x0C)
#define PORTB_IDR_REGISTER (PORTB_BASE_ADDRESS + 0x10)
#define PORTB_ODR_REGISTER (PORTB_BASE_ADDRESS + 0x14)
#define PORTB_BSRRL_REGISTER (PORTB_BASE_ADDRESS + 0x18)
#define PORTB_BSRR_REGISTER (PORTB_BASE_ADDRESS + 0x18)
#define PORTB_BSRRH_REGISTER (PORTB_BASE_ADDRESS + 0x1A)
#define PORTB_LCKR_REGISTER (PORTB_BASE_ADDRESS + 0x1C)
#define PORTB_AFR1_REGISTER (PORTB_BASE_ADDRESS + 0x20)
#define PORTB_AFR2_REGISTER (PORTB_BASE_ADDRESS + 0x24)

// PORTF Macros
#define PORTF_BASE_ADDRESS ((uint32_t)0x40021400)
#define PORTF_MODER_REGISTER (PORTB_BASE_ADDRESS + 0x00)
#define PORTF_OTYPER_REGISTER (PORTB_BASE_ADDRESS + 0x04)
#define PORTF_OSPEEDR_REGISTER (PORTB_BASE_ADDRESS + 0x08)
#define PORTF_PUPDR_REGISTER (PORTB_BASE_ADDRESS + 0x0C)
#define PORTF_IDR_REGISTER (PORTB_BASE_ADDRESS + 0x10)
#define PORTF_ODR_REGISTER (PORTB_BASE_ADDRESS + 0x14)
#define PORTF_BSRRL_REGISTER (PORTB_BASE_ADDRESS + 0x18)
#define PORTF_BSRR_REGISTER (PORTB_BASE_ADDRESS + 0x18)
#define PORTF_BSRRH_REGISTER (PORTB_BASE_ADDRESS + 0x1A)
#define PORTF_LCKR_REGISTER (PORTB_BASE_ADDRESS + 0x1C)
#define PORTF_AFR1_REGISTER (PORTB_BASE_ADDRESS + 0x20)
#define PORTF_AFR2_REGISTER (PORTB_BASE_ADDRESS + 0x24)

// flags MODER Register:
#define GPIO_0_MODER 0x03
#define GPIO_0_MODER_OUT 0x01
#define GPIO_0_MODER_AF 0x02
#define GPIO_1_MODER (uint32_t)(0x03 << 2)
#define GPIO_1_MODER_OUT (uint32_t)(0x01 << 2)
#define GPIO_1_MODER_AF (uint32_t)(0x02 << 2)
#define GPIO_2_MODER (uint32_t)(0x03 << 4)
#define GPIO_2_MODER_OUT (uint32_t)(0x01 << 4)
#define GPIO_2_MODER_AF (uint32_t)(0x02 << 4)
#define GPIO_3_MODER (uint32_t)(0x03 << 6)
#define GPIO_3_MODER_OUT (uint32_t)(0x01 << 6)
#define GPIO_3_MODER_AF (uint32_t)(0x02 << 6)
#define GPIO_4_MODER (uint32_t)(0x03 << 8)
#define GPIO_4_MODER_OUT (uint32_t)(0x01 << 8)
#define GPIO_4_MODER_AF (uint32_t)(0x02 << 8)
#define GPIO_5_MODER (uint32_t)(0x03 << 10)
#define GPIO_5_MODER_OUT (uint32_t)(0x01 << 10)
#define GPIO_5_MODER_AF (uint32_t)(0x02 << 10)
#define GPIO_6_MODER 0x3000
#define GPIO_6_MODER_AF 0x2000
#define GPIO_6_MODER_OUT 0x1000
#define GPIO_6_MODER_IN 0x0000
#define GPIO_6_MODER_AN 0x3000
#define GPIO_7_MODER (uint32_t)(0x03 << 14)
#define GPIO_7_MODER_OUT (uint32_t)(0x01 << 14)
#define GPIO_7_MODER_AF (uint32_t)(0x02 << 14)
#define GPIO_8_MODER (uint32_t)(0x03 << 16)
#define GPIO_8_MODER_OUT (uint32_t)(0x01 << 16)
#define GPIO_8_MODER_AF (uint32_t)(0x02 << 16)
#define GPIO_9_MODER (uint32_t)(0x03 << 18)
#define GPIO_9_MODER_OUT (uint32_t)(0x01 << 18)
#define GPIO_9_MODER_AF (uint32_t)(0x02 << 18)
#define GPIO_10_MODER (uint32_t)(0x03 << 20)
#define GPIO_10_MODER_OUT (uint32_t)(0x01 << 20)
#define GPIO_10_MODER_AF (uint32_t)(0x02 << 20)

// flags OTYPER Register:
#define GPIO_0_OTYPER 0x01
#define GPIO_0_OTYPER_PP 0x00
#define GPIO_1_OTYPER (uint32_t)(0x01 << 1)
#define GPIO_1_OTYPER_PP 0x00
#define GPIO_2_OTYPER (uint32_t)(0x01 << 2)
#define GPIO_2_OTYPER_PP 0x00
#define GPIO_3_OTYPER (uint32_t)(0x01 << 3)
#define GPIO_3_OTYPER_PP 0x00
#define GPIO_4_OTYPER (uint32_t)(0x01 << 4)
#define GPIO_4_OTYPER_PP 0x00
#define GPIO_5_OTYPER (uint32_t)(0x01 << 5)
#define GPIO_5_OTYPER_PP 0x00
#define GPIO_6_OTYPER 0x40
#define GPIO_6_OTYPER_PP 0x00
#define GPIO_7_OTYPER (uint32_t)(0x01 << 7)
#define GPIO_7_OTYPER_PP 0x00
#define GPIO_8_OTYPER (uint32_t)(0x01 << 8)
#define GPIO_8_OTYPER_PP 0x00
#define GPIO_9_OTYPER (uint32_t)(0x01 << 9)
#define GPIO_9_OTYPER_PP 0x00
#define GPIO_10_OTYPER (uint32_t)(0x01 << 10)
#define GPIO_10_OTYPER_PP 0x00

// flags OSPEEDR Register:
#define GPIO_0_OSPEEDR_HIGH_SPEED 3
#define GPIO_6_OSPEEDR 0x3000
#define GPIO_6_OSPEEDR_HIGH_SPEED 0x3000

// flags PUPDR Register:
#define GPIO_0_PUPDR 0x03
#define GPIO_0_PUPDR_NOPULL 0x00
#define GPIO_0_PUPDR_PU 0x01
#define GPIO_0_PUPDR_PD 0x02
#define GPIO_1_PUPDR (uint32_t)(0x03 << 2)
#define GPIO_1_PUPDR_NOPULL 0x00
#define GPIO_1_PUPDR_PU (uint32_t)(0x01 << 2)
#define GPIO_1_PUPDR_PD (uint32_t)(0x02 << 2)
#define GPIO_2_PUPDR (uint32_t)(0x03 << 4)
#define GPIO_2_PUPDR_NOPULL 0x00
#define GPIO_2_PUPDR_PU (uint32_t)(0x01 << 4)
#define GPIO_2_PUPDR_PD (uint32_t)(0x02 << 4)
#define GPIO_3_PUPDR (uint32_t)(0x03 << 6)
#define GPIO_3_PUPDR_NOPULL 0x00
#define GPIO_3_PUPDR_PU (uint32_t)(0x01 << 6)
#define GPIO_3_PUPDR_PD (uint32_t)(0x02 << 6)
#define GPIO_4_PUPDR (uint32_t)(0x03 << 8)
#define GPIO_4_PUPDR_NOPULL 0x00
#define GPIO_4_PUPDR_PU (uint32_t)(0x01 << 8)
#define GPIO_4_PUPDR_PD (uint32_t)(0x02 << 8)
#define GPIO_5_PUPDR (uint32_t)(0x03 << 10)
#define GPIO_5_PUPDR_NOPULL 0x00
#define GPIO_5_PUPDR_PU (uint32_t)(0x01 << 10)
#define GPIO_5_PUPDR_PD (uint32_t)(0x02 << 10)
#define GPIO_6_PUPDR 0x3000
#define GPIO_6_PUPDR_NOPULL 0x00
#define GPIO_6_PUPDR_PD 0x2000
#define GPIO_6_PUPDR_PU 0x1000
#define GPIO_7_PUPDR (uint32_t)(0x03 << 14)
#define GPIO_7_PUPDR_NOPULL 0x00
#define GPIO_7_PUPDR_PU (uint32_t)(0x01 << 14)
#define GPIO_7_PUPDR_PD (uint32_t)(0x02 << 14)
#define GPIO_8_PUPDR (uint32_t)(0x03 << 16)
#define GPIO_8_PUPDR_NOPULL 0x00
#define GPIO_8_PUPDR_PU (uint32_t)(0x01 << 16)
#define GPIO_8_PUPDR_PD (uint32_t)(0x02 << 16)
#define GPIO_9_PUPDR (uint32_t)(0x03 << 18)
#define GPIO_9_PUPDR_NOPULL 0x00
#define GPIO_9_PUPDR_PU (uint32_t)(0x01 << 18)
#define GPIO_9_PUPDR_PD (uint32_t)(0x02 << 18)
#define GPIO_10_PUPDR (uint32_t)(0x03 << 20)
#define GPIO_10_PUPDR_NOPULL 0x00
#define GPIO_10_PUPDR_PU (uint32_t)(0x01 << 20)
#define GPIO_10_PUPDR_PD (uint32_t)(0x02 << 20)

// input data register:
#define GPIO_0_IDR 0x01
#define GPIO_1_IDR (uint32_t)(0x01 << 1)
#define GPIO_2_IDR (uint32_t)(0x01 << 2)
#define GPIO_3_IDR (uint32_t)(0x01 << 3)
#define GPIO_4_IDR (uint32_t)(0x01 << 4)
#define GPIO_5_IDR (uint32_t)(0x01 << 5)
#define GPIO_6_IDR 0x40
#define GPIO_7_IDR (uint32_t)(0x01 << 7)
#define GPIO_8_IDR (uint32_t)(0x01 << 8)
#define GPIO_9_IDR (uint32_t)(0x01 << 9)
#define GPIO_10_IDR (uint32_t)(0x01 << 10)
// flags AFR1 Register:
#define GPIO_0_AFR1_AF2 2
#define GPIO_0_AFR1 0xF
#define GPIO_6_AFR1 0xF000000
#define GPIO_6_AFR1_AF2 0x2000000
// flags ODR Register:
#define GPIO_0_ODR_HIGH 0x01
#define GPIO_1_ODR_HIGH (uint32_t)(0x01 << 1)
#define GPIO_2_ODR_HIGH (uint32_t)(0x01 << 2)
#define GPIO_3_ODR_HIGH (uint32_t)(0x01 << 3)
#define GPIO_4_ODR_HIGH (uint32_t)(0x01 << 4)
#define GPIO_5_ODR_HIGH (uint32_t)(0x01 << 5)
#define GPIO_6_ODR_HIGH 0x40
#define GPIO_7_ODR_HIGH (uint32_t)(0x01 << 7)
#define GPIO_8_ODR_HIGH (uint32_t)(0x01 << 8)
#define GPIO_9_ODR_HIGH (uint32_t)(0x01 << 9)
#define GPIO_10_ODR_HIGH (uint32_t)(0x01 << 10)

uint16_t adcvalues[3];
uint8_t adcindex = 0;
uint8_t adcnewvalue = 0;

void initGpioF7AsAnalog(void) {
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  GPIOF-> MODER |= (1 << 14) | (1 << 15);
  GPIOF-> OTYPER |= GPIO_7_OTYPER;
  // GPIOF->OSPEEDR |= 0x00000003;
  GPIOF->PUPDR |= GPIO_7_PUPDR; // Optional for Output
}

void initADC3_5_NoInterrupt(void) {
  uint32_t *reg_pointer;
  /* Initialize adcvalues*/
  adcvalues[0] = 0;
  adcindex = 0;
  adcnewvalue = 0;
  /* Turn on ADC3 bus clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  /* Initialize GPIO F 7 as analog */
  initGpioF7AsAnalog();
  /*Setup the clock Preescaler*/
  reg_pointer = (uint32_t *)ADC_COMMON_CCR_REGISTER;
  *reg_pointer = *reg_pointer | ADC_PREESCALER_4;
  /*Clear status register*/
  reg_pointer = (uint32_t *)ADC_3_SR_REGISTER;
  *reg_pointer = 0;
  /* Configure ADC 12bit resolution, End of conversion interrupt Disabled*/
  reg_pointer = (uint32_t *)ADC_3_CR1_REGISTER;
  *reg_pointer = 0;
  /* Configure ADC External trigger dissabled, right data alignment, no DMA, EOC
   * is set at the end of each regular conversion, single conversion enabled*/
  reg_pointer = (uint32_t *)ADC_3_CR2_REGISTER;
  *reg_pointer =
      *reg_pointer | ADC_EOCS; // since it is only a sequence of 1, this is not
                               // necessary but ok. /* There will be 1 channel
                               // in the sequene of conversions */
  reg_pointer = (uint32_t *)ADC_3_SQR1_REGISTER;
  *reg_pointer = *reg_pointer | ADC_1_CONVERSIONS;
  /*Configure the sequence of conversion for the ADC (5) */
  reg_pointer = (uint32_t *)ADC_3_SQR3_REGISTER;
  *reg_pointer = *reg_pointer | ADC_CHANNEL_5;
  /*Configure Channels 5 to max sampling times (480 cycles) */
  reg_pointer = (uint32_t *)ADC_3_SMPR2_REGISTER;
  *reg_pointer = *reg_pointer | ADC_SMP_5_MX;
  // Enable the ADC3*7
  reg_pointer = (uint32_t *)ADC_3_CR2_REGISTER;
  *reg_pointer = *reg_pointer | ADC_ADON;
  /*Start a software conversion*/
  // cannot start software conversion here, need to have a small delay //
  // allowing the ADC to actually turn on.
}

void startADCConversion(void) {
  uint32_t *reg_pointer;
  reg_pointer = (uint32_t *)ADC_3_SR_REGISTER;
  *reg_pointer = 0;
  reg_pointer = (uint32_t *)ADC_3_CR2_REGISTER;
  *reg_pointer = *reg_pointer | ADC_SWSTART;
}


uint32_t checkNewValueInSoftware(void ) {
  uint32_t *reg_pointer_32;
  uint32_t value;
  /*save the current value of the statur register*/
  reg_pointer_32 = (uint32_t *)ADC_3_SR_REGISTER;
  value = *reg_pointer_32;
  /* Is the end of conversion flag set */

  if ((value & ADC_EOC) > 0) {
    /*if it is set, read the value of the Data register and save it*/
    reg_pointer_32 = (uint32_t *)ADC_3_DR_REGISTER;
    adcvalues[adcindex] = *reg_pointer_32 & 0xFFF;
    startADCConversion();
    //debugprint(adcvalues[0]);
  }
    return adcvalues[0];
}

//==========================================================DMA

void initGpioF789AsAnalog(void) {
  uint32_t *reg_pointer;
  /* GPIOC Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  /* GPIOF Pins 7,8,9 as analog*/
  reg_pointer = (uint32_t *)PORTF_MODER_REGISTER;
  *reg_pointer =
      *reg_pointer | ((uint32_t)GPIO_7_MODER + (uint32_t)GPIO_8_MODER +
                      (uint32_t)GPIO_9_MODER);
  /* PUSH-PULL Pins*/
  reg_pointer = (uint32_t *)PORTF_OTYPER_REGISTER;
  *reg_pointer =
      *reg_pointer & (((uint32_t)GPIO_7_OTYPER) + ((uint32_t)GPIO_8_OTYPER) +
                      ((uint32_t)GPIO_9_OTYPER));
  /*Configure with no pull-up or pull-down*/
  reg_pointer = (uint32_t *)PORTF_PUPDR_REGISTER;
  *reg_pointer =
      *reg_pointer & ((((uint32_t)GPIO_7_PUPDR) + ((uint32_t)GPIO_8_PUPDR) +
                       ((uint32_t)GPIO_9_PUPDR)));
}

void enableDMAForAdc3_3channels(void) {
  uint32_t *reg_pointer;
  reg_pointer = (uint32_t *)DMA2_S0CR_ADDRESS;
  *reg_pointer = *reg_pointer | DMA_SxCR_STREAM_ENABLE;
}

uint16_t adcDmaDataStorageBuffer[3];

uint16_t returnADC3StoredValue(uint8_t index) {
  return adcDmaDataStorageBuffer[index];
}

void initDMAForAdc3_3channels(void) {
  uint32_t *reg_pointer;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  /* Configure stream 0 to use channel 2 (ADC3) */
  reg_pointer = (uint32_t *)DMA2_S0CR_ADDRESS;
  *reg_pointer = DMA_SxCR_CHANNEL_2_SELECT + DMA_SxCR_MSIZE_HALF_WORD +
                 DMA_SxCR_PSIZE_HALF_WORD + DMA_SxCR_MINC_INCREMENT +
                 DMA_SxCR_DIR_PERTOMEM + DMA_SxCR_CIRC_ENABLE;

  /* We will transfer 3 data registers for 3 channels of ADC*/
  reg_pointer = (uint32_t *)DMA2_S0NDTR_ADDRESS;
  *reg_pointer = 3;
  /* We will transfer from the ADC3 data register */
  reg_pointer = (uint32_t *)DMA2_S0PAR_ADDRESS;
  *reg_pointer = ADC_3_DR_REGISTER;
  /* We will transfer to the adc DmaDataStorageBuffer we just created */
  reg_pointer = (uint32_t *)DMA2_S0M0AR_ADDRESS;
  *reg_pointer = (uint32_t)&adcDmaDataStorageBuffer[0];
}

void initADC3_567_withDMA(void) {
  uint32_t *reg_pointer;
  /* Turn on ADC3 bus clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  /* Initialize GPIO F 7 8 9 as analog */
  initGpioF789AsAnalog();
  initDMAForAdc3_3channels();
  enableDMAForAdc3_3channels();
  /*Setup the clock Preescaler*/
  reg_pointer = (uint32_t *)ADC_COMMON_CCR_REGISTER;
  *reg_pointer = ADC_PREESCALER_4;
  /* Configure ADC 12bit resolution, End of conversion interrupt Disabled,
   * SCAN mode enabled to be able to scan a group of channels*/
  reg_pointer = (uint32_t *)ADC_3_CR1_REGISTER;
  *reg_pointer = *reg_pointer |= ADC_SCAN;
  /* Configure ADC External trigger dissabled, right data alignment, DMA,
  EOC is set at the end of each regular conversion, continuous conversion
  enabled*/
  reg_pointer = (uint32_t *)ADC_3_CR2_REGISTER;
  *reg_pointer = ADC_EOCS + ADC_CONT + ADC_DDS + ADC_DMA; //
  /* There will be 3 channels in the sequene of conversions */
  reg_pointer = (uint32_t *)ADC_3_SQR1_REGISTER;
  *reg_pointer = ADC_3_CONVERSIONS;
  /*Configure Channels 5,6,7 to max sampling times (480 cycles) */
  reg_pointer = (uint32_t *)ADC_3_SMPR2_REGISTER;
  *reg_pointer = ADC_SMP_5_MX + ADC_SMP_6_MX + ADC_SMP_7_MX; /*Configure the
                                  sequence of conversion for the ADC (5,6,7)*/
  reg_pointer = (uint32_t *)ADC_3_SQR3_REGISTER;
  *reg_pointer = (ADC_CHANNEL_5 + ADC_CHANNEL_6 + ADC_CHANNEL_7);
  /*Enable the ADC3*/
  reg_pointer = (uint32_t *)ADC_3_CR2_REGISTER;
  *reg_pointer = *reg_pointer | ADC_ADON;
  /*Start a software conversion*/ // need to do this separately
}

//==========================================================DMA