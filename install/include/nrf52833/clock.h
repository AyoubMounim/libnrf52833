
#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include "macros.h"


#define CLOCK_BASE 0x40000000

#define CLOCK_HFCLKSTART     REG(uint32_t, CLOCK_BASE, 0x000)
#define CLOCK_LFCLKSTART     REG(uint32_t, CLOCK_BASE, 0x008)
#define CLOCK_HFCLKSTARTED   REG(uint32_t, CLOCK_BASE, 0x100)
#define CLOCK_LFCLKSTARTED   REG(uint32_t, CLOCK_BASE, 0x104)
#define CLOCK_LFCLKSRC       REG(uint32_t, CLOCK_BASE, 0x518)

#define CLOCK_LFCLKRC_RC     0
#define CLOCK_XTALFREQ_16MHz 0xFF


void sleep(uint32_t usec);


#endif
