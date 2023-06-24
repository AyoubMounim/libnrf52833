
#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>


void radioInit(void);
void radioReset(void);

void radioSetCRCIeee(void);
void radioSetModeIEEE(void);
void radioSetModeBLE1Mbps(void);
void radioSetMaxLen(uint8_t maxLen);
void radioSetStatLen(uint8_t statLen);
void radioSetFrequency(uint16_t freqMhz);
void radioSetRxState(void);
void radioSetTxState(void);
void radioSetTxState(void);
void radioSetSFD(uint8_t sfd);
void radioSetTxLogicalAddr(uint8_t addr);
void radioSetRxLogicalAddr(uint8_t addr);
void radioSetAddr(uint8_t logicalAddr, uint32_t addr);

void radioShortReadyToStartEnable(void);
void radioShortReadyToStartDisable(void);
void radioShortEndToDisableEnable(void);
void radioShortEndToDisableDisable(void);
void radioShortReadyToCCAstartEnable(void);
void radioShortReadyToCCAstartDisable(void);
void radioShortCCAidleToTxEnableEnable(void);
void radioShortCCAidleToTxEnableDisable(void);

void radioReceive(uint8_t* rxBuffer);
uint8_t radioTransmit(uint8_t* txBuffer);
uint8_t radioBLE1MbpsTransmit(uint8_t* txBuffer);
uint8_t radioEd(uint32_t edCnt);
uint8_t radioRSSI(void);

#endif

