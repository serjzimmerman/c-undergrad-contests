CC=gcc

CFLAGS=-Wall -O0 --std=gnu11 -lm -g
CLANGF=-style='{BasedOnStyle: LLVM, IndentWidth: 2,\
								AlignConsecutiveMacros: None,\
								AllowShortBlocksOnASingleLine: Never,\
								AllowShortIfStatementsOnASingleLine: Never,\
								AllowShortFunctionsOnASingleLine: None}'

TARGET=main

all:
	$(CC) $(CFLAGS) src/$(TARGET).c -o $(TARGET).elf -lm

clean:
	rm $(TARGET).elf

clang-format:
	clang-format $(CLANGF) -i --verbose src/$(TARGET).c
