#include "main.h"
#include "debug_mort.h"
#include "hardware_stm_gpio.h"
#include "mbed.h"
#include "stdint.h"
#include "stm32f4xx.h"
#include "adc.h"

PwmOut Flower(PA_6);
PwmOut Cologne(PA_5);

uint32_t inputLeftSensor = 0;
uint32_t inputphotoResSensor = 0;
uint32_t adcVal = 0;

bool leftInit = false;
bool photoResInit = false;
bool leftDone = false;
bool photoResDone = false;
bool smallInteract = false;

uint32_t MAP(uint32_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax);

int main() {
  Flower.pulsewidth_us(400);  // home
  Cologne.pulsewidth_us(500); // home

  initGpioB0AsOutput();  // flower left
  initGpioB7AsOutput();  // flower center
  initGpioB14AsOutput(); // flower right

  initGpioC13AsInput();
  initGpioC6AsInput();

  clearGPIOB0();
  clearGPIOB7();
  clearGPIOB14();

  initADC3_5_NoInterrupt();
  startADCConversion();

  while (1) {
    debugprint(checkGPIOC13());
    debugprint(checkGPIOC6());

    inputLeftSensor = checkGPIOC6();      // 64
    inputphotoResSensor = checkGPIOC13(); // 8192
    adcVal = checkNewValueInSoftware();

    if (adcVal > 200 && leftInit ==false){
        uint32_t mapped = MAP(adcVal, 200, 4500, 300,2500);
        Flower.pulsewidth_us(mapped);
        wait(0.2);
    }

    if (inputLeftSensor == 0 && leftInit ==false) //--------Set flags for action
    {
        leftInit = true;
        leftDone = false; // service
    }

    else if(inputphotoResSensor == 1 && inputLeftSensor == 64 && leftInit == false)
    {
        smallInteract = true;
    }

    if(smallInteract == true)
    {
        setGPIOB0();
        setGPIOB7();
        setGPIOB14();

        for (int i = 200; i < 2400; i = i + 200) {
            Flower.pulsewidth_us(i);
            wait(0.2);
        }

        wait(1);

        for (int i = 500; i < 2400; i = i + 200) {
            Cologne.pulsewidth_us(i);
            wait(0.2);
        }

        wait(1);

        for (int i = 2400; i < 500; i = i - 200) {
            Cologne.pulsewidth_us(i);
            wait(0.2);
        }

        wait(10); //Back to welcome - action
        clearGPIOB0();
        clearGPIOB7();
        clearGPIOB14();
    
        for (int j = 2400; j > 200; j = j - 200) {
            Flower.pulsewidth_us(j);
            wait(0.2);
        }


        smallInteract = false;
    }

    if (leftInit == true && leftDone == false) //-----------action when someone walks by sensor
    {
        clearGPIOB0();
        clearGPIOB7();
        clearGPIOB14();

        wait(1);

        clearGPIOB0();
        clearGPIOB7();
        setGPIOB14(); // left

        for (int i = 200; i < 2400; i = i + 200) {
            Flower.pulsewidth_us(i);
            wait(0.2);
        }

        wait(1);

        for (int j = 2400; j > 200; j = j - 200) {
            Flower.pulsewidth_us(j);
            wait(0.2);
        }
        
        clearGPIOB0();
        clearGPIOB7();
        clearGPIOB14();

        wait(2);

        clearGPIOB0();
        clearGPIOB14();
        setGPIOB7(); // centre
      
        for (int i = 200; i < 2400; i = i + 200) {
            Flower.pulsewidth_us(i);
            wait(0.2);
        }

        wait(1);

        for (int j = 2400; j > 200; j = j - 200) {
            Flower.pulsewidth_us(j);
            wait(0.2);
        }
      
        clearGPIOB0();
        clearGPIOB7();
        clearGPIOB14();

        wait(2);

        clearGPIOB14();
        clearGPIOB7();
        setGPIOB0(); // right

        for (int i = 200; i < 2400; i = i + 200) {
            Flower.pulsewidth_us(i);
            wait(0.2);
        }

        wait(1);

        for (int j = 2400; j > 200; j = j - 200) {
            Flower.pulsewidth_us(j);
            wait(0.2);
        }

        clearGPIOB0();
        clearGPIOB7();
        clearGPIOB14();

        wait(2); //Towards 10 second interaction 

        setGPIOB0();
        setGPIOB7();
        setGPIOB14();

        for (int i = 200; i < 2400; i = i + 200) {
            Flower.pulsewidth_us(i);
            wait(0.2);
        }

        wait(1);

        for (int i = 500; i < 2400; i = i + 200) {
            Cologne.pulsewidth_us(i);
            wait(0.2);
        }

        wait(1);

        for (int i = 2400; i < 500; i = i - 200) {
            Cologne.pulsewidth_us(i);
            wait(0.2);
        }

        wait(10); //Back to welcome - action
        clearGPIOB0();
        clearGPIOB7();
        clearGPIOB14();
    
        for (int j = 2400; j > 200; j = j - 200) {
            Flower.pulsewidth_us(j);
            wait(0.2);
        }

        leftDone = true;
        leftInit = false; // back to welcome - flag
    }
  }
}
uint32_t MAP(uint32_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax)
{
    return ((((au32_IN - au32_INmin)*(au32_OUTmax - au32_OUTmin))/(au32_INmax - au32_INmin)) + au32_OUTmin);
}