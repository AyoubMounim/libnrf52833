
#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>


void radioInit(void);
void radioReset(void);

void radioSetCRCIeee(void);
void radioSetModeIee(void);
void radioSetMaxLen(uint8_t maxLen);
void radioSetStatLen(uint8_t statLen);
void radioSetFrequency(uint16_t freqMhz);
void radioSetRxState(void);

void radioShortReadyToStartEnable(void);
void radioShortReadyToStartDisable(void);
void radioShortEndToDisableEnable(void);
void radioShortEndToDisableDisable(void);

void radioReceive(uint8_t* rxBuffer);
uint8_t radioEd(uint32_t edCnt);

#endif
