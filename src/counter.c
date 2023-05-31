
#include "nrf52833/counter.h"
#include "macros.h"
#include <stdint.h>


#define COUNTER_BASE(unit) (0x4000B000 + (unit)*0x6000)

#define COUNTER_EVTEN(unit) REG(uint32_t, COUNTER_BASE(unit), 0x340)
#define COUNTER_COUNTER(unit) REG(uint32_t, COUNTER_BASE(unit), 0x504)
#define COUNTER_PRESCALE(unit) REG(uint32_t, COUNTER_BASE(unit), 0x508)

#define MICRO_FACTOR (1000000)


enum {
    COUNTER_EVTEN_TICK_POS = 0,
    COUNTER_EVTEN_OVRFLW_POS = 1,
    COUNTER_EVTEN_COMPARE0_POS = 16,
    COUNTER_EVTEN_COMPARE1_POS = 17,
    COUNTER_EVTEN_COMPARE2_POS = 18,
    COUNTER_EVTEN_COMPARE3_POS = 19
};


typedef struct __attribute__((packed)){
    uint32_t start;
    uint32_t stop;
    uint32_t clear;
    uint32_t trigOvrFlw;
} CounterTask;

typedef struct __attribute__((packed)){
    uint32_t tick;
    uint32_t ovrFlw;
} CounterEvent;

typedef struct __attribute__((packed)){
    uint32_t intenSet;
    uint32_t intenClr;
} CounterInterrupt;

typedef struct __attribute__((packed)){
    uint32_t compare0;
    uint32_t compare1;
    uint32_t compare2;
    uint32_t compare3;
} CounterCompareEvent;

typedef struct __attribute__((packed)){
    uint32_t compare0;
    uint32_t compare1;
    uint32_t compare2;
    uint32_t compare3;
} CounterCompareValue;


CounterTask volatile* pCounterTask[] = {
    (CounterTask volatile*) COUNTER_BASE(0),
    (CounterTask volatile*) COUNTER_BASE(1),
};
CounterEvent volatile* pCounterEvent[] = {
    (CounterEvent volatile*) (COUNTER_BASE(0) + 0x100),
    (CounterEvent volatile*) (COUNTER_BASE(1) + 0x100),
};
CounterInterrupt volatile* pCounterInterrupt[] = {
    (CounterInterrupt volatile*) (COUNTER_BASE(0) + 0x304),
    (CounterInterrupt volatile*) (COUNTER_BASE(1) + 0x304),
};
CounterCompareEvent volatile* pCounterCompareEvent[] = {
    (CounterCompareEvent volatile*) (COUNTER_BASE(0) + 0x140),
    (CounterCompareEvent volatile*) (COUNTER_BASE(1) + 0x140),
};
CounterCompareValue volatile* pCounterCompareValue[] = {
    (CounterCompareValue volatile*) (COUNTER_BASE(0) + 0x540),
    (CounterCompareValue volatile*) (COUNTER_BASE(1) + 0x540),
};


void counter_init(const Counter *const self);


Counter counter_create(uint8_t const unit, uint32_t const period){
    Counter counter = {.unit=unit, .periodMicroSec=period};
    counter_init(&counter);
    return counter;
}

void counter_reset(Counter const* const self){
    counter_init(self);
    return;
}


uint32_t computePrescaler(uint32_t periodMicroSec){
    uint32_t prescaler = (32768*periodMicroSec)/MICRO_FACTOR - 1;
    return prescaler;
}

void counter_setPrescaler(Counter const* const self, uint32_t prescaler){
    COUNTER_PRESCALE(self->unit) = prescaler;
    return;
}

void counter_eventsReset(Counter const* const self){
    pCounterEvent[self->unit]->tick = 0;
    pCounterEvent[self->unit]->ovrFlw = 0;
    pCounterCompareEvent[self->unit]->compare0 = 0;
    pCounterCompareEvent[self->unit]->compare1 = 0;
    pCounterCompareEvent[self->unit]->compare2 = 0;
    pCounterCompareEvent[self->unit]->compare3 = 0;
    return;
}

void counter_interruptReset(Counter const* const self){
    SET_BIT(pCounterInterrupt[self->unit]->intenClr, COUNTER_EVTEN_TICK_POS);
    SET_BIT(pCounterInterrupt[self->unit]->intenClr, COUNTER_EVTEN_OVRFLW_POS);
    SET_BIT(pCounterInterrupt[self->unit]->intenClr, COUNTER_EVTEN_COMPARE0_POS);
    SET_BIT(pCounterInterrupt[self->unit]->intenClr, COUNTER_EVTEN_COMPARE1_POS);
    SET_BIT(pCounterInterrupt[self->unit]->intenClr, COUNTER_EVTEN_COMPARE2_POS);
    SET_BIT(pCounterInterrupt[self->unit]->intenClr, COUNTER_EVTEN_COMPARE3_POS);
    return;
}

void counter_eventRoutingReset(Counter const* const self){
    CLR_BIT(COUNTER_EVTEN(self->unit), COUNTER_EVTEN_TICK_POS);
    CLR_BIT(COUNTER_EVTEN(self->unit), COUNTER_EVTEN_OVRFLW_POS);
    CLR_BIT(COUNTER_EVTEN(self->unit), COUNTER_EVTEN_COMPARE0_POS);
    CLR_BIT(COUNTER_EVTEN(self->unit), COUNTER_EVTEN_COMPARE1_POS);
    CLR_BIT(COUNTER_EVTEN(self->unit), COUNTER_EVTEN_COMPARE2_POS);
    CLR_BIT(COUNTER_EVTEN(self->unit), COUNTER_EVTEN_COMPARE3_POS);
    return;
}

void counter_init(Counter const* const self){
    pCounterTask[self->unit]->stop = 1;
    pCounterTask[self->unit]->clear = 1;
    uint32_t prescaler = computePrescaler(self->periodMicroSec);
    counter_setPrescaler(self, prescaler);
    counter_eventsReset(self);
    counter_interruptReset(self);
    counter_eventRoutingReset(self);
    return;
}

