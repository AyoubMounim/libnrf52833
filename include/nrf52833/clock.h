
#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>


void hfClockInit(void);
void hfClockStart(void);
void hfClockStop(void);
void hfClockSetDebounce(uint8_t debounce);

void lfClockInit(void);
void lfClockConfigRC(void);
void lfClockConfigXtal(void);
void lfClockConfigSynth(void);
void lfClockStart(void);
void lfClockStop(void);
void lfClockSetDebounceNormal(void);
void lfClockSetDebounceExtended(void);

uint8_t hfClockIsRunning(void);
uint8_t lfClockIsRunning(void);

void setCTIV(uint8_t calibrationInterval);


#endif

