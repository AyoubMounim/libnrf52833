
#include "nrf52833/pwm.h"
#include "macros.h"
#include <stdint.h>


#define PWM_BASE(unit) (0x4001C000 + (unit)*0x5000)

#define PWM_SHORTS(unit) REG(uint32_t, PWM_BASE(unit), 0x200)
#define PWM_DECODER(unit) REG(uint32_t, PWM_BASE(unit), 0x510)


enum {
  PWM_SHORTS_SEQUEND0_STOP_POS = 0,
  PWM_SHORTS_SEQUEND1_STOP_POS = 1,
  PWM_SHORTS_LOOPSDONE_SEQSTART0_POS = 2,
  PWM_SHORTS_LOOPSDONE_SEQSTART1_POS = 3,
  PWM_SHORTS_LOOPSDONE_STOP_POS = 4,
};

enum {
  PWM_DECODER_LOAD_COMMON = 0,
  PWM_DECODER_LOAD_GROUP = 1,
  PWM_DECODER_LOAD_INDIVIDUAL = 2,
  PWM_DECODER_LOAD_WAVEFORM = 3,
  PWM_DECODER_LOAD_WIDTH = 2,
  PWM_DECODER_LOAD_POS = 0,
  PWM_DECODER_MODE_POS = 8,
};


typedef struct __attribute__((packed)){
  uint32_t _reserved0;
  uint32_t stop;
  uint32_t seqStart0;
  uint32_t seqStart1;
  uint32_t nextStep;
} PwmTask;

typedef struct __attribute__((packed)){
  uint32_t stopped;
  uint32_t seqStarted0;
  uint32_t seqStarted1;
  uint32_t seqEnd0;
  uint32_t seqEnd1;
  uint32_t pwmPeriodEnd;
  uint32_t loopsDone;
} PwmEvent;

typedef struct __attribute__((packed)){
  uint32_t inten;
  uint32_t intenSet;
  uint32_t intenClr;
} PwmInterrupt;

typedef struct __attribute__((packed)){
  uint32_t enable;
  uint32_t mode;
  uint32_t countertop;
  uint32_t prescaler;
  uint32_t _reserved0;
  uint32_t loop;
} PwmConfig;

typedef struct __attribute__((packed)){
  uint32_t seqPtr;
  uint32_t seqCnt;
  uint32_t seqRefresh;
  uint32_t seqEndDelay;
} PwmSeqConfig;

typedef struct __attribute__((packed)){
  uint32_t pselOut0;
  uint32_t pselOut1;
  uint32_t pselOut2;
  uint32_t pselOut3;
} PwmPin;


PwmTask volatile* pPwmTask[] = {
  (PwmTask volatile*) PWM_BASE(0),
  (PwmTask volatile*) PWM_BASE(1),
};
PwmEvent volatile* pPwmEvent[] = {
  (PwmEvent volatile*) (PWM_BASE(0) + 0x104),
  (PwmEvent volatile*) (PWM_BASE(1) + 0x104),
};
PwmInterrupt volatile* pPwmInterrupt[] = {
  (PwmInterrupt volatile*) (PWM_BASE(0) + 0x300),
  (PwmInterrupt volatile*) (PWM_BASE(1) + 0x300),
};
PwmConfig volatile* pPwmConfig[] = {
  (PwmConfig volatile*) (PWM_BASE(0) + 0x500),
  (PwmConfig volatile*) (PWM_BASE(1) + 0x500),
};
PwmSeqConfig volatile* pPwmSeqConfig[2][2] = {
  {
    (PwmSeqConfig volatile*) (PWM_BASE(0) + 0x520),
    (PwmSeqConfig volatile*) (PWM_BASE(0) + 0x540),
  },
  {
    (PwmSeqConfig volatile*) (PWM_BASE(1) + 0x520),
    (PwmSeqConfig volatile*) (PWM_BASE(1) + 0x540),
  },
};
PwmPin volatile* pPwmPin[] = {
  (PwmPin volatile*) (PWM_BASE(0) + 0x560),
  (PwmPin volatile*) (PWM_BASE(1) + 0x560),
};



