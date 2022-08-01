#ifndef ARM11_23_EXECUTE_H
#define ARM11_23_EXECUTE_H

#include "utils.h"

extern void execute( ARM * arm, instruction instr );
extern void executeMultiply( ARM * arm, instruction instr );
extern void executeDP( ARM * arm, instruction instr );
extern void executeSDT( ARM * arm, instruction instr );
extern void executeBranch( ARM * arm, instruction instr );
uint32_t shiftedRegister(ARM *arm, uint32_t *shiftedreg, reg *rm);
bool condMet(ARM arm, enum condition cond);
#endif //ARM11_23_EXECUTE_H
