/**
 ******************************************************************************
 * @file    hardware_stm_gpio.c
 * @author  mortamar@andrew.cmu.edu
 * @version 1.0
 * @date    Septembr-2021
 * @brief   Controls STM32F446ze GPIO
 ******************************************************************************
 */

#include "hardware_stm_gpio.h"
#include "debug_mort.h"
#include "stm32f4xx_rcc_mort.h"

// led 1 is connected to PB0.
//  GPIO B addresses: 0x4002 0400 - 0x4002 07FF
//  GPIO C addresses: 0x4002 0800 - 0x4002 0BFF

/* MACRO definitions----------------------------------------------------------*/
// Port B addresses:
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
#define PORTB_OSPEEDR_REGISTER (PORTB_BASE_ADDRESS + 0x08)
// Port C addresses:
#define PORTC_BASE_ADDRESS ((uint32_t)0x40020800)
#define PORTC_MODER_REGISTER (PORTC_BASE_ADDRESS + 0x00)
#define PORTC_OTYPER_REGISTER (PORTC_BASE_ADDRESS + 0x04)
#define PORTC_OSPEEDR_REGISTER (PORTC_BASE_ADDRESS + 0x08)
#define PORTC_PUPDR_REGISTER (PORTC_BASE_ADDRESS + 0x0C)
#define PORTC_IDR_REGISTER (PORTC_BASE_ADDRESS + 0x10)
#define PORTC_ODR_REGISTER (PORTC_BASE_ADDRESS + 0x14)
#define PORTC_BSRRL_REGISTER (PORTC_BASE_ADDRESS + 0x18)
#define PORTC_BSRR_REGISTER (PORTC_BASE_ADDRESS + 0x18)
#define PORTC_BSRRH_REGISTER (PORTC_BASE_ADDRESS + 0x1A)
#define PORTC_LCKR_REGISTER (PORTC_BASE_ADDRESS + 0x1C)
#define PORTC_AFR1_REGISTER (PORTC_BASE_ADDRESS + 0x20)
#define PORTC_AFR2_REGISTER (PORTC_BASE_ADDRESS + 0x24)
#define PORTC_OSPEEDR_REGISTER (PORTC_BASE_ADDRESS + 0x08)

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

#define GPIO_13_MODER (uint32_t)(0x03 << 26)
#define GPIO_13_MODER_OUT (uint32_t)(0x01 << 26)
#define GPIO_13_MODER_AF (uint32_t)(0x02 << 26)
#define GPIO_13_MODER_IN 0x0000
#define GPIO_0_MODER_IN 0x0000

#define GPIO_14_MODER (uint32_t)(0x03 << 28)
#define GPIO_14_MODER_OUT (uint32_t)(0x01 << 28)
#define GPIO_14_MODER_AF (uint32_t)(0x02 << 28)

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

#define GPIO_13_OTYPER (uint32_t)(0x01 << 13)
#define GPIO_13_OTYPER_PP 0x00

#define GPIO_14_OTYPER (uint32_t)(0x01 << 14)
#define GPIO_14_OTYPER_PP 0x00

// flags OSPEEDR Register:
#define GPIO_0_OSPEEDR_HIGH_SPEED 3
#define GPIO_6_OSPEEDR 0x3000
#define GPIO_6_OSPEEDR_HIGH_SPEED 0x3000
#define GPIO_13_OSPEEDR_HIGH_SPEED 0xC000000

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

#define GPIO_13_PUPDR (uint32_t)(0x03 << 28)
#define GPIO_13_PUPDR_NOPULL 0x00
#define GPIO_13_PUPDR_PU (uint32_t)(0x01 << 26)
#define GPIO_13_PUPDR_PD (uint32_t)(0x02 << 26)

#define GPIO_14_PUPDR (uint32_t)(0x03 << 28)
#define GPIO_14_PUPDR_NOPULL 0x00
#define GPIO_14_PUPDR_PU (uint32_t)(0x01 << 28)
#define GPIO_14_PUPDR_PD (uint32_t)(0x02 << 28)
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
#define GPIO_13_IDR (uint32_t)(0x01 << 13)
#define GPIO_14_IDR (uint32_t)(0x01 << 14)
// flags AFR1 Register:
#define GPIO_0_AFR1_AF2 2
#define GPIO_0_AFR1 0xF
#define GPIO_6_AFR1 0xF000000
#define GPIO_6_AFR1_AF2 0x2000000
#define GPIO_7_AFR1 0xF0000000
#define GPIO_7_AFR1_AF2 0x20000000
#define GPIO_14_AFR1 0x0000000
#define GPIO_14_AFR1_AF2 0x20000000
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
#define GPIO_13_ODR_HIGH (uint32_t)(0x01 << 13)
#define GPIO_14_ODR_HIGH (uint32_t)(0x01 << 14)

/* function
 * definitions----------------------------------------------------------*/


//-------------------------------------------------------B0 LED1 GREEN START
void initGpioB0AsOutput(void) {
  uint32_t *reg_pointer;
  /* GPIOB Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  /* GPIOB0 configured as output */
  reg_pointer = (uint32_t *)PORTB_MODER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_0_MODER));
  *reg_pointer = *reg_pointer | GPIO_0_MODER_OUT;
  /*GPIOB0 configured as push-pull */
  reg_pointer = (uint32_t *)PORTB_OTYPER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_0_OTYPER));
  *reg_pointer = *reg_pointer | GPIO_0_OTYPER_PP;
  /*GPIOB0 configured floating */
  reg_pointer = (uint32_t *)PORTB_PUPDR_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_0_PUPDR));
  *reg_pointer = *reg_pointer | GPIO_0_PUPDR_NOPULL;
  /* GPIOB0 driven high to start out with */
  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;
  *reg_pointer = *reg_pointer | GPIO_0_ODR_HIGH;
  //*reg_pointer = *reg_pointer & (~((uint32_t)0b01));
}

void setGPIOB0(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer | 0b01;
}

void clearGPIOB0(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer & (~(uint32_t)0b01);
}

//-------------------------------------------------------B1--- NO DEBUG LED
void initGpioB1AsOutput(void) {
  uint32_t *reg_pointer;
  /* GPIOB Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  /* GPIOB0 configured as output */
  reg_pointer = (uint32_t *)PORTB_MODER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_1_MODER));
  *reg_pointer = *reg_pointer | GPIO_1_MODER_OUT;
  /*GPIOB0 configured as push-pull */
  reg_pointer = (uint32_t *)PORTB_OTYPER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_1_OTYPER));
  *reg_pointer = *reg_pointer | GPIO_1_OTYPER_PP;
  /*GPIOB0 configured floating */
  reg_pointer = (uint32_t *)PORTB_PUPDR_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_1_PUPDR));
  *reg_pointer = *reg_pointer | GPIO_1_PUPDR_NOPULL;
  /* GPIOB0 driven high to start out with */
  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;
  *reg_pointer = *reg_pointer | GPIO_1_ODR_HIGH;
  //*reg_pointer = *reg_pointer & (~((uint32_t)0b01));
}

void setGPIOB1(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer | 0b0010;
  //debugprint(11);
}

void clearGPIOB1(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer & (~(uint32_t)0b0010);
  //debugprint(10);
}

//-------------------------------------------------------B2--- NO DEBUG LED
void initGpioB2AsOutput(void) {
  uint32_t *reg_pointer;
  /* GPIOB Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  /* GPIOB0 configured as output */
  reg_pointer = (uint32_t *)PORTB_MODER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_2_MODER));
  *reg_pointer = *reg_pointer | GPIO_2_MODER_OUT;
  /*GPIOB0 configured as push-pull */
  reg_pointer = (uint32_t *)PORTB_OTYPER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_2_OTYPER));
  *reg_pointer = *reg_pointer | GPIO_2_OTYPER_PP;
  /*GPIOB0 configured floating */
  reg_pointer = (uint32_t *)PORTB_PUPDR_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_2_PUPDR));
  *reg_pointer = *reg_pointer | GPIO_2_PUPDR_NOPULL;
  /* GPIOB0 driven high to start out with */
  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;
  *reg_pointer = *reg_pointer | GPIO_2_ODR_HIGH;
  //*reg_pointer = *reg_pointer & (~((uint32_t)0b01));
}

void setGPIOB2(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer | 0b0100;
  //debugprint(21);
}

void clearGPIOB2(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer & (~(uint32_t)0b0100);
  //debugprint(20);
}

//-------------------------------------------------------B7 LED2 BLUE START

void initGpioB7AsOutput(void) {
  uint32_t *reg_pointer;
  /* GPIOB Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  /* GPIOB0 configured as output */
  reg_pointer = (uint32_t *)PORTB_MODER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_7_MODER));
  *reg_pointer = *reg_pointer | GPIO_7_MODER_OUT;
  /*GPIOB0 configured as push-pull */
  reg_pointer = (uint32_t *)PORTB_OTYPER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_7_OTYPER));
  *reg_pointer = *reg_pointer | GPIO_7_OTYPER_PP;
  /*GPIOB0 configured floating */
  reg_pointer = (uint32_t *)PORTB_PUPDR_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_7_PUPDR));
  *reg_pointer = *reg_pointer | GPIO_7_PUPDR_NOPULL;
  /* GPIOB0 driven high to start out with */
  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;
  *reg_pointer = *reg_pointer | GPIO_7_ODR_HIGH;
  //*reg_pointer = *reg_pointer & (~((uint32_t)0b01));
}

void toggleGPIOB7(void) {
  uint32_t value;
  uint32_t *reg_pointer;
  // get the current value of the pin
  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;
  value = *reg_pointer & GPIO_7_ODR_HIGH;
  if (value > 0) {
    // if high, clear the bit
    *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_7_ODR_HIGH));
  } else {
    // if low, set the bit
    *reg_pointer = *reg_pointer | GPIO_7_ODR_HIGH;
  }
}

void setGPIOB7(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer | 0b10000000;
}

void clearGPIOB7(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer & (~(uint32_t)0b10000000);
}

//-------------------------------------------------------B14 LED3 RED START
void initGpioB14AsOutput(void) {
  uint32_t *reg_pointer;
  /* GPIOB Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  /* GPIOB0 configured as output */
  reg_pointer = (uint32_t *)PORTB_MODER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_14_MODER));
  *reg_pointer = *reg_pointer | GPIO_14_MODER_OUT;
  /*GPIOB0 configured as push-pull */
  reg_pointer = (uint32_t *)PORTB_OTYPER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_14_OTYPER));
  *reg_pointer = *reg_pointer | GPIO_14_OTYPER_PP;
  /*GPIOB0 configured floating */
  reg_pointer = (uint32_t *)PORTB_PUPDR_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_14_PUPDR));
  *reg_pointer = *reg_pointer | GPIO_14_PUPDR_NOPULL;
  /* GPIOB0 driven high to start out with */
  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;
  *reg_pointer = *reg_pointer | GPIO_14_ODR_HIGH;
  //*reg_pointer = *reg_pointer & (~((uint32_t)0b01));
}

void toggleGPIOB14(void) {
  uint32_t value;
  uint32_t *reg_pointer;
  // get the current value of the pin
  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;
  value = *reg_pointer & GPIO_14_ODR_HIGH;
  if (value > 0) {
    // if high, clear the bit
    *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_14_ODR_HIGH));
  } else {
    // if low, set the bit
    *reg_pointer = *reg_pointer | GPIO_14_ODR_HIGH;
  }
}

void setGPIOB14(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer | 0b0100000000000000;
}

void clearGPIOB14(void) {
  uint32_t *reg_pointer;

  reg_pointer = (uint32_t *)PORTB_ODR_REGISTER;

  *reg_pointer = *reg_pointer & (~(uint32_t)0b0100000000000000);
}


//-------------------------------------------------------C6 INPUT START
void initGpioC6AsInput(void) {
  uint32_t *reg_pointer;
  /* GPIOC Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  /* GPIOC Pin 6 as input*/
  reg_pointer = (uint32_t *)PORTC_MODER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_6_MODER));
  *reg_pointer = *reg_pointer | GPIO_6_MODER_IN;
  /*PUSH-PULL Pin*/
  reg_pointer = (uint32_t *)PORTC_OTYPER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_6_OTYPER));
  *reg_pointer = *reg_pointer | GPIO_6_OTYPER_PP;
  /*GPIOC pin 6 high speed */
  reg_pointer = (uint32_t *)PORTC_OSPEEDR_REGISTER;
  *reg_pointer = *reg_pointer | GPIO_6_OSPEEDR_HIGH_SPEED;
  /*Configure pulled-down*/
  reg_pointer = (uint32_t *)PORTC_PUPDR_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_6_PUPDR));
  *reg_pointer = *reg_pointer | GPIO_6_PUPDR_PD;
}

uint32_t checkGPIOC6(void) {
  uint32_t value;
  uint32_t *reg_pointer;
  // get the current value of the pin
  reg_pointer = (uint32_t *)PORTC_IDR_REGISTER;
  value = *reg_pointer & GPIO_6_IDR;
  return value;
}



//------------------------------------------------------- C13 B1 START
void initGpioC13AsInput(void) {
  uint32_t *reg_pointer;
  /* GPIOC Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  /* GPIOC Pin 6 as input*/
  reg_pointer = (uint32_t *)PORTC_MODER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_0_MODER));
  *reg_pointer = *reg_pointer | GPIO_0_MODER_IN;
  /*PUSH-PULL Pin*/
  reg_pointer = (uint32_t *)PORTC_OTYPER_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_0_OTYPER));
  *reg_pointer = *reg_pointer | GPIO_0_OTYPER_PP;
  /*GPIOC pin 6 high speed */
  reg_pointer = (uint32_t *)PORTC_OSPEEDR_REGISTER;
  *reg_pointer = *reg_pointer | GPIO_0_OSPEEDR_HIGH_SPEED;
  /*Configure pulled-down*/
  reg_pointer = (uint32_t *)PORTC_PUPDR_REGISTER;
  *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_0_PUPDR));
  *reg_pointer = *reg_pointer | GPIO_0_PUPDR_PD;
}

uint32_t checkGPIOC13(void) {
  uint32_t value13 = GPIOC->IDR & (1 << 0);

  if (value13 == 1) {

  }

  if (value13 == 0) {

  }

  return value13;
}
