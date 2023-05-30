
#include "nrf52833/pwm.h"
#include "macros.h"
#include "nrf52833/gpio.h"
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
  uint32_t counterTop;
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


void pwm_init(Pwm const* const self);
void pwm_eventsReset(Pwm const* const self);


Pwm pwm_create(uint8_t const unit){
  Pwm pwm = {.unit=unit};
  pwm_init(&pwm);
  return pwm;
}

void pwm_reset(Pwm const* const self){
  pwm_stop(self);   
  pwm_init(self);
  return;
}


void pwm_stop(Pwm const* const self){
  if (!(pPwmEvent[self->unit]->seqStarted0) && !(pPwmEvent[self->unit]->seqStarted1)){
    return;
  }
  pPwmTask[self->unit]->stop = 1;
  while (!(pPwmEvent[self->unit]->stopped)){}
  pwm_eventsReset(self);
  return;
}

void pwm_start(Pwm const* const self, Sequence seq){
  if (seq){
    pPwmTask[self->unit]->seqStart1 = 1;
  }
  else {
    pPwmTask[self->unit]->seqStart0 = 1;
  }
  return;
}

void pwm_nextStep(Pwm const* const self){
  pPwmTask[self->unit]->nextStep = 1;
  return;
}


uint8_t pwm_isRunning(const Pwm *const self){
  uint8_t isRunning;
  if (!(pPwmEvent[self->unit]->seqStarted0) && !(pPwmEvent[self->unit]->seqStarted0)){
    isRunning = 0;
  }
  else if (!(pPwmEvent[self->unit]->loopsDone)){
    isRunning = 1;
  }
  else {
    isRunning = 0;
  }
  return isRunning;
}


void pwm_seqEndtoStopShortEnable(Pwm const* const self, Sequence seq){
  if (seq){
    SET_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_SEQUEND1_STOP_POS);
  }
  else {
    SET_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_SEQUEND0_STOP_POS);
  }
  return;
}

void pwm_seqEndtoStopShortDisable(Pwm const* const self, Sequence seq){
  if (seq){
    CLR_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_SEQUEND1_STOP_POS);
  }
  else {
    CLR_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_SEQUEND0_STOP_POS);
  }
  return;
}

void pwm_loopsDoneToSeqStartEnable(Pwm const* const self, Sequence seq){
  if (seq){
    SET_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_LOOPSDONE_SEQSTART1_POS);
  }
  else {
    SET_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_LOOPSDONE_SEQSTART0_POS);
  }
  return;
}

void pwm_loopsDoneToSeqStartDisable(Pwm const* const self, Sequence seq){
  if (seq){
    CLR_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_LOOPSDONE_SEQSTART1_POS);
  }
  else {
    CLR_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_LOOPSDONE_SEQSTART0_POS);
  }
  return;
}

void pwm_loopsDoneToStopEnable(Pwm const* const self){
  SET_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_LOOPSDONE_STOP_POS);
  return;
}

void pwm_loopsDoneToStopDisable(Pwm const* const self){
  CLR_BIT(PWM_SHORTS(self->unit), PWM_SHORTS_LOOPSDONE_STOP_POS);
  return;
}

void pwm_setDecoderLoadCommon(Pwm const* const self){
  SET_FIELD(
    PWM_DECODER(self->unit),
    PWM_DECODER_LOAD_POS,
    PWM_DECODER_LOAD_WIDTH,
    PWM_DECODER_LOAD_COMMON
  );
  return;
}

void pwm_setDecoderLoadGrouped(Pwm const* const self){
  SET_FIELD(
    PWM_DECODER(self->unit),
    PWM_DECODER_LOAD_POS,
    PWM_DECODER_LOAD_WIDTH,
    PWM_DECODER_LOAD_GROUP
  );
  return;
}

void pwm_setDecoderLoadIndividual(Pwm const* const self){
  SET_FIELD(
    PWM_DECODER(self->unit),
    PWM_DECODER_LOAD_POS,
    PWM_DECODER_LOAD_WIDTH,
    PWM_DECODER_LOAD_INDIVIDUAL
  );
  return;
}

void pwm_setDecoderLoadWaveForm(Pwm const* const self){
  SET_FIELD(
    PWM_DECODER(self->unit),
    PWM_DECODER_LOAD_POS,
    PWM_DECODER_LOAD_WIDTH,
    PWM_DECODER_LOAD_WAVEFORM
  );
}

void pwm_setDecoderModeRefresh(Pwm const* const self){
  CLR_BIT(
    PWM_DECODER(self->unit),
    PWM_DECODER_MODE_POS
  );
  return;
}
void pwm_setDecoderModeNextStep(Pwm const* const self){
  SET_BIT(
    PWM_DECODER(self->unit),
    PWM_DECODER_MODE_POS
  );
  return;
}

void pwm_enable(Pwm const* const self){
  uint32_t enable = 1;
  pPwmConfig[self->unit]->enable = enable;
  return;
}

void pwm_disable(Pwm const* const self){
  pPwmConfig[self->unit]->enable = 0;
  return;
}

void pwm_setModeUp(Pwm const* const self){
  pPwmConfig[self->unit]->mode = 0;
  return;
}

void pwm_setModeUpAndDown(Pwm const* const self){
  uint32_t enable = 1;
  pPwmConfig[self->unit]->mode = enable;
}

void pwm_setCounterTop(Pwm const* const self, uint32_t counterTop){
  pPwmConfig[self->unit]->counterTop = counterTop;
  return;
}

void pwm_setPrescaler(Pwm const* const self, Prescaler prescaler){
  pPwmConfig[self->unit]->prescaler = prescaler;
  return;
}

void pwm_setLoopsNumber(Pwm const* const self, uint16_t loopsNumber){
  pPwmConfig[self->unit]->loop = loopsNumber;
  return;
}

void pwm_setSeqPtr(Pwm const* const self, Sequence seq, uint32_t ptr){
  pPwmSeqConfig[self->unit][seq]->seqPtr = ptr;
  return;
}

void pwm_setSeqCnt(Pwm const* const self, Sequence seq, uint16_t cnt){
  pPwmSeqConfig[self->unit][seq]->seqCnt = cnt;
  return;
}
void pwm_setSeqRefresh(Pwm const* const self, Sequence seq, uint32_t refreshNum){
  pPwmSeqConfig[self->unit][seq]->seqRefresh = refreshNum;
  return;
}

void pwm_setSeqEndDelay(Pwm const* const self, Sequence seq, uint32_t delay){
  pPwmSeqConfig[self->unit][seq]->seqEndDelay = delay;
  return;
}

void pwm_setOutputPin(Pwm const* const self, Channel channel, Gpio const* const pin){
  gpio_configDisconnect(pin);
  gpio_configOutput(pin);
  gpio_setLow(pin);
  gpio_configConnect(pin);
  pwm_stop(self);
  pwm_disable(self);
  switch (channel){
    case CHANNEL_0:
      pPwmPin[self->unit]->pselOut0 = (pin->port << 5) + pin->pin;
      break;
    case CHANNEL_1:
      pPwmPin[self->unit]->pselOut1 = (pin->port << 5) + pin->pin;
      break;
    case CHANNEL_2:
      pPwmPin[self->unit]->pselOut2 = (pin->port << 5) + pin->pin;
      break;
    default:
      pPwmPin[self->unit]->pselOut3 = (pin->port << 5) + pin->pin;
      break;
  }
  pwm_enable(self);
  return;
}


void pwm_eventsReset(Pwm const* const self){
  pPwmEvent[self->unit]->stopped = 0;
  pPwmEvent[self->unit]->seqStarted0 = 0;
  pPwmEvent[self->unit]->seqStarted1= 0;
  pPwmEvent[self->unit]->seqEnd0 = 0;
  pPwmEvent[self->unit]->seqEnd1 = 0;
  pPwmEvent[self->unit]->pwmPeriodEnd = 0;
  pPwmEvent[self->unit]->loopsDone = 0;
  return;
}

void pwm_shortsReset(Pwm const* const self){
  pwm_seqEndtoStopShortDisable(self, 0);
  pwm_seqEndtoStopShortDisable(self, 1);
  pwm_loopsDoneToSeqStartDisable(self, 0);
  pwm_loopsDoneToSeqStartDisable(self, 1);
  pwm_loopsDoneToStopDisable(self);
  return;
}

void pwm_init(Pwm const* const self){
  pwm_disable(self);
  pwm_eventsReset(self);
  pwm_shortsReset(self);
  pwm_setModeUp(self);
  pwm_setCounterTop(self, 0x3FF);
  pwm_setPrescaler(self, DIV_1);
  pwm_setDecoderLoadCommon(self);
  pwm_setDecoderModeRefresh(self);
  pwm_setLoopsNumber(self, 0x0);
  for (int i = 0; i < 2; i++){
    pwm_setSeqPtr(self, i, 0x0);
    pwm_setSeqCnt(self, i, 0x0);
    pwm_setSeqRefresh(self, i, 0x1);
    pwm_setSeqEndDelay(self, i, 0x0);
  }
  pPwmPin[self->unit]->pselOut0 = 0xFFFFFFFF;
  pPwmPin[self->unit]->pselOut1 = 0xFFFFFFFF;
  pPwmPin[self->unit]->pselOut2 = 0xFFFFFFFF;
  pPwmPin[self->unit]->pselOut3 = 0xFFFFFFFF;
  pwm_enable(self);
  return;
}

