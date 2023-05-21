
#include "clock.h"


void sleep(uint32_t usec){
    uint32_t t = usec << 20;
    while (t > 0){
        nop();
        t--;
    }
    return;
}
