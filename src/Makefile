CC					=	gcc
CFLAGS				=	-Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -pedantic -O3 -Werror
BUILD				=	emulate assemble foodBank foodBankTest
EMULATE_OBJS		=	emulate.o emulate_files/decode.o emulate_files/execute.o emulate_files/fetch.o emulate_files/printOutput.o
ASSEMBLE_OBJS		=	assemble.o assemble_files/symbolTable/symbolTable.o assemble_files/assembleFunctions/assembleFunctions.o assemble_files/fileIO.o assemble_files/twoPassAssembly/twoPassAssembly.o assemble_files/twoPassAssembly/twoPassAssemblyUtils.o assemble_files/assembleFunctions/assembleUtils.o
FOOD_BANK_OBJS		=	foodBank.o extensionFoodBank/srcFoodBank/foodBankSystem.o
SRC					=	extensionFoodBank/srcFoodBank/foodBankSystem.c
FOOD_BANK_TEST_OBJS	=	extensionFoodBank/testsFoodBank/tests.c
COMMON_OBJS 		=	common/common_utils.o
LIBS				=	-lm

.PHONY: all clean

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) $(EMULATE_OBJS) $(ASSEMBLE_OBJS) $(FOOD_BANK_OBJS) $(COMMON_OBJS) core a.out .nfs*

emulate:	$(EMULATE_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(COMMON_OBJS) $(EMULATE_OBJS)  $(LDFLAGS) $(LIBS) -o emulate $(LDLIBS)

assemble:	$(ASSEMBLE_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(COMMON_OBJS) $(ASSEMBLE_OBJS) $(LDFLAGS) $(LIBS) -o assemble $(LDLIBS)

foodBank:	$(FOOD_BANK_OBJS)
	$(CC) $(CFLAGS) $(FOOD_BANK_OBJS) $(LDFLAGS) $(LIBS) -o foodBank $(LDLIBS)

foodBankTest:	$(FOOD_BANK_TEST_OBJS) $(FOOD_BANK_OBJS)
	$(CC) $(CFLAGS) $(FOOD_BANK_TEST_OBJS) $(SRC) $(LDFLAGS) $(LIBS) -o foodBankTest $(LDLIBS)

assemble.o:	assemble.c assemble_files/symbolTable/symbolTable.h common/common_utils.h assemble_files/assemble_constants.h assemble_files/assembleFunctions/assembleFunctions.h assemble_files/fileIO.h assemble_files/twoPassAssembly/twoPassAssembly.h assemble_files/twoPassAssembly/twoPassAssemblyUtils.h assemble_files/assembleFunctions/assembleUtils.h
assemble_files/symbolTable/symbolTable.o:	assemble_files/symbolTable/symbolTable.c assemble_files/symbolTable/symbolTable.h assemble_files/assemble_constants.h
assemble_files/assembleFunctions/assembleFunctions.o:	assemble_files/assembleFunctions/assembleFunctions.c assemble_files/symbolTable/symbolTable.h assemble_files/assemble_constants.h assemble_files/assembleFunctions/assembleUtils.h
assemble_files/fileIO.o:	assemble_files/fileIO.c assemble_files/fileIO.h assemble_files/assemble_constants.h
assemble_files/twoPassAssembly/twoPassAssembly.o:	assemble_files/twoPassAssembly/twoPassAssembly.c assemble_files/twoPassAssembly/twoPassAssembly.h assemble_files/assemble_constants.h assemble_files/fileIO.h assemble_files/symbolTable/symbolTable.h
assemble_files/twoPassAssembly/twoPassAssemblyUtils.o:	assemble_files/twoPassAssembly/twoPassAssemblyUtils.c assemble_files/symbolTable/symbolTable.h
assemble_files/assembleFunctions/assembleUtils.o:	assemble_files/assembleFunctions/assembleUtils.c assemble_files/assembleFunctions/assembleUtils.h assemble_files/assemble_constants.h
emulate.o:	emulate.c emulate_files/decode.h emulate_files/execute.h emulate_files/fetch.h emulate_files/utils.h emulate_files/constants.h emulate_files/printOutput.h
emulate_files/decode.o:	emulate_files/decode.c emulate_files/decode.h emulate_files/constants.h
emulate_files/execute.o:	emulate_files/execute.c emulate_files/execute.h emulate_files/decode.h emulate_files/utils.h emulate_files/constants.h common/common_utils.h
emulate_files/fetch.o:	emulate_files/fetch.c emulate_files/fetch.h emulate_files/utils.h emulate_files/constants.h
emulate_files/printOutput.o:	emulate_files/printOutput.c emulate_files/printOutput.h emulate_files/utils.h emulate_files/constants.h common/common_utils.h
common/common_utils.o:	common/common_utils.c common/common_utils.h
extensionFoodBank/srcFoodBank/foodBankSystem.o:	extensionFoodBank/srcFoodBank/foodBankSystem.c extensionFoodBank/srcFoodBank/foodBankSystem.h
foodBank.o:	foodBank.c extensionFoodBank/srcFoodBank/foodBankSystem.h
