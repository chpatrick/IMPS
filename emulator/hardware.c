#include <string.h>
#include <stdint.h>

#include "hardware.h"

uint32_t getWord(uint16_t address) {
  uint32_t value;
  memcpy(&value, memory + address, 4);

#ifdef __BIG_ENDIAN__
  value = __builtin_bswap32(value);
#endif

  return value;
}

void setWord(uint16_t address, uint32_t value) {
#ifdef  __BIG_ENDIAN__
  value = __builtin_bswap32(value);
#endif

  memcpy(memory + address, &value, 4);
}
