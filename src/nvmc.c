
#include "nrf52833/nvmc.h"
#include "macros.h"
#include <stdint.h>


#define NVMC_BASE 0x4001E000

#define NVMC_READY      REG(uint32_t, NVMC_BASE, 0x400)
#define NVMC_CONFIG     REG(uint32_t, NVMC_BASE, 0x504)
#define NVMC_ERASEPAGE  REG(uint32_t, NVMC_BASE, 0x508)
#define NVMC_ICACHECONF REG(uint32_t, NVMC_BASE, 0x540)

#define NVMC_CONFIG_WEN 0
#define NVMC_CONFIG_EEN 1
#define NVMC_ICACHECONF_CACHEEN 0
