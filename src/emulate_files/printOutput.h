#ifndef ARM11_23_PRINTOUTPUT_H
#define ARM11_23_PRINTOUTPUT_H
#include <stdint.h>
#include "utils.h"
extern int log_10( uint32_t value );
extern void printState( ARM arm );
extern uint32_t cpsrOutput( ARM arm );
#endif //ARM11_23_PRINTOUTPUT_H
