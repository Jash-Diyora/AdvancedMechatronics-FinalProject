#include "mbed.h"
#include "debug_mort.h"

void debugprint(uint32_t number)
{
    printf("Got to %u\n",number);    
}

void debugprintHelloWorld( void )
{
    printf("heartbeat debug print\n");
} 
