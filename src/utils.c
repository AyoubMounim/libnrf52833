
#include "utils.h"
#include "nrf52833/clock.h"
#include "nrf52833/counter.h"


uint32_t strLength(const char* str){
  uint32_t len = 0;
  while (*(str + len) != '\0'){
    len++;
  }
  return len;
}


uint8_t strCompare(const char string_1[], const char string_2[]){
  int len_string_1 = strLength(string_1);
  int len_string_2 = strLength(string_2);
  if (len_string_1 != len_string_2){
    return 0;
  }
  for (int i=0; i<len_string_1; i++){
    if (string_1[i] != string_2[i]){
      return 0;
    }
  }
  return 1;
}


uint8_t numberOfDigits(uint32_t integer, uint8_t base){
  uint8_t nDigits = 0;
  do {
      nDigits++; 
      integer /= base;
  } while (integer > 0);
  return nDigits;
}

