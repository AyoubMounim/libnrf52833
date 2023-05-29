
#ifndef PWM_H
#define PWM_H

#include "nrf52833/gpio.h"
#include <stdint.h>


typedef struct {
  uint8_t const unit;
} Pwm;


typedef enum {
  SEQ_0,
  SEQ_1
} Sequence;

typedef enum {
  CHANNEL_0,
  CHANNEL_1,
  CHANNEL_2,
  CHANNEL_3,
} Channel;

typedef enum {
  DIV_1,
  DIV_2,
  DIV_4,
  DIV_8,
  DIV_16,
  DIV_32,
  DIV_64,
  DIV_128,
} Prescaler;


Pwm pwm_create(uint8_t const unit);
void pwm_reset(Pwm const* const self);

void pwm_stop(Pwm const* const self);
void pwm_start(Pwm const* const self, Sequence seq);
void pwm_nextStep(Pwm const* const self);

void pwm_seqEndtoStopShortEnable(Pwm const* const self, Sequence seq);
void pwm_seqEndtoStopShortDisable(Pwm const* const self, Sequence seq);
void pwm_loopsDoneToSeqStartEnable(Pwm const* const self, Sequence seq);
void pwm_loopsDoneToSeqStartDisable(Pwm const* const self, Sequence seq);
void pwm_loopsDoneToStopEnable(Pwm const* const self);
void pwm_loopsDoneToStopDisable(Pwm const* const self);

void pwm_setDecoderLoadCommon(Pwm const* const self);
void pwm_setDecoderLoadGrouped(Pwm const* const self);
void pwm_setDecoderLoadIndividual(Pwm const* const self);
void pwm_setDecoderLoadWaveForm(Pwm const* const self);
void pwm_setDecoderModeRefresh(Pwm const* const self);
void pwm_setDecoderModeNextStep(Pwm const* const self);

void pwm_enable(Pwm const* const self);
void pwm_disable(Pwm const* const self);
void pwm_setModeUp(Pwm const* const self);
void pwm_setModeUpAndDown(Pwm const* const self);
void pwm_setCounterTop(Pwm const* const self, uint32_t counterTop);
void pwm_setPrescaler(Pwm const* const self, Prescaler prescaler);
void pwm_setLoopsNumber(Pwm const* const self, uint16_t loopsNumber);

void pwm_setSeqPtr(Pwm const* const self, Sequence seq, uint32_t ptr);
void pwm_setSeqCnt(Pwm const* const self, Sequence seq, uint16_t cnt);
void pwm_setSeqRefresh(Pwm const* const self, Sequence seq, uint32_t refreshNum);
void pwm_setSeqEndDelay(Pwm const* const self, Sequence seq, uint32_t delay);

void setOutputPin(Pwm const* const self, Channel channel, Gpio* pin);




#endif
