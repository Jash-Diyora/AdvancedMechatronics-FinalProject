/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_GPIO_H_
#define __HARDWARE_STM_GPIO_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Macros for Everyone--------------------------------------------------------*/
#define PIN_0   0
#define PIN_1   1
#define PIN_2   2
#define PIN_3   3
#define PIN_4   4
#define PIN_5   5
#define PIN_6   6
#define PIN_7   7
#define PIN_8   8
#define PIN_9   9
#define PIN_10  10


/*Function definitions---------------------------------------------------------*/
void initGpioB0AsOutput( void );
//void initGpioB0AsAF2( void );
void toggleGPIOB0( void );
void setGPIOB0( void );
void clearGPIOB0( void );

void initGpioB1AsOutput( void );
//void initGpioB1AsAF2( void );
void toggleGPIOB1( void );
void setGPIOB1( void );
void clearGPIOB1( void );

void initGpioB2AsOutput( void );
//void initGpioB2AsAF2( void );
void toggleGPIOB2( void );
void setGPIOB2( void );
void clearGPIOB2( void );

void initGpioB7AsOutput( void );
//void initGpioB7AsAF2( void );
void toggleGPIOB7( void );
void setGPIOB7( void );
void clearGPIOB7( void );

void initGpioB14AsOutput( void );
//void initGpioB14AsAF2( void );
void toggleGPIOB14( void );
void setGPIOB14( void );
void clearGPIOB14( void );

//void initGpioC6AsAF2( void );
void initGpioC6AsInput( void );
uint32_t checkGPIOC6(void);

void initGpioC13AsInput( void );
uint32_t checkGPIOC13(void);

#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */